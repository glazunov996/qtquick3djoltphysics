#include "swingtwistconstraint_p.h"
#include "physicsutils_p.h"

#include <Jolt/Physics/PhysicsSystem.h>

SwingTwistConstraint::SwingTwistConstraint(QQuick3DNode *parent) : AbstractPhysicsConstraint(parent)
{
}

SwingTwistConstraint::~SwingTwistConstraint() = default;

Body *SwingTwistConstraint::body1() const
{
    return m_body1;
}

void SwingTwistConstraint::setBody1(Body *body)
{
    if (m_body1 == body)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'body1' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_body1 = body;
    emit body1Changed(m_body1);
}

Body *SwingTwistConstraint::body2() const
{
    return m_body2;
}

void SwingTwistConstraint::setBody2(Body *body)
{
    if (m_body2 == body)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'body2' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_body2 = body;
    emit body2Changed(m_body2);
}

QVector3D SwingTwistConstraint::position1() const
{
    return m_position1;
}

void SwingTwistConstraint::setPosition1(const QVector3D &position1)
{
    if (m_position1 == position1)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'position1' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_position1 = position1;
    emit position1Changed(m_position1);
}

QVector3D SwingTwistConstraint::position2() const
{
    return m_position2;
}

void SwingTwistConstraint::setPosition2(const QVector3D &position2)
{
    if (m_position2 == position2)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'position2' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_position2 = position2;
    emit position2Changed(m_position2);
}

QVector3D SwingTwistConstraint::twistAxis1() const
{
    return m_twistAxis1;
}

void SwingTwistConstraint::setTwistAxis1(const QVector3D &twistAxis1)
{
    if (m_twistAxis1 == twistAxis1)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'twistAxis1' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_twistAxis1 = twistAxis1;
    emit twistAxis1Changed(m_twistAxis1);
}

QVector3D SwingTwistConstraint::twistAxis2() const
{
    return m_twistAxis2;
}

void SwingTwistConstraint::setTwistAxis2(const QVector3D &twistAxis2)
{
    if (m_twistAxis2 == twistAxis2)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'twistAxis2' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_twistAxis2 = twistAxis2;
    emit twistAxis1Changed(m_twistAxis2);
}

QVector3D SwingTwistConstraint::planeAxis1() const
{
    return m_planeAxis1;
}

void SwingTwistConstraint::setPlaneAxis1(const QVector3D &planeAxis1)
{
    if (m_planeAxis1 == planeAxis1)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'planeAxis1' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_planeAxis1 = planeAxis1;
    emit planeAxis1Changed(m_planeAxis1);
}

QVector3D SwingTwistConstraint::planeAxis2() const
{
    return m_planeAxis2;
}

void SwingTwistConstraint::setPlaneAxis2(const QVector3D &planeAxis2)
{
    if (m_planeAxis2 == planeAxis2)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'planeAxis2' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_planeAxis2 = planeAxis2;
    emit planeAxis2Changed(m_planeAxis1);
}

SwingTwistConstraint::SwingType SwingTwistConstraint::swingType() const
{
    return static_cast<SwingType>(m_constraintSettings.mSwingType);
}

void SwingTwistConstraint::setSwingType(SwingType swingType)
{
    if (static_cast<SwingType>(m_constraintSettings.mSwingType) == swingType)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'swingType' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_constraintSettings.mSwingType = static_cast<JPH::ESwingType>(swingType);
    emit swingTypeChanged(swingType);
}

float SwingTwistConstraint::normalHalfConeAngle() const
{
    return m_normalHalfConeAngle;
}

void SwingTwistConstraint::setNormalHalfConeAngle(float normalHalfConeAngle)
{
    if (qFuzzyCompare(m_normalHalfConeAngle, normalHalfConeAngle))
        return;

    if (m_constraint)
        static_cast<JPH::SwingTwistConstraint *>(m_constraint)->SetNormalHalfConeAngle(qDegreesToRadians(normalHalfConeAngle));

    m_normalHalfConeAngle = normalHalfConeAngle;
    emit normalHalfConeAngleChanged(normalHalfConeAngle);
}

float SwingTwistConstraint::planeHalfConeAngle() const
{
    return m_planeHalfConeAngle;
}

void SwingTwistConstraint::setPlaneHalfConeAngle(float planeHalfConeAngle)
{
    if (qFuzzyCompare(m_planeHalfConeAngle, planeHalfConeAngle))
        return;

    if (m_constraint)
        static_cast<JPH::SwingTwistConstraint *>(m_constraint)->SetPlaneHalfConeAngle(qDegreesToRadians(m_planeHalfConeAngle));

    m_planeHalfConeAngle = planeHalfConeAngle;
    emit planeHalfConeAngleChanged(planeHalfConeAngle);
}

float SwingTwistConstraint::twistMinAngle() const
{
    return m_twistMinAngle;
}

void SwingTwistConstraint::setTwistMinAngle(float twistMinAngle)
{
    if (qFuzzyCompare(m_twistMinAngle, twistMinAngle))
        return;

    if (m_constraint)
        static_cast<JPH::SwingTwistConstraint *>(m_constraint)->SetTwistMinAngle(qDegreesToRadians(m_twistMinAngle));

    m_twistMinAngle = twistMinAngle;
    emit twistMinAngleChanged(twistMinAngle);
}

float SwingTwistConstraint::twistMaxAngle() const
{
    return m_twistMaxAngle;
}

void SwingTwistConstraint::setTwistMaxAngle(float twistMaxAngle)
{
    if (qFuzzyCompare(m_twistMaxAngle, twistMaxAngle))
        return;

    if (m_constraint)
        static_cast<JPH::SwingTwistConstraint *>(m_constraint)->SetTwistMaxAngle(qDegreesToRadians(m_twistMaxAngle));

    m_twistMaxAngle = twistMaxAngle;
    emit twistMaxAngleChanged(twistMaxAngle);
}

float SwingTwistConstraint::maxFrictionTorque() const
{
    return m_constraintSettings.mMaxFrictionTorque;
}

void SwingTwistConstraint::setMaxFrictionTorque(float maxFrictionTorque)
{
    if (qFuzzyCompare(m_constraintSettings.mMaxFrictionTorque, maxFrictionTorque))
        return;

    if (m_constraint)
        static_cast<JPH::SwingTwistConstraint *>(m_constraint)->SetMaxFrictionTorque(maxFrictionTorque);

    m_constraintSettings.mMaxFrictionTorque = maxFrictionTorque;
    emit maxFrictionTorqueChanged(maxFrictionTorque);
}

void SwingTwistConstraint::updateJoltObject()
{
    if (m_jolt == nullptr || m_body1 == nullptr || m_body2 == nullptr || m_constraint)
        return;

    m_constraintSettings.mPosition1 = PhysicsUtils::toJoltType(m_position1);
    m_constraintSettings.mPosition2 = PhysicsUtils::toJoltType(m_position2);
    m_constraintSettings.mTwistAxis1 = PhysicsUtils::toJoltType(m_twistAxis1);
    m_constraintSettings.mTwistAxis2 = PhysicsUtils::toJoltType(m_twistAxis2);
    m_constraintSettings.mPlaneAxis1 = PhysicsUtils::toJoltType(m_planeAxis1);
    m_constraintSettings.mPlaneAxis2 = PhysicsUtils::toJoltType(m_planeAxis2);
    m_constraintSettings.mNormalHalfConeAngle = qDegreesToRadians(m_normalHalfConeAngle);
    m_constraintSettings.mPlaneHalfConeAngle = qDegreesToRadians(m_planeHalfConeAngle);
    m_constraintSettings.mTwistMinAngle = qDegreesToRadians(m_twistMinAngle);
    m_constraintSettings.mTwistMaxAngle = qDegreesToRadians(m_twistMaxAngle);

    m_constraint = m_constraintSettings.Create(*m_body1->m_body, *m_body2->m_body);
    m_jolt->AddConstraint(m_constraint);
}
