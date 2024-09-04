#include "body_p.h"
#include "physicssystem_p.h"
#include "physicsutils_p.h"

#include <Jolt/Physics/Collision/Shape/Shape.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Core/Memory.h>

Body::Body(QQuick3DNode *parent) : AbstractPhysicsBody(parent)
{
    connect(this, &QQuick3DNode::scenePositionChanged, this, &Body::handleScenePositionChanged);
    connect(this, &QQuick3DNode::sceneRotationChanged, this, &Body::handleSceneRotationChanged);
}

Body::~Body()
{
    Body::cleanup();
}

int Body::bodyID() const
{
    return m_bodyID;
}

CollisionGroup *Body::collisionGroup() const
{
    return m_collisionGroup;
}

void Body::setCollisionGroup(CollisionGroup *collisionGroup)
{
    if (m_collisionGroup == collisionGroup)
        return;

    if (m_collisionGroup != nullptr)
        m_collisionGroup->disconnect(m_collisionGroupSignalConnection);

    m_collisionGroup = collisionGroup;

    m_collisionGroupSignalConnection = QObject::connect(m_collisionGroup, &CollisionGroup::changed, this,
                    [this]
    {
        if (m_body)
            m_body->SetCollisionGroup(m_collisionGroup->getJoltCollisionGroup());
    });
    QObject::connect(m_collisionGroup, &QObject::destroyed, this,
                    [this]
    {
        m_collisionGroup = nullptr;
        if (m_body)
            m_body->SetCollisionGroup(JPH::CollisionGroup());
    });

    if (m_body)
        m_body->SetCollisionGroup(m_collisionGroup->getJoltCollisionGroup());

    emit collisionGroupChanged(m_collisionGroup);
}

Body::MotionType Body::motionType() const
{
    return m_motionType;
}

void Body::setMotionType(MotionType motionType)
{
    if (m_motionType == motionType)
        return;

    m_motionType = motionType;
    if (m_body) {
        auto &bodyInterface = m_jolt->GetBodyInterface();
        const auto activationMode = getActivationForMotionType();
        bodyInterface.SetMotionType(m_body->GetID(), static_cast<JPH::EMotionType>(m_motionType), activationMode);
    }

    emit motionTypeChanged(motionType);
}

Body::MotionQuality Body::motionQuality() const
{
    return m_motionQuality;
}

void Body::setMotionQuality(MotionQuality motionQuality)
{
    if (m_motionQuality == motionQuality)
        return;

    m_motionQuality = motionQuality;
    if (m_body) {
        auto &bodyInterface = m_jolt->GetBodyInterface();
        bodyInterface.SetMotionQuality(m_body->GetID(), static_cast<JPH::EMotionQuality>(m_motionQuality));
    }

    emit motionQualityChanged(motionQuality);
}

unsigned int Body::objectLayer() const
{
    return m_bodySettings.mObjectLayer;
}

void Body::setObjectLayer(unsigned int objectLayer)
{
    if (m_bodySettings.mObjectLayer == objectLayer)
        return;

    m_bodySettings.mObjectLayer = objectLayer;
    if (m_body) {
        auto &bodyInterface = m_jolt->GetBodyInterface();
        bodyInterface.SetObjectLayer(m_body->GetID(), m_bodySettings.mObjectLayer);
    }

    emit objectLayerChanged(objectLayer);
}

bool Body::usedInSimulation() const
{
    return m_usedInSimulation;
}

void Body::setUsedInSimulation(bool usedInSimulation)
{
    if (m_usedInSimulation == usedInSimulation)
        return;

    m_usedInSimulation = usedInSimulation;
    if (m_body) {
        auto &bodyInterface = m_jolt->GetBodyInterface();
        if (m_usedInSimulation) {
            const auto activationMode = getActivationForMotionType();
            bodyInterface.AddBody(m_body->GetID(), activationMode);
        } else {
            bodyInterface.RemoveBody(m_body->GetID());
        }
    }

    emit usedInSimulationChanged(usedInSimulation);
}

bool Body::allowSleeping() const
{
    return m_bodySettings.mAllowSleeping;
}

void Body::setAllowSleeping(bool allowSleeping)
{
    if (m_bodySettings.mAllowSleeping == allowSleeping)
        return;

    m_bodySettings.mAllowSleeping = allowSleeping;
    if (m_body)
        m_body->SetAllowSleeping(m_bodySettings.mAllowSleeping);

    emit allowSleepingChanged(allowSleeping);
}

Body::AllowedDOFs Body::allowedDOFs() const
{
    return m_allowedDOFs;
}

void Body::setAllowedDOFs(AllowedDOFs allowedDOFs)
{
    if (m_allowedDOFs == allowedDOFs)
        return;

    m_allowedDOFs = allowedDOFs;
    if (m_body) {
        const auto &massProperties = m_shape->getJoltShape()->GetMassProperties();
        m_body->GetMotionProperties()->SetMassProperties(static_cast<JPH::EAllowedDOFs>(m_allowedDOFs), massProperties);
    }

    emit allowedDOFsChanged(allowedDOFs);
}

Body::OverrideMassProperties Body::overrideMassProperties() const
{
    return static_cast<OverrideMassProperties>(m_bodySettings.mOverrideMassProperties);
}

void Body::setOverrideMassProperties(OverrideMassProperties overrideMassProperties)
{
    if (m_overrideMassProperties == overrideMassProperties)
        return;

    if (m_body) {
        qWarning() << "Warning: Changing 'overrideMassProperties' after body is initialized will have "
                      "no effect";
        return;
    }

    m_overrideMassProperties = overrideMassProperties;

    emit overrideMassPropertiesChanged(m_overrideMassProperties);
}

float Body::mass() const
{
    return m_mass;
}

void Body::setMass(float mass)
{
    if (qFuzzyCompare(m_mass, mass))
        return;

    m_mass = mass;
    if (m_body) {
        auto massProperties = m_shape->getJoltShape()->GetMassProperties();
        massProperties.mMass = m_mass;
        m_body->GetMotionProperties()->SetMassProperties(static_cast<JPH::EAllowedDOFs>(m_allowedDOFs), massProperties);
    }

    emit massChanged(m_mass);
}

QMatrix4x4 Body::inertia() const
{
    return m_inertia;
}

void Body::setInertia(const QMatrix4x4 &inertia)
{
    if (m_inertia == inertia)
        return;

    m_inertia = inertia;
    if (m_body) {
        auto massProperties = m_shape->getJoltShape()->GetMassProperties();
        massProperties.mInertia = PhysicsUtils::toJoltType(m_inertia);
        m_body->GetMotionProperties()->SetMassProperties(static_cast<JPH::EAllowedDOFs>(m_allowedDOFs), massProperties);
    }

    emit inertiaChanged(m_inertia);
}

float Body::friction() const
{
    return m_bodySettings.mFriction;
}

void Body::setFriction(float friction)
{
    if (qFuzzyCompare(m_bodySettings.mFriction, friction))
        return;

    m_bodySettings.mFriction = friction;
    if (m_body)
        m_body->SetFriction(m_bodySettings.mFriction);

    emit frictionChanged(friction);
}

float Body::restitution() const
{
    return m_bodySettings.mRestitution;
}

void Body::setRestitution(float restitution)
{
    if (qFuzzyCompare(m_bodySettings.mRestitution, restitution))
        return;

    m_bodySettings.mRestitution = restitution;
    if (m_body)
        m_body->SetRestitution(m_bodySettings.mRestitution);

    emit restitutionChanged(restitution);
}

float Body::linearDamping() const
{
    return m_bodySettings.mLinearDamping;
}

void Body::setLinearDamping(float linearDamping)
{
    if (qFuzzyCompare(m_bodySettings.mLinearDamping, linearDamping))
        return;

    m_bodySettings.mLinearDamping = linearDamping;
    if (m_body)
        m_body->GetMotionProperties()->SetLinearDamping(m_bodySettings.mLinearDamping);

    emit linearDampingChanged(linearDamping);
}

float Body::angularDamping() const
{
    return m_bodySettings.mAngularDamping;
}

void Body::setAngularDamping(float angularDamping)
{
    if (qFuzzyCompare(m_bodySettings.mAngularDamping, angularDamping))
        return;

    m_bodySettings.mAngularDamping = angularDamping;
    if (m_body)
        m_body->GetMotionProperties()->SetAngularDamping(m_bodySettings.mAngularDamping);

    emit angularDampingChanged(angularDamping);
}

float Body::gravityFactor() const
{
    return m_bodySettings.mGravityFactor;
}

void Body::setGravityFactor(float gravityFactor)
{
    if (qFuzzyCompare(m_bodySettings.mGravityFactor, gravityFactor))
        return;

    m_bodySettings.mGravityFactor = gravityFactor;
    if (m_body)
        m_body->GetMotionProperties()->SetGravityFactor(m_bodySettings.mGravityFactor);

    emit gravityFactorChanged(gravityFactor);
}

QVector3D Body::getLinearVelocity() const
{
    if (m_body == nullptr)
        return QVector3D();

    return PhysicsUtils::toQtType(m_body->GetLinearVelocity());
}

void Body::setLinearVelocity(const QVector3D &linearVelocity)
{
    if (m_body == nullptr) {
        qWarning() << "Warning: Invoking 'setLinearVelocity' before body is initialized will have no effect";
        return;
    }

    m_body->SetLinearVelocity(PhysicsUtils::toJoltType(linearVelocity));
}

QVector3D Body::getAngularVelocity() const
{
    if (m_body == nullptr)
        return QVector3D();

    return PhysicsUtils::toQtType(m_body->GetAngularVelocity());
}

void Body::setAngularVelocity(const QVector3D &angularVelocity)
{
    if (m_body == nullptr) {
        qWarning() << "Warning: Invoking 'setAngularVelocity' before body is initialized will have no effect";
        return;
    }

    m_body->SetAngularVelocity(PhysicsUtils::toJoltType(angularVelocity));
}

QVector3D Body::getAccumulatedForce() const
{
    if (m_body == nullptr)
        return QVector3D();

    return PhysicsUtils::toQtType(m_body->GetAccumulatedForce());
}

QVector3D Body::getAccumulatedTorque() const
{
    if (m_body == nullptr)
        return QVector3D();

    return PhysicsUtils::toQtType(m_body->GetAccumulatedTorque());
}

float Body::getInverseMass() const
{
    if (m_body == nullptr)
        return 0.0f;

    return m_body->GetMotionProperties()->GetInverseMass();
}

QVector3D Body::getCenterOfMassPosition() const
{
    if (m_body == nullptr)
        return QVector3D();

    return PhysicsUtils::toQtType(m_body->GetCenterOfMassPosition());
}

void Body::activate()
{
    if (m_body == nullptr) {
        qWarning() << "Warning: Invoking 'activate' before body is initialized will have no effect";
        return;
    }

    auto &bodyInterface = m_jolt->GetBodyInterface();
    bodyInterface.ActivateBody(m_body->GetID());
}

void Body::deactivate()
{
    if (m_body == nullptr) {
        qWarning() << "Warning: Invoking 'deactivate' before body is initialized will have no effect";
        return;
    }

    auto &bodyInterface = m_jolt->GetBodyInterface();
    bodyInterface.DeactivateBody(m_body->GetID());
}

void Body::addForce(const QVector3D &force)
{
    if (m_body == nullptr) {
        qWarning() << "Warning: Invoking 'addForce' before body is initialized will have no effect";
        return;
    }

    auto &bodyInterface = m_jolt->GetBodyInterface();
    bodyInterface.AddForce(m_body->GetID(), PhysicsUtils::toJoltType(force));
}

void Body::addForce(const QVector3D &force, const QVector3D &position)
{
    if (m_body == nullptr) {
        qWarning() << "Warning: Invoking 'addForce' before body is initialized will have no effect";
        return;
    }

    auto &bodyInterface = m_jolt->GetBodyInterface();
    bodyInterface.AddForce(m_body->GetID(), PhysicsUtils::toJoltType(force), PhysicsUtils::toJoltType(position));
}

void Body::addTorque(const QVector3D &torque)
{
    if (m_body == nullptr) {
        qWarning() << "Warning: Invoking 'addTorque' before body is initialized will have no effect";
        return;
    }

    auto &bodyInterface = m_jolt->GetBodyInterface();
    bodyInterface.AddTorque(m_body->GetID(), PhysicsUtils::toJoltType(torque));
}

void Body::addImpulse(const QVector3D &impulse)
{
    if (m_body == nullptr) {
        qWarning() << "Warning: Invoking 'addImpulse' before body is initialized will have no effect";
        return;
    }

    auto &bodyInterface = m_jolt->GetBodyInterface();
    bodyInterface.AddImpulse(m_body->GetID(), PhysicsUtils::toJoltType(impulse));
}

void Body::addImpulse(const QVector3D &impulse, const QVector3D &position)
{
    if (m_body == nullptr) {
        qWarning() << "Warning: Invoking 'addImpulse' before body is initialized will have no effect";
        return;
    }

    auto &bodyInterface = m_jolt->GetBodyInterface();
    bodyInterface.AddImpulse(m_body->GetID(), PhysicsUtils::toJoltType(impulse), PhysicsUtils::toJoltType(position));
}

void Body::addAngularImpulse(const QVector3D &angularImpulse)
{
    if (m_body == nullptr) {
        qWarning() << "Warning: Invoking 'addAngularImpulse' before body is initialized will have no effect";
        return;
    }

    auto &bodyInterface = m_jolt->GetBodyInterface();
    bodyInterface.AddAngularImpulse(m_body->GetID(), PhysicsUtils::toJoltType(angularImpulse));
}

void Body::moveKinematic(const QVector3D &targetPosition, const QQuaternion &targetRotation, float deltaTime)
{
    if (m_body == nullptr) {
        qWarning() << "Warning: Invoking 'moveKinematic' before body is initialized will have no effect";
        return;
    }

    auto &bodyInterface = m_jolt->GetBodyInterface();
    bodyInterface.MoveKinematic(m_body->GetID(), PhysicsUtils::toJoltType(targetPosition), PhysicsUtils::toJoltType(targetRotation), deltaTime);
}

void Body::updateJoltObject()
{
    if (m_jolt == nullptr || m_shape == nullptr)
        return;

    auto &bodyInterface = m_jolt->GetBodyInterface();
    const auto activationMode = getActivationForMotionType();

    if (m_body) {
        if (m_shapeDirty)
            bodyInterface.SetShape(m_body->GetID(), m_shape->getJoltShape(), true, activationMode);
    } else {
        m_bodySettings.SetShape(m_shape->getJoltShape());
        m_bodySettings.mPosition = PhysicsUtils::toJoltType(scenePosition());
        m_bodySettings.mRotation = PhysicsUtils::toJoltType(sceneRotation());
        m_bodySettings.mMotionType = static_cast<JPH::EMotionType>(m_motionType);
        m_bodySettings.mMotionQuality = static_cast<JPH::EMotionQuality>(m_motionQuality);
        m_bodySettings.mAllowedDOFs = static_cast<JPH::EAllowedDOFs>(m_allowedDOFs);

        m_bodySettings.mOverrideMassProperties = static_cast<JPH::EOverrideMassProperties>(m_overrideMassProperties);
        m_bodySettings.mMassPropertiesOverride.mMass = m_mass;
        m_bodySettings.mMassPropertiesOverride.mInertia = PhysicsUtils::toJoltType(m_inertia);

        if (m_collisionGroup)
            m_bodySettings.mCollisionGroup = m_collisionGroup->getJoltCollisionGroup();

        m_bodySettings.mIsSensor = isSensor();

        m_body = bodyInterface.CreateBody(m_bodySettings);
        m_body->SetUserData(reinterpret_cast<JPH::uint64>(this));

        if (m_usedInSimulation)
            bodyInterface.AddBody(m_body->GetID(), activationMode);

        m_bodyID = static_cast<int>(m_body->GetID().GetIndexAndSequenceNumber());
        emit bodyIDChanged(m_bodyID);
    }

    m_shapeDirty = false;
}

void Body::cleanup()
{
    if (m_body) {
        auto &bodyInterface = m_jolt->GetBodyInterface();
        if (m_usedInSimulation)
            bodyInterface.RemoveBody(m_body->GetID());
        bodyInterface.DestroyBody(m_body->GetID());
    }

    m_body = nullptr;
}

void Body::sync()
{
    if (m_body == nullptr)
        return;

    auto &bodyInterface = m_jolt->GetBodyInterface();
    const auto pos = bodyInterface.GetPosition(m_body->GetID());
    const auto rotation = bodyInterface.GetRotation(m_body->GetID());
    const auto qtPosition = PhysicsUtils::toQtType(pos);
    const auto qtRotation = PhysicsUtils::toQtType(rotation);

    const QQuick3DNode *parentNode = static_cast<QQuick3DNode *>(parentItem());

    m_syncing = true;

    if (!parentNode) {
        setPosition(qtPosition);
        setRotation(qtRotation);
    } else {
        setPosition(parentNode->mapPositionFromScene(qtPosition));
        const auto relativeRotation = parentNode->sceneRotation().inverted() * qtRotation;
        setRotation(relativeRotation);
    }

    m_syncing = false;
}

void Body::handleScenePositionChanged()
{
    if (m_body == nullptr || m_syncing)
        return;

    auto &bodyInterface = m_jolt->GetBodyInterface();
    const auto activationMode = getActivationForMotionType();
    bodyInterface.SetPosition(m_body->GetID(), PhysicsUtils::toJoltType(scenePosition()), activationMode);
}

void Body::handleSceneRotationChanged()
{
    if (m_body == nullptr || m_syncing)
        return;

    auto &bodyInterface = m_jolt->GetBodyInterface();
    const auto activationMode = getActivationForMotionType();
    bodyInterface.SetRotation(m_body->GetID(), PhysicsUtils::toJoltType(sceneRotation()), activationMode);
}

JPH::EActivation Body::getActivationForMotionType() const
{
    return m_motionType == MotionType::Static ? JPH::EActivation::DontActivate : JPH::EActivation::Activate;
}
