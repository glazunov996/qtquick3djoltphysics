#include "character_p.h"
#include "physicsutils_p.h"

Character::Character(QQuick3DNode *parent) : AbstractPhysicsBody(parent)
{
}

Character::~Character()
{
    Character::cleanup();
}

int Character::bodyID() const
{
    return m_bodyID;
}

QVector4D Character::supportingVolume() const
{
    return m_supportingVolume;
}

void Character::setSupportingVolume(const QVector4D &supportingVolume)
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

float Character::maxSlopeAngle() const
{
    return m_maxSlopeAngle;
}

void Character::setMaxSlopeAngle(float maxSlopeAngle)
{
    if (qFuzzyCompare(m_maxSlopeAngle, maxSlopeAngle))
        return;

    if (m_character) {
        qWarning() << "Warning: Changing 'maxSlopeAngle' after character is initialized will have "
                      "no effect";
        return;
    }

    m_maxSlopeAngle = maxSlopeAngle;
    emit maxSlopeAngleChanged(maxSlopeAngle);
}

unsigned int Character::layer() const
{
    return m_characterSettings.mLayer;
}

void Character::setLayer(unsigned int layer)
{
    if (m_characterSettings.mLayer == layer)
        return;

    m_characterSettings.mLayer = layer;
    if (m_character)
        m_character->SetLayer(layer);

    emit layerChanged(layer);
}

float Character::mass() const
{
    return m_characterSettings.mMass;
}

void Character::setMass(float mass)
{
    if (qFuzzyCompare(m_characterSettings.mMass, mass))
        return;

    if (m_character) {
        qWarning() << "Warning: Changing 'mass' after character is initialized will have "
                      "no effect";
        return;
    }

    m_characterSettings.mMass = mass;
    emit massChanged(mass);
}

float Character::friction() const
{
    return m_characterSettings.mFriction;
}

void Character::setFriction(float friction)
{
    if (qFuzzyCompare(m_characterSettings.mFriction, friction))
        return;

    if (m_character) {
        qWarning() << "Warning: Changing 'friction' after character is initialized will have "
                      "no effect";
        return;
    }

    m_characterSettings.mFriction = friction;
    emit frictionChanged(friction);
}

float Character::gravityFactor() const
{
    return m_characterSettings.mGravityFactor;
}

void Character::setGravityFactor(float gravityFactor)
{
    if (qFuzzyCompare(m_characterSettings.mGravityFactor, gravityFactor))
        return;

    if (m_character) {
        qWarning() << "Warning: Changing 'gravityFactor' after character is initialized will have "
                      "no effect";
        return;
    }

    m_characterSettings.mGravityFactor = gravityFactor;
    emit gravityFactorChanged(gravityFactor);
}

QVector3D Character::getLinearVelocity() const
{
    if (m_character == nullptr)
        return QVector3D();

    return PhysicsUtils::toQtType(m_character->GetLinearVelocity());
}

void Character::setLinearVelocity(const QVector3D &linearVelocity)
{
    if (m_character == nullptr) {
        qWarning() << "Warning: Invoking 'setLinearVelocity' before character is initialized will have no effect";
        return;
    }

    m_character->SetLinearVelocity(PhysicsUtils::toJoltType(linearVelocity));
}

Character::GroundState Character::getGroundState() const
{
    if (m_character == nullptr)
        return GroundState::OnGround;

    return static_cast<GroundState>(m_character->GetGroundState());
}

QVector3D Character::getGroundNormal() const
{
    if (m_character == nullptr)
        return QVector3D();

    return PhysicsUtils::toQtType(m_character->GetGroundNormal());
}

void Character::postSimulation(float maxSeparationDistance)
{
    if (m_character == nullptr)
        return;

    m_character->PostSimulation(maxSeparationDistance);
}

void Character::setShape(AbstractShape *shape, float maxPenetrationDepth)
{
    if (m_character == nullptr) {
        qWarning() << "Warning: Invoking 'setShape' before character is initialized will have no effect";
        return;
    }

    m_maxPenetrationDepth = maxPenetrationDepth;

    m_updateShapeNeeded = true;
    AbstractPhysicsBody::setShape(shape);
}

void Character::updateJoltObject()
{
    if (m_jolt == nullptr || m_shape == nullptr)
        return;

    if (m_character) {
        if (m_shapeDirty) {
            if (m_updateShapeNeeded) {
                m_updateShapeNeeded = false;
                m_character->SetShape(m_shape->getJoltShape(), m_maxPenetrationDepth);
            } else {
                qWarning() << "Warning: To change character shape, the invokable 'setShape' must be called.";
            }
        }
    } else {
        m_characterSettings.mShape = m_shape->getJoltShape();
        m_characterSettings.mSupportingVolume = JPH::Plane(PhysicsUtils::toJoltType(m_supportingVolume.toVector3D()), m_supportingVolume.w());
        m_characterSettings.mMaxSlopeAngle = qDegreesToRadians(m_maxSlopeAngle);

        m_character = new JPH::Character(
            &m_characterSettings, PhysicsUtils::toJoltType(scenePosition()), PhysicsUtils::toJoltType(sceneRotation()), 0, m_jolt);
        m_character->AddToPhysicsSystem();

        m_bodyID = static_cast<int>(m_character->GetBodyID().GetIndexAndSequenceNumber());
        emit bodyIDChanged(m_bodyID);
    }

    m_shapeDirty = false;
}

void Character::cleanup()
{
    if (m_character) {
        m_character->RemoveFromPhysicsSystem();
        delete m_character;
    }

    m_character = nullptr;
}

void Character::sync()
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
