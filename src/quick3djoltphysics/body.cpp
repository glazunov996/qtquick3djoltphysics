#include "body_p.h"
#include "physicssystem_p.h"
#include "physicsutils_p.h"

#include <Jolt/Physics/Collision/Shape/OffsetCenterOfMassShape.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Core/Memory.h>

#include <QQuaternion>

Body::Body(QQuick3DNode *parent) : AbstractPhysicsBody(parent)
{
}

Body::~Body()
{
    Body::cleanup();
}

int Body::bodyID() const
{
    return m_bodyID;
}

bool Body::isSensor() const
{
    return m_bodySettings.mIsSensor;
}

void Body::setIsSensor(bool isSensor)
{
    if (m_bodySettings.mIsSensor == isSensor)
        return;

    m_bodySettings.mIsSensor = isSensor;
    if (m_body)
        m_body->SetIsSensor(isSensor);

    emit isSensorChanged(isSensor);
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
        const auto activationMode = getActivationForMotionType();
        m_bodyInterface->SetMotionType(m_body->GetID(), static_cast<JPH::EMotionType>(m_motionType), activationMode);
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
    if (m_body)
        m_bodyInterface->SetMotionQuality(m_body->GetID(), static_cast<JPH::EMotionQuality>(m_motionQuality));

    emit motionQualityChanged(motionQuality);
}


bool Body::enhancedInternalEdgeRemoval() const
{
    return m_bodySettings.mEnhancedInternalEdgeRemoval;
}

void Body::setEnhancedInternalEdgeRemoval(bool enhancedInternalEdgeRemoval)
{
    if (m_bodySettings.mEnhancedInternalEdgeRemoval == enhancedInternalEdgeRemoval)
        return;

    m_bodySettings.mEnhancedInternalEdgeRemoval = enhancedInternalEdgeRemoval;
    if (m_body)
        m_body->SetEnhancedInternalEdgeRemoval(enhancedInternalEdgeRemoval);

    emit enhancedInternalEdgeRemovalChanged(enhancedInternalEdgeRemoval);
}

int Body::objectLayer() const
{
    return m_bodySettings.mObjectLayer;
}

void Body::setObjectLayer(int objectLayer)
{
    if (m_bodySettings.mObjectLayer == objectLayer)
        return;

    m_bodySettings.mObjectLayer = objectLayer;
    if (m_body)
        m_bodyInterface->SetObjectLayer(m_body->GetID(), m_bodySettings.mObjectLayer);

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
        if (m_usedInSimulation) {
            const auto activationMode = getActivationForMotionType();
            m_bodyInterface->AddBody(m_body->GetID(), activationMode);
        } else {
            m_bodyInterface->RemoveBody(m_body->GetID());
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

QVector3D Body::offsetCenterOfMass() const
{
    return m_offsetCenterOfMass;
}

void Body::setOffsetCenterOfMass(const QVector3D &offsetCenterOfMass)
{
    if (m_offsetCenterOfMass == offsetCenterOfMass)
        return;

    m_offsetCenterOfMass = offsetCenterOfMass;
    updateJoltObject();

    emit offsetCenterOfMassChanged(m_offsetCenterOfMass);
}

float Body::maxLinearVelocity() const
{
    return m_bodySettings.mMaxLinearVelocity;
}

void Body::setMaxLinearVelocity(float maxLinearVelocity)
{
    if (qFuzzyCompare(m_bodySettings.mMaxLinearVelocity, maxLinearVelocity))
        return;

    m_bodySettings.mMaxLinearVelocity = maxLinearVelocity;
    if (m_body)
        m_body->GetMotionProperties()->SetMaxLinearVelocity(maxLinearVelocity);

    emit maxLinearVelocityChanged(maxLinearVelocity);
}

float Body::maxAngularVelocity() const
{
    return m_bodySettings.mMaxAngularVelocity;
}

void Body::setMaxAngularVelocity(float maxAngularVelocity)
{
    if (qFuzzyCompare(m_bodySettings.mMaxAngularVelocity, maxAngularVelocity))
        return;

    m_bodySettings.mMaxAngularVelocity = maxAngularVelocity;
    if (m_body)
        m_body->GetMotionProperties()->SetMaxAngularVelocity(maxAngularVelocity);

    emit maxAngularVelocityChanged(maxAngularVelocity);
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

QVector3D Body::kinematicPosition() const
{
    return m_kinematicPosition;
}

void Body::setKinematicPosition(const QVector3D &position)
{
    if (m_kinematicPosition == position)
        return;

    m_kinematicPosition = position;
    emit kinematicPositionChanged(m_kinematicPosition);
}

QQuaternion Body::kinematicRotation() const
{
    return m_kinematicRotation.getQuaternionRotation();
}

void Body::setKinematicRotation(const QQuaternion &rotation)
{
    if (m_kinematicRotation == rotation)
        return;

    m_kinematicRotation = rotation;
    emit kinematicRotationChanged(m_kinematicRotation.getQuaternionRotation());
}

QVector3D Body::kinematicEulerRotation() const
{
    return m_kinematicRotation.getEulerRotation();
}

void Body::setKinematicEulerRotation(const QVector3D &eulerRotation)
{
    if (m_kinematicRotation == eulerRotation)
        return;

    m_kinematicRotation = eulerRotation;
    emit kinematicEulerRotationChanged(m_kinematicRotation.getEulerRotation());
}

QVector3D Body::kinematicPivot() const
{
    return m_kinematicPivot;
}

void Body::setKinematicPivot(const QVector3D &pivot)
{
    if (m_kinematicPivot == pivot)
        return;

    m_kinematicPivot = pivot;
    emit kinematicPivotChanged(m_kinematicPivot);
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

QVector3D Body::getCenterOfMass() const
{
    if (m_body == nullptr)
        return QVector3D();

    return PhysicsUtils::toQtType(m_body->GetShape()->GetCenterOfMass());
}

void Body::activate()
{
    if (m_body == nullptr) {
        qWarning() << "Warning: Invoking 'activate' before body is initialized will have no effect";
        return;
    }

    m_bodyInterface->ActivateBody(m_body->GetID());
}

void Body::deactivate()
{
    if (m_body == nullptr) {
        qWarning() << "Warning: Invoking 'deactivate' before body is initialized will have no effect";
        return;
    }

    m_bodyInterface->DeactivateBody(m_body->GetID());
}

void Body::addForce(const QVector3D &force)
{
    if (m_body == nullptr) {
        qWarning() << "Warning: Invoking 'addForce' before body is initialized will have no effect";
        return;
    }

    m_bodyInterface->AddForce(m_body->GetID(), PhysicsUtils::toJoltType(force));
}

void Body::addForce(const QVector3D &force, const QVector3D &position)
{
    if (m_body == nullptr) {
        qWarning() << "Warning: Invoking 'addForce' before body is initialized will have no effect";
        return;
    }

    m_bodyInterface->AddForce(m_body->GetID(), PhysicsUtils::toJoltType(force), PhysicsUtils::toJoltType(position));
}

void Body::addTorque(const QVector3D &torque)
{
    if (m_body == nullptr) {
        qWarning() << "Warning: Invoking 'addTorque' before body is initialized will have no effect";
        return;
    }

    m_bodyInterface->AddTorque(m_body->GetID(), PhysicsUtils::toJoltType(torque));
}

void Body::resetForce()
{
    if (m_body == nullptr) {
        qWarning() << "Warning: Invoking 'resetForce' before body is initialized will have no effect";
        return;
    }

    m_body->ResetForce();
}

void Body::resetTorque()
{
    if (m_body == nullptr) {
        qWarning() << "Warning: Invoking 'resetTorque' before body is initialized will have no effect";
        return;
    }

    m_body->ResetTorque();
}

void Body::resetMotion()
{
    if (m_body == nullptr) {
        qWarning() << "Warning: Invoking 'resetMotion' before body is initialized will have no effect";
        return;
    }

    m_body->ResetMotion();
}

void Body::addImpulse(const QVector3D &impulse)
{
    if (m_body == nullptr) {
        qWarning() << "Warning: Invoking 'addImpulse' before body is initialized will have no effect";
        return;
    }

    m_bodyInterface->AddImpulse(m_body->GetID(), PhysicsUtils::toJoltType(impulse));
}

void Body::addImpulse(const QVector3D &impulse, const QVector3D &position)
{
    if (m_body == nullptr) {
        qWarning() << "Warning: Invoking 'addImpulse' before body is initialized will have no effect";
        return;
    }

    m_bodyInterface->AddImpulse(m_body->GetID(), PhysicsUtils::toJoltType(impulse), PhysicsUtils::toJoltType(position));
}

void Body::addAngularImpulse(const QVector3D &angularImpulse)
{
    if (m_body == nullptr) {
        qWarning() << "Warning: Invoking 'addAngularImpulse' before body is initialized will have no effect";
        return;
    }

    m_bodyInterface->AddAngularImpulse(m_body->GetID(), PhysicsUtils::toJoltType(angularImpulse));
}

void Body::reset(const QVector3D &position, const QVector3D &eulerRotation)
{
    if (m_body == nullptr) {
        qWarning() << "Warning: Invoking 'reset' before body is initialized will have no effect";
        return;
    }

    auto *parentNode = this->parentNode();
    QVector3D scenePosition = parentNode ? parentNode->mapPositionToScene(position) : position;

    const auto activationMode = getActivationForMotionType();
    m_bodyInterface->SetPositionAndRotation(m_body->GetID(),
                                            PhysicsUtils::toJoltType(scenePosition),
                                            PhysicsUtils::toJoltType(QQuaternion::fromEulerAngles(eulerRotation)),
                                            activationMode);
    m_body->ResetForce();
    m_body->ResetTorque();
    m_body->ResetMotion();
}

static QMatrix4x4 calculateKinematicNodeTransform(QQuick3DNode *node,
                                                  QHash<QQuick3DNode *, QMatrix4x4> &transformCache)
{
    if (transformCache.contains(node))
        return transformCache[node];

    QMatrix4x4 localTransform;
    if (auto body = qobject_cast<const Body *>(node); body != nullptr) {
        if (body->motionType() != Body::MotionType::Kinematic) {
            qWarning() << "Non-kinematic body as a parent of a kinematic body is unsupported";
        }
        localTransform = QSSGRenderNode::calculateTransformMatrix(body->kinematicPosition(), body->scale(), body->kinematicPivot(),
                                                                  body->kinematicRotation());
    } else {
        localTransform = QSSGRenderNode::calculateTransformMatrix(node->position(), node->scale(), node->pivot(),
                                                                  node->rotation());
    }

    QQuick3DNode *parent = node->parentNode();
    if (!parent)
        return localTransform;

    QMatrix4x4 parentTransform = calculateKinematicNodeTransform(parent, transformCache);
    QMatrix4x4 sceneTransform = parentTransform * localTransform;

    transformCache[node] = sceneTransform;
    return sceneTransform;
}

static void getJoltPositionAndRotation(const QMatrix4x4 &transform, JPH::Vec3 &worldPosition, JPH::Quat &rotation)
{
    auto rotationMatrix = transform;
    QSSGUtils::mat44::normalize(rotationMatrix);
    rotation = PhysicsUtils::toJoltType(QQuaternion::fromRotationMatrix(QSSGUtils::mat44::getUpper3x3(rotationMatrix)).normalized());
    worldPosition = PhysicsUtils::toJoltType(QSSGUtils::mat44::getPosition(transform));
}

void Body::updateJoltObject()
{
    if (m_jolt == nullptr || m_shape == nullptr)
        return;

    auto shape = m_shape->getJoltShape();

    if (m_motionType == MotionType::Dynamic && shape->MustBeStatic()) {
        qWarning() << "Cannot make body containing static shape, forcing kinematic.";
        m_motionType = MotionType::Kinematic;
        emit motionTypeChanged(m_motionType);
    }

    if (m_motionType == MotionType::Kinematic && shape->MustBeStatic() && m_overrideMassProperties != OverrideMassProperties::MassAndInertiaProvided) {
        qWarning() << "Cannot make kinematic body containting static shape without mass and inertia provided, forcing mass 1.0 if not provided.";
        setOverrideMassProperties(OverrideMassProperties::MassAndInertiaProvided);
        setMass(1.0);
        setInertia(QMatrix4x4());
    }

    if (!shape->MustBeStatic())
        shape = new JPH::OffsetCenterOfMassShape(shape, PhysicsUtils::toJoltType(m_offsetCenterOfMass));

    const auto activationMode = getActivationForMotionType();

    if (m_body == nullptr) {
        m_bodySettings.SetShape(shape);
        m_bodySettings.mMotionType = static_cast<JPH::EMotionType>(m_motionType);
        m_bodySettings.mMotionQuality = static_cast<JPH::EMotionQuality>(m_motionQuality);
        m_bodySettings.mAllowedDOFs = static_cast<JPH::EAllowedDOFs>(m_allowedDOFs);
        m_bodySettings.mOverrideMassProperties = static_cast<JPH::EOverrideMassProperties>(m_overrideMassProperties);
        m_bodySettings.mMassPropertiesOverride.mMass = m_mass;
        m_bodySettings.mMassPropertiesOverride.mInertia = PhysicsUtils::toJoltType(m_inertia);

        if (m_collisionGroup)
            m_bodySettings.mCollisionGroup = m_collisionGroup->getJoltCollisionGroup();

        if (m_motionType == MotionType::Kinematic) {
            setKinematicPosition(position());
            setKinematicRotation(rotation());
            QHash<QQuick3DNode *, QMatrix4x4> transformCache;
            const QMatrix4x4 transform = calculateKinematicNodeTransform(this, transformCache);
            getJoltPositionAndRotation(transform, m_bodySettings.mPosition, m_bodySettings.mRotation);
        } else {
            m_bodySettings.mPosition = PhysicsUtils::toJoltType(scenePosition());
            m_bodySettings.mRotation = PhysicsUtils::toJoltType(sceneRotation());
        }

        m_body = m_bodyInterface->CreateBody(m_bodySettings);
        m_body->SetUserData(reinterpret_cast<JPH::uint64>(this));

        if (m_usedInSimulation)
            m_bodyInterface->AddBody(m_body->GetID(), activationMode);

        m_bodyID = static_cast<int>(m_body->GetID().GetIndexAndSequenceNumber());
        emit bodyIDChanged(m_bodyID);

        sync();
    } else {
        m_bodyInterface->SetShape(m_body->GetID(), shape, true, activationMode);
    }
}

void Body::cleanup()
{
    if (m_body) {
        if (m_usedInSimulation)
            m_bodyInterface->RemoveBody(m_body->GetID());
        m_bodyInterface->DestroyBody(m_body->GetID());
    }

    m_body = nullptr;

    AbstractPhysicsBody::cleanup();
}

void Body::preSync(float deltaTime, QHash<QQuick3DNode *, QMatrix4x4> &transformCache)
{
    Q_UNUSED(deltaTime)

    if (m_body == nullptr || m_motionType == MotionType::Dynamic)
        return;

    JPH::Vec3 oldPosition;
    JPH::Quat oldRotation;

    m_bodyInterface->GetPositionAndRotation(m_body->GetID(), oldPosition, oldRotation);

    if (m_motionType == MotionType::Kinematic) {
        JPH::Vec3 position;
        JPH::Quat rotation;

        const QMatrix4x4 transform = calculateKinematicNodeTransform(this, transformCache);
        getJoltPositionAndRotation(transform, position, rotation);

        if (!qFuzzyCompare(PhysicsUtils::toQtType(oldPosition), PhysicsUtils::toQtType(position))
                || !qFuzzyCompare(PhysicsUtils::toQtType(oldRotation), PhysicsUtils::toQtType(rotation))) {
            m_bodyInterface->MoveKinematic(m_body->GetID(), position, rotation, deltaTime);
            m_moveKinematicNeedsReset = true;
        } else if (m_moveKinematicNeedsReset) {
            m_moveKinematicNeedsReset = false;
            m_body->ResetMotion();
        }
    } else {
        if (!qFuzzyCompare(PhysicsUtils::toQtType(oldPosition), scenePosition())
                || !qFuzzyCompare(PhysicsUtils::toQtType(oldRotation), sceneRotation())) {
            const auto activationMode = getActivationForMotionType();
            m_bodyInterface->SetPositionAndRotation(m_body->GetID(),
                                                    PhysicsUtils::toJoltType(scenePosition()),
                                                    PhysicsUtils::toJoltType(sceneRotation()),
                                                    activationMode);
        }
    }
}

void Body::sync()
{
    const auto activationMode = getActivationForMotionType();
    if (m_body == nullptr || !m_usedInSimulation || activationMode == JPH::EActivation::DontActivate)
        return;

    const auto pos = m_bodyInterface->GetPosition(m_body->GetID());
    const auto rotation = m_bodyInterface->GetRotation(m_body->GetID());
    const auto qtRotation = PhysicsUtils::toQtType(rotation);
    const auto qtPosition = PhysicsUtils::toQtType(pos);

    const QQuick3DNode *parentNode = static_cast<QQuick3DNode *>(parentItem());

    if (!parentNode) {
        setPosition(qtPosition);
        setRotation(qtRotation);
    } else {
        setPosition(parentNode->mapPositionFromScene(qtPosition));
        const auto relativeRotation = parentNode->sceneRotation().inverted() * qtRotation;
        setRotation(relativeRotation);
    }

    AbstractPhysicsBody::sync();
}

JPH::EActivation Body::getActivationForMotionType() const
{
    return m_motionType == MotionType::Static ? JPH::EActivation::DontActivate : JPH::EActivation::Activate;
}
