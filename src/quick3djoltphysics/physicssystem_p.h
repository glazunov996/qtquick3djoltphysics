#ifndef PHYSICSSYSTEM_P_H
#define PHYSICSSYSTEM_P_H

#include "physicslayers.h"
#include "physicssettings_p.h"
#include "raycastresult_p.h"
#include "collideshaperesult_p.h"
#include "contactlistener.h"
#include "qtquick3djoltphysicsglobal_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>

#include <QLoggingCategory>
#include <QObject>
#include <QPointer>
#include <QtQml/qqml.h>
#include <QAbstractAnimation>

#include <QtQuick3D/private/qquick3dviewport_p.h>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSettings.h>

namespace JPH {
class PhysicsSystem;
class TempAllocator;
class JobSystem;
class Body;
}

Q_DECLARE_LOGGING_CATEGORY(lcQuick3dJoltPhysics)

class AbstractPhysicsNode;
class PhysicsSystemAnimation;

class Q_QUICK3DJOLTPHYSICS_EXPORT CollideShapeSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float maxSeparationDistance MEMBER maxSeparationDistance)
    Q_PROPERTY(Quick3dJoltPhysics::BackFaceMode backFaceMode MEMBER backFaceMode)
    Q_PROPERTY(Quick3dJoltPhysics::ActiveEdgeMode activeEdgeMode MEMBER activeEdgeMode)
    Q_PROPERTY(Quick3dJoltPhysics::CollectFacesMode collectFacesMode MEMBER collectFacesMode)
    Q_PROPERTY(float collisionTolerance MEMBER collisionTolerance)
    Q_PROPERTY(float penetrationTolerance MEMBER penetrationTolerance)
    Q_PROPERTY(QVector3D activeEdgeMovementDirection MEMBER activeEdgeMovementDirection)
    QML_NAMED_ELEMENT(CollideShapeSettings)
public:
    explicit CollideShapeSettings(QObject *parent = nullptr) : QObject(parent)
    {}
    ~CollideShapeSettings() override = default;

    float maxSeparationDistance = 0.0f;
    Quick3dJoltPhysics::BackFaceMode backFaceMode = Quick3dJoltPhysics::BackFaceMode::IgnoreBackFaces;
    Quick3dJoltPhysics::ActiveEdgeMode activeEdgeMode = Quick3dJoltPhysics::ActiveEdgeMode::CollideOnlyWithActive;
    Quick3dJoltPhysics::CollectFacesMode collectFacesMode = Quick3dJoltPhysics::CollectFacesMode::NoFaces;
    float collisionTolerance = JPH::cDefaultCollisionTolerance;
    float penetrationTolerance = JPH::cDefaultPenetrationTolerance;
    QVector3D activeEdgeMovementDirection;
};

class Q_QUICK3DJOLTPHYSICS_EXPORT PhysicsSystem : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(PhysicsSettings *settings READ settings WRITE setSettings NOTIFY settingsChanged)
    Q_PROPERTY(QVector3D gravity READ gravity WRITE setGravity NOTIFY gravityChanged)
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(int collisionSteps READ collisionSteps WRITE setCollisionSteps NOTIFY collisionStepsChanged)
    Q_PROPERTY(unsigned int numBodies READ numBodies WRITE setNumBodies NOTIFY numBodiesChanged)
    Q_PROPERTY(unsigned int numBodyMutexes READ numBodyMutexes WRITE setNumBodyMutexes NOTIFY numBodyMutexesChanged)
    Q_PROPERTY(unsigned int maxBodyPairs READ maxBodyPairs WRITE setMaxBodyPairs NOTIFY maxBodyPairsChanged)
    Q_PROPERTY(unsigned int maxContactConstraints READ maxContactConstraints WRITE setMaxContactConstraints NOTIFY maxContactConstraintsChanged)
    Q_PROPERTY(int numThreads READ numThreads WRITE setNumThreads NOTIFY numThreadsChanged)
    Q_PROPERTY(AbstractObjectLayerPairFilter *objectLayerPairFilter READ objectLayerPairFilter WRITE
                   setObjectLayerPairFilter NOTIFY objectLayerPairFilterChanged)
    Q_PROPERTY(AbstractBroadPhaseLayer *broadPhaseLayer READ broadPhaseLayer WRITE
                   setBroadPhaseLayer NOTIFY broadPhaseLayerChanged)
    Q_PROPERTY(AbstractObjectVsBroadPhaseLayerFilter *objectVsBroadPhaseLayerFilter READ objectVsBroadPhaseLayerFilter WRITE
                   setObjectVsBroadPhaseLayerFilter NOTIFY objectVsBroadPhaseLayerFilterChanged)
    Q_PROPERTY(AbstractContactListener *contactListener READ contactListener WRITE setContactListener NOTIFY contactListenerChanged)
    Q_PROPERTY(QQuick3DNode *scene READ scene WRITE setScene NOTIFY sceneChanged)

    QML_NAMED_ELEMENT(PhysicsSystem)
public:
    PhysicsSystem(QObject *parent = nullptr);
    ~PhysicsSystem();

    void classBegin() override {}
    void componentComplete() override;

    static PhysicsSystem *getPhysicsSystem(QQuick3DNode *node);

    static void registerPhysicsNode(AbstractPhysicsNode *physicsNode);
    static void unRegisterPhysicsNode(AbstractPhysicsNode *physicsNode);

    PhysicsSettings *settings() const;
    void setSettings(PhysicsSettings *settings);
    QVector3D gravity() const;
    void setGravity(QVector3D gravity);
    bool running() const;
    void setRunning(bool running);
    int collisionSteps() const;
    void setCollisionSteps(int collisionSteps);
    unsigned int numBodies() const;
    void setNumBodies(unsigned int maxBodies);
    unsigned int numBodyMutexes() const;
    void setNumBodyMutexes(unsigned int numBodyMutexes);
    unsigned int maxBodyPairs() const;
    void setMaxBodyPairs(unsigned int maxBodyPairs);
    unsigned int maxContactConstraints() const;
    void setMaxContactConstraints(unsigned int maxContactConstraints);
    int numThreads() const;
    void setNumThreads(int numThreads);
    AbstractObjectLayerPairFilter *objectLayerPairFilter() const;
    void setObjectLayerPairFilter(AbstractObjectLayerPairFilter *objectLayerPairFilter);
    AbstractBroadPhaseLayer *broadPhaseLayer() const;
    void setBroadPhaseLayer(AbstractBroadPhaseLayer *broadPhaseLayer);
    AbstractObjectVsBroadPhaseLayerFilter *objectVsBroadPhaseLayerFilter() const;
    void setObjectVsBroadPhaseLayerFilter(AbstractObjectVsBroadPhaseLayerFilter *objectVsBroadPhaseLayerFilter);
    AbstractContactListener *contactListener() const;
    void setContactListener(AbstractContactListener *contactListener);
    QQuick3DNode *scene() const;
    void setScene(QQuick3DNode *scene);

    Q_INVOKABLE RayCastResult castRay(const QVector3D &origin, const QVector3D &direction) const;
    Q_INVOKABLE RayCastResult castRay(const QVector3D &origin, const QVector3D &direction, unsigned int broadPhaseLayerFilter, unsigned int objectLayerFilter) const;
    Q_INVOKABLE QVector<Body *> collidePoint(const QVector3D &point) const;
    Q_INVOKABLE QVector<Body *> collidePoint(const QVector3D &point, unsigned int broadPhaseLayerFilter, unsigned int objectLayerFilter) const;
    Q_INVOKABLE QVector<CollideShapeResult> collideShape(AbstractShape *shape, const QMatrix4x4 &transform, CollideShapeSettings *settings, const QVector3D &baseOffset) const;
    Q_INVOKABLE QVector<CollideShapeResult> collideShape(AbstractShape *shape, const QMatrix4x4 &transform, CollideShapeSettings *settings, const QVector3D &baseOffset, unsigned int broadPhaseLayerFilter, unsigned int objectLayerFilter) const;

signals:
    void settingsChanged(PhysicsSettings *settings);
    void gravityChanged(QVector3D gravity);
    void runningChanged(bool running);
    void collisionStepsChanged(int collisionSteps);
    void numBodiesChanged(unsigned int maxBodies);
    void numBodyMutexesChanged(unsigned int numBodyMutexes);
    void maxBodyPairsChanged(unsigned int maxBodyPairs);
    void maxContactConstraintsChanged(unsigned int maxContactConstraints);
    void numThreadsChanged(int numThreads);
    void objectLayerPairFilterChanged(AbstractObjectLayerPairFilter *objectLayerPairFilter);
    void broadPhaseLayerChanged(AbstractBroadPhaseLayer *broadPhaseLayer);
    void objectVsBroadPhaseLayerFilterChanged(AbstractObjectVsBroadPhaseLayerFilter *objectVsBroadPhaseLayerFilter);
    void contactListenerChanged(AbstractContactListener *contactListener);
    void sceneChanged(QQuick3DNode *scene);
    void updateFrame(float frequency, int collisionSteps);
    void beforeFrameDone(float deltaTime);
    void frameDone(float deltaTime);

private:
    void initPhysics();
    void findPhysicsNodes();
    void updateCurrentTime(int currentTime);
    void emitContactCallbacks();

    friend class PhysicsSystemAnimation;

    QList<AbstractPhysicsNode *> m_physicsNodes;

    PhysicsSettings *m_settings = nullptr;
    QMetaObject::Connection m_settingsSignalConnection;
    QVector3D m_gravity = QVector3D(0.f, -981.f, 0.f);
    bool m_running = true;
    int m_collisionSteps = 1;
    unsigned int m_numBodies = 10240;
    unsigned int m_numBodyMutexes = 0;
    unsigned int m_maxBodyPairs = 65536;
    unsigned int m_maxContactConstraints = 20480;
    int m_numThreads = -1;
    QQuick3DNode *m_scene = nullptr;

    int m_timeInt = 0;

    JPH::PhysicsSystem *m_jolt = nullptr;
    JPH::TempAllocator *m_tempAllocator = nullptr;
    JPH::JobSystem *m_jobSystem = nullptr;
    JPH::JobSystem *m_jobSystemValidating = nullptr;
    AbstractObjectLayerPairFilter *m_objectLayerPairFilter = nullptr;
    AbstractBroadPhaseLayer *m_broadPhaseLayer = nullptr;
    AbstractObjectVsBroadPhaseLayerFilter *m_objectVsBroadPhaseLayerFilter = nullptr;
    AbstractContactListener *m_contactListener = nullptr;

    PhysicsSystemAnimation *m_animation = nullptr;
    bool m_componentComplete = false;
    bool m_physicsInitialized = false;
};

class PhysicsSystemAnimation : public QAbstractAnimation
{
    Q_OBJECT
public:
    PhysicsSystemAnimation(PhysicsSystem *system)
        : QAbstractAnimation(static_cast<QObject *>(system)), m_system(system)
    {}
protected:
    void updateCurrentTime(int t) override
    {
        m_system->updateCurrentTime(t);
    }

    int duration() const override
    {
        return -1;
    }

private:
    PhysicsSystem *m_system = nullptr;
};

#endif // PHYSICSSYSTEM_P_H
