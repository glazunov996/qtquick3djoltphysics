#include "charactervirtual_p.h"
#include "physicsutils_p.h"
#include "body_p.h"

#include <QtQuick3D/private/qquick3dobject_p.h>

#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Math/Mat44.h>

CharacterVirtual::CharacterVirtual(QQuick3DNode *parent) : AbstractPhysicsCharacter(parent)
{
    connect(this, &QQuick3DNode::upChanged, this, &CharacterVirtual::handleUpChanged);
    connect(this, &QQuick3DNode::scenePositionChanged, this, &CharacterVirtual::handleScenePositionChanged);
    connect(this, &QQuick3DNode::sceneRotationChanged, this, &CharacterVirtual::handleSceneRotationChanged);
}

CharacterVirtual::~CharacterVirtual() = default;

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

bool CharacterVirtual::enhancedInternalEdgeRemoval() const
{
    return m_characterSettings.mEnhancedInternalEdgeRemoval;
}

void CharacterVirtual::setEnhancedInternalEdgeRemoval(bool enhancedInternalEdgeRemoval)
{
    if (m_characterSettings.mEnhancedInternalEdgeRemoval == enhancedInternalEdgeRemoval)
        return;

    m_characterSettings.mEnhancedInternalEdgeRemoval = enhancedInternalEdgeRemoval;
    if (m_character)
        m_character->SetEnhancedInternalEdgeRemoval(enhancedInternalEdgeRemoval);

    emit enhancedInternalEdgeRemovalChanged(enhancedInternalEdgeRemoval);
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

int CharacterVirtual::maxCollisionIterations() const
{
    return m_characterSettings.mMaxCollisionIterations;
}

void CharacterVirtual::setMaxCollisionIterations(int maxCollisionIterations)
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

int CharacterVirtual::maxConstraintIterations() const
{
    return m_characterSettings.mMaxConstraintIterations;
}

void CharacterVirtual::setMaxConstraintIterations(int maxConstraintIterations)
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

int CharacterVirtual::maxNumHits() const
{
    return m_characterSettings.mMaxNumHits;
}

void CharacterVirtual::setMaxNumHits(int maxNumHits)
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

int CharacterVirtual::innerBodyShapeLayer() const
{
    return m_characterSettings.mInnerBodyLayer;
}

void CharacterVirtual::setInnerBodyShapeLayer(int innerShapeLayer)
{
    if (m_characterSettings.mInnerBodyLayer == innerShapeLayer)
        return;

    if (m_character) {
        qWarning() << "Warning: Changing 'innerShapeLayer' after character is initialized will have "
                      "no effect";
        return;
    }

    m_characterSettings.mInnerBodyLayer = innerShapeLayer;
    emit innerBodyShapeLayerChanged(innerShapeLayer);
}

int CharacterVirtual::innerBodyID() const
{
    return m_innerBodyID;
}

AbstractShape *CharacterVirtual::innerBodyShape() const
{
    return m_innerBodyShape;
}

void CharacterVirtual::setInnerBodyShape(AbstractShape *innerBodyShape)
{
    if (m_innerBodyShape == innerBodyShape)
        return;

    if (m_character) {
        qWarning() << "Warning: To change character inner shape, the invokable 'setShape' must be called.";
        return;
    }

    if (m_innerBodyShape != nullptr)
        m_innerBodyShape->disconnect(m_innerBodyShapeSignalConnection);

    m_innerBodyShape = innerBodyShape;

    if (m_innerBodyShape->parentItem() == nullptr) {
        QQuick3DObject *parentItem = qobject_cast<QQuick3DObject *>(innerBodyShape->parent());
        if (parentItem) {
            m_innerBodyShape->setParentItem(parentItem);
        } else {
            const auto &sceneManager = QQuick3DObjectPrivate::get(this)->sceneManager;
            if (sceneManager)
                QQuick3DObjectPrivate::refSceneManager(m_innerBodyShape, *sceneManager);
        }
    }

    m_innerBodyShapeSignalConnection = QObject::connect(m_innerBodyShape, &AbstractShape::changed, this,
                                                    [this]
    {
        const auto &shape = m_innerBodyShape->getJoltShape();
        if (m_character)
            m_character->SetInnerBodyShape(shape);
        else
            m_characterSettings.mInnerBodyShape = shape;
    });

    QObject::connect(m_innerBodyShape, &QObject::destroyed, this,
                     [this] { m_innerBodyShape = nullptr; });

    emit innerBodyShapeChanged(m_innerBodyShape);
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

bool CharacterVirtual::isSlopeTooSteep(const QVector3D &normal)
{
    if (m_character == nullptr)
        return false;

    return m_character->IsSlopeTooSteep(PhysicsUtils::toJoltType(normal));
}

void CharacterVirtual::extendedUpdate(float deltaTime,
                                      const QVector3D &gravity,
                                      ExtendedUpdateSettings *updateSettings,
                                      int broadPhaseLayerFilter,
                                      int objectLayerFilter)
{
    if (m_character == nullptr) {
        qWarning() << "Warning: Invoking 'extendedUpdate' before character is initialized will have no effect";
        return;
    }

    JPH::CharacterVirtual::ExtendedUpdateSettings inSettings;
    if (updateSettings) {
        inSettings.mStickToFloorStepDown = PhysicsUtils::toJoltType(updateSettings->stickToFloorStepDown);
        inSettings.mWalkStairsStepUp = PhysicsUtils::toJoltType(updateSettings->walkStairsStepUp);
        inSettings.mWalkStairsMinStepForward = updateSettings->walkStairsMinStepForward;
        inSettings.mWalkStairsStepForwardTest = updateSettings->walkStairsStepForwardTest;
        inSettings.mWalkStairsCosAngleForwardContact = qCos(qDegreesToRadians(updateSettings->walkStairsCosAngleForwardContact));
        inSettings.mWalkStairsStepDownExtra = PhysicsUtils::toJoltType(updateSettings->walkStairsStepDownExtra);
    }

    m_character->ExtendedUpdate(deltaTime,
                                PhysicsUtils::toJoltType(gravity),
                                inSettings,
                                m_jolt->GetDefaultBroadPhaseLayerFilter(broadPhaseLayerFilter),
                                m_jolt->GetDefaultLayerFilter(objectLayerFilter),
                                {},
                                {},
                                *m_tempAllocator);
}

void CharacterVirtual::refreshContacts(int broadPhaseLayerFilter, int objectLayerFilter)
{
    if (m_character == nullptr) {
        qWarning() << "Warning: Invoking 'refreshContacts' before character is initialized will have no effect";
        return;
    }

    m_character->RefreshContacts(m_jolt->GetDefaultBroadPhaseLayerFilter(broadPhaseLayerFilter),
                                 m_jolt->GetDefaultLayerFilter(objectLayerFilter),
                                 {},
                                 {},
                                 *m_tempAllocator);
}

void CharacterVirtual::updateGroundVelocity()
{
    if (m_character == nullptr) {
        qWarning() << "Warning: Invoking 'updateGroundVelocity' before character is initialized will have no effect";
        return;
    }

    m_character->UpdateGroundVelocity();
}

bool CharacterVirtual::setShape(AbstractShape *shape, AbstractShape *innerShape, float maxPenetrationDepth, int broadPhaseLayerFilter, int objectLayerFilter)
{
    if (m_character == nullptr) {
        qWarning() << "Warning: Invoking 'setShape' before character is initialized will have no effect";
        return false;
    }

    AbstractPhysicsBody::setShape(shape);

    const auto &joltShape = shape->getJoltShape();
    if (joltShape == nullptr)
        return false;

    if (!m_character->SetShape(joltShape,
                               maxPenetrationDepth,
                               m_jolt->GetDefaultBroadPhaseLayerFilter(broadPhaseLayerFilter),
                               m_jolt->GetDefaultLayerFilter(objectLayerFilter),
                               {},
                               {},
                               *m_tempAllocator)) {
        return false;
    }

    if (innerShape)
        m_character->SetInnerBodyShape(innerShape->getJoltShape());

    sync();

    return true;
}

void CharacterVirtual::updateJoltObject()
{
    if (m_jolt == nullptr || m_shape == nullptr)
        return;

    if (m_character) {
        if (m_shapeDirty)
            qWarning() << "Warning: To change character shape, the invokable 'setShape' must be called.";
    } else {
        const auto &shape = m_shape->getJoltShape();
        if (shape == nullptr)
            return;

        m_characterSettings.mShape = shape;
        m_characterSettings.mSupportingVolume = JPH::Plane(PhysicsUtils::toJoltType(m_supportingVolume.toVector3D()), m_supportingVolume.w());
        m_characterSettings.mBackFaceMode = static_cast<JPH::EBackFaceMode>(m_backFaceMode);
        m_characterSettings.mMaxSlopeAngle = qDegreesToRadians(m_maxSlopeAngle);
        m_characterSettings.mUp = PhysicsUtils::toJoltType(up());

        if (m_innerBodyShape)
            m_characterSettings.mInnerBodyShape = m_innerBodyShape->getJoltShape();

        m_character = new JPH::CharacterVirtual(
            &m_characterSettings, PhysicsUtils::toJoltType(scenePosition()), PhysicsUtils::toJoltType(sceneRotation()), 0, m_jolt);

        m_character->SetUserData(reinterpret_cast<JPH::uint64>(this));

        if (m_characterContactListener)
            m_character->SetListener(m_characterContactListener->getJoltCharacterContactListener());

        if (m_innerBodyShape) {
            m_innerBodyID = static_cast<int>(m_character->GetInnerBodyID().GetIndexAndSequenceNumber());
            emit innerBodyIDChanged(m_innerBodyID);
        }

        sync();
    }

    m_shapeDirty = false;
}

void CharacterVirtual::cleanup()
{
    if (m_character)
        delete m_character;

    m_character = nullptr;
}

void CharacterVirtual::sync()
{
    if (m_character == nullptr)
        return;

    QVector3D position = PhysicsUtils::toQtType(m_character->GetCenterOfMassTransform().GetColumn3(3));
    const QQuick3DNode *parentNode = static_cast<QQuick3DNode *>(parentItem());

    m_syncing = true;

    if (!parentNode) {
        setPosition(position);
    } else {
        setPosition(parentNode->mapPositionFromScene(position));
    }

    m_syncing = false;

    emitContactCallbacks();
}

void CharacterVirtual::emitContactCallbacks()
{
    if (m_characterContactListener == nullptr)
        return;

    auto contacts = m_characterContactListener->takeContacts();
    for (const auto &contact : std::as_const(contacts)) {
        Body *qtBody = nullptr;
        {
            JPH::BodyLockRead bodyLock(m_jolt->GetBodyLockInterface(), JPH::BodyID(contact.bodyID2));
            if (bodyLock.Succeeded()) {
                const JPH::Body &body = bodyLock.GetBody();
                qtBody = reinterpret_cast<Body *>(body.GetUserData());
            }
        }

        if (qtBody)
            emit qtBody->bodyContact(this);
    }
}

void CharacterVirtual::handleUpChanged()
{
    if (m_character == nullptr)
        return;

    m_character->SetUp(PhysicsUtils::toJoltType(up()));
}

void CharacterVirtual::handleScenePositionChanged()
{
    if (m_character == nullptr || m_syncing)
        return;

    m_character->SetPosition(PhysicsUtils::toJoltType(scenePosition()));
}

void CharacterVirtual::handleSceneRotationChanged()
{
    if (m_character == nullptr)
        return;

    m_character->SetRotation(PhysicsUtils::toJoltType(sceneRotation()));
}
