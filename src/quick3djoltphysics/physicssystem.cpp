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
#include <Jolt/Physics/Collision/CollisionCollectorImpl.h>
#include <Jolt/Physics/Collision/CollidePointResult.h>
#include <Jolt/Physics/Collision/NarrowPhaseQuery.h>
#include <Jolt/Physics/Body/BodyLock.h>
#include <Jolt/Physics/Collision/CollideShape.h>
#include <Jolt/Physics/Collision/CollisionCollectorImpl.h>

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
    for (auto *physicsNode : std::as_const(m_physicsNodes))
        physicsNode->cleanup();

    m_physicsNodes.clear();

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
    for (auto *physicsSystem : *g_physicsSystems) {
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

    m_timeInt = 0;

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

    auto physicsNodes = m_physicsNodes;
    for (auto node : std::as_const(physicsNodes))
        unRegisterPhysicsNode(node);

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

static RayCastResult getRayCastHitResult(JPH::PhysicsSystem *jolt, const JPH::RRayCast &ray, const JPH::RayCastResult &hit, bool hadHit)
{
    Body *body = nullptr;
    JPH::Vec3 position = ray.GetPointOnRay(hit.mFraction);
    JPH::Vec3 normal;

    if (hadHit) {
        JPH::BodyLockWrite lock(jolt->GetBodyLockInterface(), hit.mBodyID);
        if (lock.Succeeded()) {
            const auto &joltBody = lock.GetBody();
            body = reinterpret_cast<Body *>(joltBody.GetUserData());
            normal = joltBody.GetWorldSpaceSurfaceNormal(hit.mSubShapeID2, position);
        }
    }

    return RayCastResult(
        body,
        PhysicsUtils::toQtType(position),
        PhysicsUtils::toQtType(normal),
        hit.mFraction);
}

RayCastResult PhysicsSystem::castRay(const QVector3D &origin, const QVector3D &direction) const
{
    JPH::RRayCast ray {
        PhysicsUtils::toJoltType(origin),
        PhysicsUtils::toJoltType(direction)
    };

    JPH::RayCastResult hit;
    bool hadHit = m_jolt->GetNarrowPhaseQuery().CastRay(ray, hit);

    return getRayCastHitResult(m_jolt, ray, hit, hadHit);
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

    return getRayCastHitResult(m_jolt, ray, hit, hadHit);
}

static QVector<Body *> getCollidePointResult(JPH::PhysicsSystem *jolt, const JPH::AllHitCollisionCollector<JPH::CollidePointCollector> &collector)
{
    QVector<Body *> hits;

    for (const auto &hit : collector.mHits) {
        JPH::BodyLockRead lock(jolt->GetBodyLockInterface(), hit.mBodyID);
        if (lock.Succeeded()) {
            const auto &joltBody = lock.GetBody();
            Body *body = reinterpret_cast<Body *>(joltBody.GetUserData());
            hits.push_back(body);
        }
    }

    return hits;
}

QVector<Body *> PhysicsSystem::collidePoint(const QVector3D &point) const
{
    JPH::AllHitCollisionCollector<JPH::CollidePointCollector> collector;
    m_jolt->GetNarrowPhaseQuery().CollidePoint(PhysicsUtils::toJoltType(point), collector);

    return getCollidePointResult(m_jolt, collector);
}

QVector<Body *> PhysicsSystem::collidePoint(const QVector3D &point, unsigned int broadPhaseLayerFilter, unsigned int objectLayerFilter) const
{
    JPH::AllHitCollisionCollector<JPH::CollidePointCollector> collector;
    m_jolt->GetNarrowPhaseQuery().CollidePoint(PhysicsUtils::toJoltType(point), collector,
                                               JPH::SpecifiedBroadPhaseLayerFilter(JPH::BroadPhaseLayer(broadPhaseLayerFilter)),
                                               JPH::SpecifiedObjectLayerFilter(JPH::ObjectLayer(objectLayerFilter)));

    return getCollidePointResult(m_jolt, collector);
}

static QVector<CollideShapeResult> getCollideShapeResult(JPH::PhysicsSystem *jolt, const JPH::AllHitCollisionCollector<JPH::CollideShapeCollector> &collector)
{
    QVector<CollideShapeResult> hits;

    for (const auto &hit : collector.mHits) {
        JPH::BodyLockRead lock(jolt->GetBodyLockInterface(), hit.mBodyID2);
        if (lock.Succeeded()) {
            const auto &joltBody = lock.GetBody();
            Body *body = reinterpret_cast<Body *>(joltBody.GetUserData());
            const auto &surfaceNormal = joltBody.GetWorldSpaceSurfaceNormal(hit.mSubShapeID2, hit.mContactPointOn2);
            hits.push_back(CollideShapeResult(PhysicsUtils::toQtType(hit.mContactPointOn1),
                                              PhysicsUtils::toQtType(hit.mContactPointOn2),
                                              PhysicsUtils::toQtType(hit.mPenetrationAxis),
                                              hit.mPenetrationDepth,
                                              PhysicsUtils::toQtType(surfaceNormal),
                                              body)
            );
        }
    }

    return hits;
}

QVector<CollideShapeResult> PhysicsSystem::collideShape(AbstractShape *shape, const QMatrix4x4 &transform, CollideShapeSettings *settings, const QVector3D &baseOffset) const
{
    JPH::AllHitCollisionCollector<JPH::CollideShapeCollector> collector;

    JPH::CollideShapeSettings inSettings;
    inSettings.mMaxSeparationDistance = settings->maxSeparationDistance;
    inSettings.mBackFaceMode = static_cast<JPH::EBackFaceMode>(settings->backFaceMode);
    inSettings.mActiveEdgeMode = static_cast<JPH::EActiveEdgeMode>(settings->activeEdgeMode);
    inSettings.mCollectFacesMode = static_cast<JPH::ECollectFacesMode>(settings->collectFacesMode);
    inSettings.mCollisionTolerance = settings->collisionTolerance;
    inSettings.mPenetrationTolerance = settings->penetrationTolerance;
    inSettings.mActiveEdgeMovementDirection = PhysicsUtils::toJoltType(settings->activeEdgeMovementDirection);

    m_jolt->GetNarrowPhaseQuery().CollideShape(shape->getJoltShape(), JPH::Vec3::sReplicate(1.0f), PhysicsUtils::toJoltType(transform), inSettings, PhysicsUtils::toJoltType(baseOffset), collector);

    collector.Sort();

    return getCollideShapeResult(m_jolt, collector);
}

QVector<CollideShapeResult> PhysicsSystem::collideShape(AbstractShape *shape, const QMatrix4x4 &transform, CollideShapeSettings *settings, const QVector3D &baseOffset, unsigned int broadPhaseLayerFilter, unsigned int objectLayerFilter) const
{
    JPH::AllHitCollisionCollector<JPH::CollideShapeCollector> collector;

    JPH::CollideShapeSettings inSettings;
    inSettings.mMaxSeparationDistance = settings->maxSeparationDistance;
    inSettings.mBackFaceMode = static_cast<JPH::EBackFaceMode>(settings->backFaceMode);

    m_jolt->GetNarrowPhaseQuery().CollideShape(shape->getJoltShape(),
                                               JPH::Vec3::sReplicate(1.0f),
                                               PhysicsUtils::toJoltType(transform),
                                               inSettings,
                                               PhysicsUtils::toJoltType(baseOffset), collector,
                                               JPH::SpecifiedBroadPhaseLayerFilter(JPH::BroadPhaseLayer(broadPhaseLayerFilter)),
                                               JPH::SpecifiedObjectLayerFilter(JPH::ObjectLayer(objectLayerFilter)));

    collector.Sort();

    return getCollideShapeResult(m_jolt, collector);
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

    int dt = m_timeInt;
    m_timeInt = currentTime;
    int time =  m_timeInt;
    dt = time - dt;

    float deltaTime = dt / 1000.0f;

    emit beforeFrameDone(deltaTime);

    m_jolt->Update(deltaTime, m_collisionSteps, m_tempAllocator, m_jobSystem);

    for (auto physicsNode : std::as_const(m_physicsNodes))
        physicsNode->sync();

    emitContactCallbacks();

    emit frameDone(deltaTime);
}

void PhysicsSystem::emitContactCallbacks()
{
    if (m_contactListener == nullptr)
        return;

    auto bodyContacts = m_contactListener->takeBodyContacts();
    for (const auto &bodyContact : std::as_const(bodyContacts)) {
        Body *qtBody1 = nullptr;
        {
            JPH::BodyLockRead bodyLock1(m_jolt->GetBodyLockInterface(), JPH::BodyID(bodyContact.bodyID1));
            if (bodyLock1.Succeeded()) {
                const JPH::Body &body = bodyLock1.GetBody();
                qtBody1 = reinterpret_cast<Body *>(body.GetUserData());
            }
        }

        Body *qtBody2 = nullptr;
        {
            JPH::BodyLockRead bodyLock2(m_jolt->GetBodyLockInterface(), JPH::BodyID(bodyContact.bodyID2));
            if (bodyLock2.Succeeded()) {
                const JPH::Body &body = bodyLock2.GetBody();
                qtBody2 = reinterpret_cast<Body *>(body.GetUserData());
            }
        }

        if (qtBody1 && qtBody2) {
            emit qtBody1->bodyContact(qtBody2);
            emit qtBody2->bodyContact(qtBody1);
        }
    }

    bodyContacts = m_contactListener->takeEnteredBodyContacts();
    for (const auto &bodyContact : std::as_const(bodyContacts)) {
        Body *qtBody1 = nullptr;
        {
            JPH::BodyLockRead bodyLock1(m_jolt->GetBodyLockInterface(), JPH::BodyID(bodyContact.bodyID1));
            if (bodyLock1.Succeeded()) {
                const JPH::Body &body = bodyLock1.GetBody();
                qtBody1 = reinterpret_cast<Body *>(body.GetUserData());
            }
        }

        Body *qtBody2 = nullptr;
        {
            JPH::BodyLockRead bodyLock2(m_jolt->GetBodyLockInterface(), JPH::BodyID(bodyContact.bodyID2));
            if (bodyLock2.Succeeded()) {
                const JPH::Body &body = bodyLock2.GetBody();
                qtBody2 = reinterpret_cast<Body *>(body.GetUserData());
            }
        }

        if (qtBody1 && qtBody2) {
            emit qtBody1->bodyEntered(qtBody2);
            emit qtBody2->bodyEntered(qtBody1);
        }
    }

    bodyContacts = m_contactListener->takeExitedBodyContacts();
    for (const auto &bodyContact : std::as_const(bodyContacts)) {
        Body *qtBody1 = nullptr;
        {
            JPH::BodyLockRead bodyLock1(m_jolt->GetBodyLockInterface(), JPH::BodyID(bodyContact.bodyID1));
            if (bodyLock1.Succeeded()) {
                const JPH::Body &body = bodyLock1.GetBody();
                qtBody1 = reinterpret_cast<Body *>(body.GetUserData());
            }
        }

        Body *qtBody2 = nullptr;
        {
            JPH::BodyLockRead bodyLock2(m_jolt->GetBodyLockInterface(), JPH::BodyID(bodyContact.bodyID2));
            if (bodyLock2.Succeeded()) {
                const JPH::Body &body = bodyLock2.GetBody();
                qtBody2 = reinterpret_cast<Body *>(body.GetUserData());
            }
        }

        if (qtBody1 && qtBody2) {
            emit qtBody1->bodyExited(qtBody2);
            emit qtBody2->bodyExited(qtBody1);
        }
    }
}
