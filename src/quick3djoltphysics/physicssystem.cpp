#include "physicssystem_p.h"

#include "body_p.h"
#include "physicsutils_p.h"

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/CastResult.h>
#include <Jolt/Physics/Collision/RayCast.h>
#include <Jolt/Physics/Collision/NarrowPhaseQuery.h>

Q_LOGGING_CATEGORY(lcQuick3dJoltPhysics, "qt.quick3d.joltphysics")

using PhysicsSystems = QVector<PhysicsSystem *>;
Q_GLOBAL_STATIC(PhysicsSystems, g_physicsSystems)

int g_factoryRefCount = 0;

PhysicsSystem::PhysicsSystem(QObject *parent)
    : QObject(parent)
    , m_animation(new PhysicsSystemAnimation(this))
{
    if (g_factoryRefCount == 0) {
        JPH::RegisterDefaultAllocator();
        JPH::Factory::sInstance = new JPH::Factory;
        JPH::RegisterTypes();
    }
    g_factoryRefCount++;
    m_tempAllocator = new JPH::TempAllocatorImpl(32 * 1024 * 1024);
}

PhysicsSystem::~PhysicsSystem()
{
    for (auto physicsNode : std::as_const(m_physicsNodes))
        physicsNode->cleanup();

    g_factoryRefCount--;
    if (g_factoryRefCount == 0) {
        JPH::UnregisterTypes();
        delete JPH::Factory::sInstance;
        JPH::Factory::sInstance = nullptr;
    }

    delete m_jolt;
    m_jolt = nullptr;
    delete m_tempAllocator;
    m_tempAllocator = nullptr;

    g_physicsSystems->removeAll(this);
    m_animation->stop();
}

void PhysicsSystem::componentComplete()
{
    m_componentComplete = true;
    initPhysics();
    findPhysicsNodes();

    g_physicsSystems->append(this);

    m_timeInt = 0;

    if (m_animation->state() == QAbstractAnimation::Running)
        m_animation->stop();
    if (m_running)
        m_animation->start();
}

PhysicsSystem *PhysicsSystem::getPhysicsSystem(QQuick3DNode *node)
{
    for (auto *physicsSystem : *g_physicsSystems) {
        if (!physicsSystem->m_scene)
            continue;

        auto *nodeCurr = node;

        if (nodeCurr == physicsSystem->m_scene)
            return physicsSystem;

        while (nodeCurr->parentNode()) {
            nodeCurr = nodeCurr->parentNode();
            if (nodeCurr == physicsSystem->m_scene)
                return physicsSystem;
        }
    }

    return nullptr;
}

void PhysicsSystem::registerPhysicsNode(AbstractPhysicsNode *physicsNode)
{
    auto *physicsSystem = getPhysicsSystem(physicsNode);
    if (physicsSystem) {
        physicsSystem->m_physicsNodes.push_back(physicsNode);
        physicsNode->init(physicsSystem->m_jolt, physicsSystem->m_tempAllocator);
    }
}

void PhysicsSystem::unRegisterPhysicsNode(AbstractPhysicsNode *physicsNode)
{
    auto *physicsSystem = getPhysicsSystem(physicsNode);
    if (physicsSystem) {
        physicsSystem->m_physicsNodes.removeAll(physicsNode);
        physicsNode->cleanup();
    }
}

PhysicsSettings *PhysicsSystem::settings() const
{
    return m_settings;
}

void PhysicsSystem::setSettings(PhysicsSettings *settings)
{
    if (m_settings == settings)
        return;

    if (m_settings)
        m_settings->disconnect(m_settingsSignalConnection);

    m_settings = settings;

    m_settingsSignalConnection = QObject::connect(m_settings, &PhysicsSettings::changed, this,
                        [this]
    {
        if (m_jolt)
            m_jolt->SetPhysicsSettings(m_settings->getJoltPhysicsSettings());
    });
    QObject::connect(m_settings, &QObject::destroyed, this,
                        [this]
    {
        m_settings = nullptr;
        if (m_jolt)
            m_jolt->SetPhysicsSettings(JPH::PhysicsSettings());
    });

    if (m_jolt)
        m_jolt->SetPhysicsSettings(m_settings->getJoltPhysicsSettings());

    emit settingsChanged(m_settings);
}

QVector3D PhysicsSystem::gravity() const
{
    return m_gravity;
}

void PhysicsSystem::setGravity(QVector3D gravity)
{
    if (m_gravity == gravity)
        return;

    m_gravity = gravity;
    if (m_physicsInitialized)
        m_jolt->SetGravity(PhysicsUtils::toJoltType(m_gravity));

    emit gravityChanged(m_gravity);
}

bool PhysicsSystem::running() const
{
    return m_running;
}

void PhysicsSystem::setRunning(bool running)
{
    if (m_running == running)
        return;

    m_running = running;
    if (m_running)
        m_animation->start();
    else
        m_animation->stop();

    emit runningChanged(m_running);
}

int PhysicsSystem::collisionSteps() const
{
    return m_collisionSteps;
}

void PhysicsSystem::setCollisionSteps(int collisionSteps)
{
    if (m_collisionSteps == collisionSteps)
        return;

    m_collisionSteps = collisionSteps;
    emit collisionStepsChanged(m_collisionSteps);
}

unsigned int PhysicsSystem::numBodies() const
{
    return m_numBodies;
}

void PhysicsSystem::setNumBodies(unsigned int maxBodies)
{
    if (m_numBodies == maxBodies)
        return;

    if (m_physicsInitialized) {
        qWarning() << "Warning: Changing 'maxBodies' after physics is initialized will have "
                      "no effect";
        return;
    }

    m_numBodies = maxBodies;

    emit numBodiesChanged(maxBodies);
}

unsigned int PhysicsSystem::numBodyMutexes() const
{
    return m_numBodyMutexes;
}

void PhysicsSystem::setNumBodyMutexes(unsigned int numBodyMutexes)
{
    if (m_numBodyMutexes == numBodyMutexes)
        return;

    if (m_physicsInitialized) {
        qWarning() << "Warning: Changing 'numBodyMutexes' after physics is initialized will have "
                      "no effect";
        return;
    }

    m_numBodyMutexes = numBodyMutexes;
    emit numBodyMutexesChanged(numBodyMutexes);
}

unsigned int PhysicsSystem::maxBodyPairs() const
{
    return m_maxBodyPairs;
}

void PhysicsSystem::setMaxBodyPairs(unsigned int maxBodyPairs)
{
    if (m_maxBodyPairs == maxBodyPairs)
        return;

    if (m_physicsInitialized) {
        qWarning() << "Warning: Changing 'maxBodyPairs' after physics is initialized will have "
                      "no effect";
        return;
    }

    m_maxBodyPairs = maxBodyPairs;
    emit maxBodyPairsChanged(maxBodyPairs);
}

unsigned int PhysicsSystem::maxContactConstraints() const
{
    return m_maxContactConstraints;
}

void PhysicsSystem::setMaxContactConstraints(unsigned int maxContactConstraints)
{
    if (m_maxContactConstraints == maxContactConstraints)
        return;

    if (m_physicsInitialized) {
        qWarning() << "Warning: Changing 'maxContactConstraints' after physics is initialized will have "
                      "no effect";
        return;
    }

    m_maxContactConstraints = maxContactConstraints;
    emit maxContactConstraintsChanged(maxContactConstraints);
}

int PhysicsSystem::numThreads() const
{
    return m_numThreads;
}

void PhysicsSystem::setNumThreads(int numThreads)
{
    if (m_numThreads == numThreads)
        return;

    if (m_physicsInitialized) {
        qWarning() << "Warning: Changing 'numThreads' after physics is initialized will have "
                      "no effect";
        return;
    }

    m_numThreads = numThreads;
    emit numThreadsChanged(numThreads);
}

AbstractObjectLayerPairFilter *PhysicsSystem::objectLayerPairFilter() const
{
    return m_objectLayerPairFilter;
}

void PhysicsSystem::setObjectLayerPairFilter(AbstractObjectLayerPairFilter *objectLayerPairFilter)
{
    if (m_objectLayerPairFilter == objectLayerPairFilter)
        return;

    if (m_physicsInitialized) {
        qWarning() << "Warning: Changing 'objectLayerPairFilter' after physics is initialized will have "
                      "no effect";
        return;
    }

    m_objectLayerPairFilter = objectLayerPairFilter;
    emit objectLayerPairFilterChanged(objectLayerPairFilter);
}

AbstractBroadPhaseLayer *PhysicsSystem::broadPhaseLayer() const
{
    return m_broadPhaseLayer;
}

void PhysicsSystem::setBroadPhaseLayer(AbstractBroadPhaseLayer *broadPhaseLayer)
{
    if (m_broadPhaseLayer == broadPhaseLayer)
        return;

    if (m_physicsInitialized) {
        qWarning() << "Warning: Changing 'broadPhaseLayer' after physics is initialized will have "
                      "no effect";
        return;
    }

    m_broadPhaseLayer = broadPhaseLayer;
    emit broadPhaseLayerChanged(broadPhaseLayer);
}

AbstractObjectVsBroadPhaseLayerFilter *PhysicsSystem::objectVsBroadPhaseLayerFilter() const
{
    return m_objectVsBroadPhaseLayerFilter;
}

void PhysicsSystem::setObjectVsBroadPhaseLayerFilter(AbstractObjectVsBroadPhaseLayerFilter *objectVsBroadPhaseLayerFilter)
{
    if (m_objectVsBroadPhaseLayerFilter == objectVsBroadPhaseLayerFilter)
        return;

    if (m_physicsInitialized) {
        qWarning() << "Warning: Changing 'objectVsBroadPhaseLayerFilter' after physics is initialized will have "
                      "no effect";
        return;
    }

    m_objectVsBroadPhaseLayerFilter = objectVsBroadPhaseLayerFilter;
    emit objectVsBroadPhaseLayerFilterChanged(objectVsBroadPhaseLayerFilter);
}

AbstractContactListener *PhysicsSystem::contactListener() const
{
    return m_contactListener;
}

void PhysicsSystem::setContactListener(AbstractContactListener *contactListener)
{
    if (m_contactListener == contactListener)
        return;

    m_contactListener = contactListener;

    QObject::connect(m_contactListener, &QObject::destroyed, this,
                     [this]
                     {
                        m_contactListener = nullptr;
                        if (m_jolt)
                            m_jolt->SetContactListener(nullptr);
                     });

    if (m_jolt)
        m_jolt->SetContactListener(m_contactListener->getJoltContactListener());

    emit contactListenerChanged(m_contactListener);
}

QQuick3DNode *PhysicsSystem::scene() const
{
    return m_scene;
}

void PhysicsSystem::setScene(QQuick3DNode *scene)
{
    if (m_scene == scene)
        return;

    m_scene = scene;

    auto physicsBodies = m_physicsNodes;
    for (auto body : std::as_const(physicsBodies))
        unRegisterPhysicsNode(body);

    m_physicsNodes.clear();

    if (m_componentComplete) {
        bool sceneOK = true;
        for (auto *physicsSystem : *g_physicsSystems) {
            if (physicsSystem != this && physicsSystem->scene() == scene) {
                sceneOK = false;
                qWarning() << "Warning: scene already associated with physics world";
            }
        }

        if (sceneOK)
            findPhysicsNodes();
    }

    emit sceneChanged(m_scene);
}

static RayCastResult getRayCastHitResult(JPH::PhysicsSystem *jolt, const JPH::RRayCast &ray, const JPH::RayCastResult &hit)
{
    Body *body = nullptr;
    JPH::BodyLockWrite lock(jolt->GetBodyLockInterface(), hit.mBodyID);
    if (lock.Succeeded()) {
        JPH::Body &joltBody = lock.GetBody();
        body = reinterpret_cast<Body *>(joltBody.GetUserData());

        return RayCastResult(
            body,
            PhysicsUtils::toQtType(ray.GetPointOnRay(hit.mFraction)),
            hit.mFraction);
    }

    return RayCastResult();
}

RayCastResult PhysicsSystem::castRay(const QVector3D &origin, const QVector3D &direction) const
{
    JPH::RRayCast ray {
        PhysicsUtils::toJoltType(origin),
        PhysicsUtils::toJoltType(direction)
    };

    JPH::RayCastResult hit;
    bool hadHit = m_jolt->GetNarrowPhaseQuery().CastRay(ray, hit);

    if (hadHit)
        return getRayCastHitResult(m_jolt, ray, hit);

    return RayCastResult();
}

RayCastResult PhysicsSystem::castRay(const QVector3D &origin, const QVector3D &direction, unsigned int broadPhaseLayerFilter, unsigned int objectLayerFilter) const
{
    JPH::RRayCast ray {
        PhysicsUtils::toJoltType(origin),
        PhysicsUtils::toJoltType(direction)
    };

    JPH::RayCastResult hit;
    bool hadHit = m_jolt->GetNarrowPhaseQuery().CastRay(
        ray, hit,
        JPH::SpecifiedBroadPhaseLayerFilter(JPH::BroadPhaseLayer(broadPhaseLayerFilter)),
        JPH::SpecifiedObjectLayerFilter(JPH::ObjectLayer(objectLayerFilter)));

    if (hadHit)
        return getRayCastHitResult(m_jolt, ray, hit);

    return RayCastResult();
}

void PhysicsSystem::initPhysics()
{
    if (m_objectLayerPairFilter == nullptr) {
        qWarning() << "Warning: Object 'objectLayerPairFilter' must be provided";
        return;
    }

    if (m_broadPhaseLayer == nullptr) {
        qWarning() << "Warning: Object 'broadPhaseLayer' must be provided";
        return;
    }

    if (m_objectVsBroadPhaseLayerFilter == nullptr) {
        qWarning() << "Warning: Object 'objectVsBroadPhaseLayerFilter must be provided";
        return;
    }

    const unsigned int numThreads = m_numThreads >= 0 ? m_numThreads : qMax(0, QThread::idealThreadCount());
    m_jobSystem = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, numThreads);

    m_jolt = new JPH::PhysicsSystem();

    m_jolt->Init(m_numBodies,
                 m_numBodyMutexes,
                 m_maxBodyPairs,
                 m_maxContactConstraints,
                 m_broadPhaseLayer->impl(),
                 m_objectVsBroadPhaseLayerFilter->impl(),
                 m_objectLayerPairFilter->impl());

    if (m_settings)
        m_jolt->SetPhysicsSettings(m_settings->getJoltPhysicsSettings());

    if (m_contactListener)
        m_jolt->SetContactListener(m_contactListener->getJoltContactListener());

    m_jolt->SetGravity(PhysicsUtils::toJoltType(m_gravity));

    m_physicsInitialized = true;
}

void PhysicsSystem::findPhysicsNodes()
{
    if (m_scene == nullptr)
        return;

    QList<QQuick3DObject *> children = m_scene->childItems();
    while (!children.empty()) {
        auto child = children.takeFirst();
        if (auto converted = qobject_cast<AbstractPhysicsNode *>(child); converted != nullptr) {
            m_physicsNodes.push_back(converted);
            converted->init(m_jolt, m_tempAllocator);
        }
        children.append(child->childItems());
    }
}

void PhysicsSystem::updateCurrentTime(int currentTime)
{
    if (!m_physicsInitialized)
        return;

    qreal dt = m_timeInt / 1000.;
    m_timeInt = currentTime;
    qreal time =  m_timeInt / 1000.;
    dt = time - dt;

    emit beforeFrameDone(dt * 1000);

    m_jolt->Update(dt, m_collisionSteps, m_tempAllocator, m_jobSystem);

    for (auto physicsNode : std::as_const(m_physicsNodes))
        physicsNode->sync();

    emit frameDone(dt * 1000);
}
