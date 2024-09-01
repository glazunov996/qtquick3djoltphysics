#include "charactervirtual_p.h"
#include "physicsutils_p.h"

#include <Jolt/Physics/PhysicsSystem.h>

CharacterVirtual::CharacterVirtual(QQuick3DNode *parent) : AbstractPhysicsBody(parent)
{
    connect(this, &QQuick3DNode::upChanged, this, &CharacterVirtual::handleUpChanged);
    connect(this, &QQuick3DNode::sceneRotationChanged, this, &CharacterVirtual::handleSceneRotationChanged);
}

CharacterVirtual::~CharacterVirtual()
{
    CharacterVirtual::cleanup();
}

QVector4D CharacterVirtual::supportingVolume() const
{
    return m_supportingVolume;
}

void CharacterVirtual::setSupportingVolume(const QVector4D &supportingVolume)
{
    if (m_supportingVolume == supportingVolume)
        return;

    if (m_character) {
        qWarning() << "Warning: Changing 'supportingVolume' after character is initialized will have "
                      "no effect";
        return;
    }

    m_supportingVolume = supportingVolume;
    emit supportingVolumeChanged(supportingVolume);
}

float CharacterVirtual::maxSlopeAngle() const
{
    return m_maxSlopeAngle;
}

void CharacterVirtual::setMaxSlopeAngle(float maxSlopeAngle)
{
    if (qFuzzyCompare(m_maxSlopeAngle, maxSlopeAngle))
        return;

    m_maxSlopeAngle = maxSlopeAngle;
    if (m_character)
        m_character->SetMaxSlopeAngle(maxSlopeAngle);

    emit maxSlopeAngleChanged(maxSlopeAngle);
}

float CharacterVirtual::mass() const
{
    return m_characterSettings.mMass;
}

void CharacterVirtual::setMass(float mass)
{
    if (qFuzzyCompare(m_characterSettings.mMass, mass))
        return;

    m_characterSettings.mMass = mass;
    if (m_character)
        m_character->SetMass(mass);

    emit massChanged(mass);
}

float CharacterVirtual::maxStrength() const
{
    return m_characterSettings.mMaxStrength;
}

void CharacterVirtual::setMaxStrength(float maxStrength)
{
    if (qFuzzyCompare(m_characterSettings.mMaxStrength, maxStrength))
        return;

    m_characterSettings.mMaxStrength = maxStrength;
    if (m_character)
        m_character->SetMaxStrength(maxStrength);

    emit maxStrengthChanged(maxStrength);
}

QVector3D CharacterVirtual::shapeOffset() const
{
    return m_shapeOffset;
}

void CharacterVirtual::setShapeOffset(const QVector3D &shapeOffset)
{
    if (m_shapeOffset == shapeOffset)
        return;

    m_shapeOffset = shapeOffset;
    if (m_character)
        m_character->SetShapeOffset(PhysicsUtils::toJoltType(shapeOffset));

    emit shapeOffsetChanged(shapeOffset);
}

CharacterVirtual::BackFaceMode CharacterVirtual::backFaceMode() const
{
    return m_backFaceMode;
}

void CharacterVirtual::setBackFaceMode(BackFaceMode backFaceMode)
{
    if (m_backFaceMode == backFaceMode)
        return;

    if (m_character) {
        qWarning() << "Warning: Changing 'backFaceMode' after character is initialized will have "
                      "no effect";
        return;
    }

    m_backFaceMode = backFaceMode;
    emit backFaceModeChanged(backFaceMode);
}

float CharacterVirtual::predictiveContactDistance() const
{
    return m_characterSettings.mPredictiveContactDistance;
}

void CharacterVirtual::setPredictiveContactDistance(float predictiveContactDistance)
{
    if (qFuzzyCompare(m_characterSettings.mPredictiveContactDistance, predictiveContactDistance))
        return;

    if (m_character) {
        qWarning() << "Warning: Changing 'predictiveContactDistance' after character is initialized will have "
                      "no effect";
        return;
    }

    m_characterSettings.mPredictiveContactDistance = predictiveContactDistance;
    emit predictiveContactDistanceChanged(predictiveContactDistance);
}

quint8 CharacterVirtual::maxCollisionIterations() const
{
    return m_characterSettings.mMaxCollisionIterations;
}

void CharacterVirtual::setMaxCollisionIterations(quint8 maxCollisionIterations)
{
    if (m_characterSettings.mMaxCollisionIterations == maxCollisionIterations)
        return;

    if (m_character) {
        qWarning() << "Warning: Changing 'maxCollisionIterations' after character is initialized will have "
                      "no effect";
        return;
    }

    m_characterSettings.mMaxCollisionIterations = maxCollisionIterations;
    emit maxCollisionIterationsChanged(maxCollisionIterations);
}

quint8 CharacterVirtual::maxConstraintIterations() const
{
    return m_characterSettings.mMaxConstraintIterations;
}

void CharacterVirtual::setMaxConstraintIterations(quint8 maxConstraintIterations)
{
    if (m_characterSettings.mMaxCollisionIterations == maxConstraintIterations)
        return;

    if (m_character) {
        qWarning() << "Warning: Changing 'maxConstraintIterations' after character is initialized will have "
                      "no effect";
        return;
    }

    m_characterSettings.mMaxConstraintIterations = maxConstraintIterations;
    emit maxConstraintIterationsChanged(maxConstraintIterations);
}

float CharacterVirtual::minTimeRemaining() const
{
    return m_characterSettings.mMinTimeRemaining;
}

void CharacterVirtual::setMinTimeRemaining(float minTimeRemaining)
{
    if (qFuzzyCompare(m_characterSettings.mMinTimeRemaining, minTimeRemaining))
        return;

    if (m_character) {
        qWarning() << "Warning: Changing 'minTimeRemaining' after character is initialized will have "
                      "no effect";
        return;
    }

    m_characterSettings.mMinTimeRemaining = minTimeRemaining;
    emit minTimeRemainingChanged(minTimeRemaining);
}

float CharacterVirtual::collisionTolerance() const
{
    return m_characterSettings.mCollisionTolerance;
}

void CharacterVirtual::setCollisionTolerance(float collisionTolerance)
{
    if (qFuzzyCompare(m_characterSettings.mCollisionTolerance, collisionTolerance))
        return;

    if (m_character) {
        qWarning() << "Warning: Changing 'collisionTolerance' after character is initialized will have "
                      "no effect";
        return;
    }

    m_characterSettings.mCollisionTolerance = collisionTolerance;
    emit collisionToleranceChanged(collisionTolerance);
}

float CharacterVirtual::characterPadding() const
{
    return m_characterSettings.mCharacterPadding;
}

void CharacterVirtual::setCharacterPadding(float characterPadding)
{
    if (qFuzzyCompare(m_characterSettings.mCharacterPadding, characterPadding))
        return;

    if (m_character) {
        qWarning() << "Warning: Changing 'characterPadding' after character is initialized will have "
                      "no effect";
        return;
    }

    m_characterSettings.mCharacterPadding = characterPadding;
    emit characterPaddingChanged(characterPadding);
}

quint8 CharacterVirtual::maxNumHits() const
{
    return m_characterSettings.mMaxNumHits;
}

void CharacterVirtual::setMaxNumHits(quint8 maxNumHits)
{
    if (m_characterSettings.mMaxNumHits == maxNumHits)
        return;

    m_characterSettings.mMaxNumHits = maxNumHits;
    if (m_character)
        m_character->SetMaxNumHits(maxNumHits);

    emit maxNumHitsChanged(maxNumHits);
}

float CharacterVirtual::hitReductionCosMaxAngle() const
{
    return m_characterSettings.mHitReductionCosMaxAngle;
}

void CharacterVirtual::setHitReductionCosMaxAngle(float hitReductionCosMaxAngle)
{
    if (qFuzzyCompare(m_characterSettings.mHitReductionCosMaxAngle, hitReductionCosMaxAngle))
        return;

    m_characterSettings.mHitReductionCosMaxAngle = hitReductionCosMaxAngle;
    if (m_character)
        m_character->SetHitReductionCosMaxAngle(hitReductionCosMaxAngle);

    emit hitReductionCosMaxAngleChanged(hitReductionCosMaxAngle);
}

float CharacterVirtual::penetrationRecoverySpeed() const
{
    return m_characterSettings.mPenetrationRecoverySpeed;
}

void CharacterVirtual::setPenetrationRecoverySpeed(float penetrationRecoverySpeed)
{
    if (qFuzzyCompare(m_characterSettings.mPenetrationRecoverySpeed, penetrationRecoverySpeed))
        return;

    m_characterSettings.mPenetrationRecoverySpeed = penetrationRecoverySpeed;
    if (m_character)
        m_character->SetPenetrationRecoverySpeed(penetrationRecoverySpeed);

    emit penetrationRecoverySpeedChanged(penetrationRecoverySpeed);
}

AbstractCharacterContactListener *CharacterVirtual::characterContactListener() const
{
    return m_characterContactListener;
}

void CharacterVirtual::setCharacterContactListener(AbstractCharacterContactListener *characterContactListener)
{
    if (m_characterContactListener == characterContactListener)
        return;

    m_characterContactListener = characterContactListener;

    QObject::connect(m_characterContactListener, &QObject::destroyed, this,
                    [this]
                    {
                        m_characterContactListener = nullptr;
                        if (m_character)
                            m_character->SetListener(nullptr);
                    });

    if (m_character)
        m_character->SetListener(m_characterContactListener->getJoltCharacterContactListener());

    emit characterContactListenerChanged(m_characterContactListener);
}

QVector3D CharacterVirtual::getLinearVelocity() const
{
    if (m_character == nullptr)
        return QVector3D();

    return PhysicsUtils::toQtType(m_character->GetLinearVelocity());
}

void CharacterVirtual::setLinearVelocity(const QVector3D &linearVelocity)
{
    if (m_character == nullptr) {
        qWarning() << "Warning: Invoking 'setLinearVelocity' before character is initialized will have no effect";
        return;
    }

    m_character->SetLinearVelocity(PhysicsUtils::toJoltType(linearVelocity));
}

CharacterVirtual::GroundState CharacterVirtual::getGroundState() const
{
    if (m_character == nullptr)
        return GroundState::OnGround;

    return static_cast<GroundState>(m_character->GetGroundState());
}

QVector3D CharacterVirtual::getGroundPosition() const
{
    if (m_character == nullptr)
        return QVector3D();

    return PhysicsUtils::toQtType(m_character->GetGroundPosition());
}

QVector3D CharacterVirtual::getGroundNormal() const
{
    if (m_character == nullptr)
        return QVector3D();

    return PhysicsUtils::toQtType(m_character->GetGroundNormal());
}

QVector3D CharacterVirtual::getGroundVelocity() const
{
    if (m_character == nullptr)
        return QVector3D();

    return PhysicsUtils::toQtType(m_character->GetGroundVelocity());
}

QMatrix4x4 CharacterVirtual::getCenterOfMassTransform() const
{
    if (m_character == nullptr)
        return QMatrix4x4();

    return PhysicsUtils::toQtType(m_character->GetCenterOfMassTransform());
}

bool CharacterVirtual::isSlopeTooSteep(const QVector3D &normal)
{
    if (m_character == nullptr)
        return false;

    return m_character->IsSlopeTooSteep(PhysicsUtils::toJoltType(normal));
}

void CharacterVirtual::extendedUpdate(float deltaTime,
                                      const QVector3D &gravity,
                                      ExtendedUpdateSettings *updateSettings,
                                      unsigned int broadPhaseLayerFilter,
                                      unsigned int objectLayerFilter)
{
    if (m_character == nullptr || m_jolt == nullptr)
        return;

    JPH::CharacterVirtual::ExtendedUpdateSettings update_settings;
    if (updateSettings) {
        update_settings.mStickToFloorStepDown = PhysicsUtils::toJoltType(updateSettings->stickToFloorStepDown);
        update_settings.mWalkStairsStepUp = PhysicsUtils::toJoltType(updateSettings->walkStairsStepUp);
        update_settings.mWalkStairsMinStepForward = updateSettings->walkStairsMinStepForward;
        update_settings.mWalkStairsStepForwardTest = updateSettings->walkStairsStepForwardTest;
        update_settings.mWalkStairsCosAngleForwardContact = updateSettings->walkStairsCosAngleForwardContact;
        update_settings.mWalkStairsStepDownExtra = PhysicsUtils::toJoltType(updateSettings->walkStairsStepDownExtra);
    }

    m_character->ExtendedUpdate(deltaTime,
                                PhysicsUtils::toJoltType(gravity),
                                update_settings,
                                m_jolt->GetDefaultBroadPhaseLayerFilter(broadPhaseLayerFilter),
                                m_jolt->GetDefaultLayerFilter(objectLayerFilter),
                                {},
                                {},
                                *m_tempAllocator);
}

void CharacterVirtual::updateGroundVelocity()
{
    if (m_character == nullptr)
        return;

    m_character->UpdateGroundVelocity();
}

void CharacterVirtual::setShape(AbstractShape *shape, float maxPenetrationDepth, unsigned int broadPhaseLayerFilter, unsigned int objectLayerFilter)
{
    if (m_character == nullptr)
        return;

    m_maxPenetrationDepth = maxPenetrationDepth;
    m_broadPhaseLayerFilter = broadPhaseLayerFilter;
    m_objectLayerFilter = objectLayerFilter;

    m_updateShapeNeeded = true;
    AbstractPhysicsBody::setShape(shape);
}

void CharacterVirtual::updateJoltObject()
{
    if (m_jolt == nullptr || m_shape == nullptr)
        return;

    if (m_character) {
        if (m_shapeDirty) {
            if (m_updateShapeNeeded) {
                m_updateShapeNeeded = false;
                m_character->SetShape(m_shape->getJoltShape(),
                                      m_maxPenetrationDepth,
                                      m_jolt->GetDefaultBroadPhaseLayerFilter(m_broadPhaseLayerFilter),
                                      m_jolt->GetDefaultLayerFilter(m_objectLayerFilter),
                                      {},
                                      {},
                                      *m_tempAllocator);
            } else {
                qWarning() << "Warning: To change character shape, the invokable 'setShape' must be called.";
            }
        }
    } else {
        m_characterSettings.mShape = m_shape->getJoltShape();
        m_characterSettings.mSupportingVolume = JPH::Plane(PhysicsUtils::toJoltType(m_supportingVolume.toVector3D()), m_supportingVolume.w());
        m_characterSettings.mBackFaceMode = static_cast<JPH::EBackFaceMode>(m_backFaceMode);
        m_characterSettings.mMaxSlopeAngle = qDegreesToRadians(m_maxSlopeAngle);
        m_characterSettings.mUp = PhysicsUtils::toJoltType(up());

        m_character = new JPH::CharacterVirtual(
            &m_characterSettings, PhysicsUtils::toJoltType(scenePosition()), PhysicsUtils::toJoltType(sceneRotation()), 0, m_jolt);

        if (m_characterContactListener)
            m_character->SetListener(m_characterContactListener->getJoltCharacterContactListener());
    }

    m_shapeDirty = false;
}

void CharacterVirtual::cleanup()
{
    delete m_character;
    m_character = nullptr;
}

void CharacterVirtual::sync()
{
    if (m_character == nullptr)
        return;

    QVector3D position = PhysicsUtils::toQtType(m_character->GetPosition());
    const QQuick3DNode *parentNode = static_cast<QQuick3DNode *>(parentItem());

    if (!parentNode) {
        setPosition(position);
    } else {
        setPosition(parentNode->mapPositionFromScene(position));
    }
}

void CharacterVirtual::handleUpChanged()
{
    if (m_character == nullptr)
        return;

    m_character->SetUp(PhysicsUtils::toJoltType(up()));
}

void CharacterVirtual::handleSceneRotationChanged()
{
    if (m_character == nullptr)
        return;

    m_character->SetRotation(PhysicsUtils::toJoltType(sceneRotation()));
}
