#include "hingeconstraint_p.h"
#include "physicsutils_p.h"

#include <Jolt/Physics/PhysicsSystem.h>

HingeConstraint::HingeConstraint(QQuick3DNode *parent) : AbstractPhysicsConstraint(parent)
{
}

HingeConstraint::~HingeConstraint() = default;

QVector3D HingeConstraint::point1() const
{
    return m_point1;
}

void HingeConstraint::setPoint1(const QVector3D &point)
{
    if (m_point1 == point)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'point1' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_point1 = point;
    emit point1Changed(m_point1);
}

QVector3D HingeConstraint::point2() const
{
    return m_point2;
}

void HingeConstraint::setPoint2(const QVector3D &point)
{
    if (m_point2 == point)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'point2' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_point2 = point;
    emit point2Changed(m_point2);
}

Body *HingeConstraint::body1() const
{
    return m_body1;
}

void HingeConstraint::setBody1(Body *body)
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

Body *HingeConstraint::body2() const
{
    return m_body2;
}

void HingeConstraint::setBody2(Body *body)
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

QVector3D HingeConstraint::hingeAxis1() const
{
    return m_hingeAxis1;
}

void HingeConstraint::setHingeAxis1(const QVector3D &hingeAxis1)
{
    if (m_hingeAxis1 == hingeAxis1)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'hingeAxis1' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_hingeAxis1 = hingeAxis1;
    emit hingeAxis1Changed(m_hingeAxis1);
}

QVector3D HingeConstraint::hingeAxis2() const
{
    return m_hingeAxis2;
}

void HingeConstraint::setHingeAxis2(const QVector3D &hingeAxis2)
{
    if (m_hingeAxis2 == hingeAxis2)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'hingeAxis2' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_hingeAxis2 = hingeAxis2;
    emit hingeAxis2Changed(m_hingeAxis2);
}

QVector3D HingeConstraint::normalAxis1() const
{
    return m_normalAxis1;
}

void HingeConstraint::setNormalAxis1(const QVector3D &normalAxis1)
{
    if (m_normalAxis1 == normalAxis1)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'normalAxis1' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_normalAxis1 = normalAxis1;
    emit normalAxis1Changed(m_normalAxis1);
}

QVector3D HingeConstraint::normalAxis2() const
{
    return m_normalAxis2;
}

void HingeConstraint::setNormalAxis2(const QVector3D &normalAxis2)
{
    if (m_normalAxis2 == normalAxis2)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'normalAxis2' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_normalAxis2 = normalAxis2;
    emit normalAxis1Changed(m_normalAxis2);
}

float HingeConstraint::limitsMin() const
{
    return m_limitsMin;
}

void HingeConstraint::setLimitsMin(float limitsMin)
{
    if (qFuzzyCompare(m_limitsMin, limitsMin))
        return;

    m_limitsMin = limitsMin;
    if (m_constraint)
        static_cast<JPH::HingeConstraint *>(m_constraint)->SetLimits(m_limitsMin, m_limitsMax);

    emit limitsMinChanged(m_limitsMin);
}

float HingeConstraint::limitsMax() const
{
    return m_limitsMax;
}

void HingeConstraint::setLimitsMax(float limitsMax)
{
    if (qFuzzyCompare(m_limitsMax, limitsMax))
        return;

    m_limitsMax = limitsMax;
    if (m_constraint)
        static_cast<JPH::HingeConstraint *>(m_constraint)->SetLimits(m_limitsMin, m_limitsMax);

    emit limitsMaxChanged(m_limitsMax);
}

SpringSettings *HingeConstraint::limitsSpringSettings() const
{
    return m_limitsSpringSettings;
}

void HingeConstraint::setLimitsSpringSettings(SpringSettings *limitsSpringSettings)
{
    if (m_limitsSpringSettings == limitsSpringSettings)
        return;

    if (m_limitsSpringSettings)
        m_limitsSpringSettings->disconnect(m_limitsSpringSettingsConnection);

    m_limitsSpringSettings = limitsSpringSettings;

    m_limitsSpringSettingsConnection = QObject::connect(m_limitsSpringSettings, &SpringSettings::changed, this,
                    [this]
    {
        if (m_constraint)
            static_cast<JPH::HingeConstraint *>(m_constraint)->SetLimitsSpringSettings(m_limitsSpringSettings->getJoltSpringSettings());
    });
    QObject::connect(m_limitsSpringSettings, &QObject::destroyed, this,
                    [this]
    {
        m_limitsSpringSettings = nullptr;
        if (m_constraint)
            static_cast<JPH::HingeConstraint *>(m_constraint)->SetLimitsSpringSettings(JPH::SpringSettings());
    });

    if (m_constraint)
        static_cast<JPH::HingeConstraint *>(m_constraint)->SetLimitsSpringSettings(m_limitsSpringSettings->getJoltSpringSettings());


    emit limitsSpringSettingsChanged(m_limitsSpringSettings);
}

float HingeConstraint::maxFrictionTorque() const
{
    return m_constraintSettings.mMaxFrictionTorque;
}

void HingeConstraint::setMaxFrictionTorque(float maxFrictionTorque)
{
    if (qFuzzyCompare(m_constraintSettings.mMaxFrictionTorque, maxFrictionTorque))
        return;

    m_constraintSettings.mMaxFrictionTorque = maxFrictionTorque;

    if (m_constraint)
        static_cast<JPH::HingeConstraint *>(m_constraint)->SetMaxFrictionTorque(maxFrictionTorque);

    emit maxFrictionTorqueChanged(maxFrictionTorque);
}

void HingeConstraint::updateJoltObject()
{
    if (m_jolt == nullptr || m_body1 == nullptr || m_body2 == nullptr || m_constraint)
        return;

    m_constraintSettings.mPoint1 = PhysicsUtils::toJoltType(m_point1);
    m_constraintSettings.mPoint2 = PhysicsUtils::toJoltType(m_point2);
    m_constraintSettings.mHingeAxis1 = PhysicsUtils::toJoltType(m_hingeAxis1);
    m_constraintSettings.mHingeAxis2 = PhysicsUtils::toJoltType(m_hingeAxis2);
    m_constraintSettings.mNormalAxis1 = PhysicsUtils::toJoltType(m_normalAxis1);
    m_constraintSettings.mNormalAxis2 = PhysicsUtils::toJoltType(m_normalAxis2);
    m_constraintSettings.mLimitsMin = qDegreesToRadians(m_limitsMin);
    m_constraintSettings.mLimitsMax = qDegreesToRadians(m_limitsMax);

    if (m_limitsSpringSettings)
        m_constraintSettings.mLimitsSpringSettings = m_limitsSpringSettings->getJoltSpringSettings();

    m_constraint = m_constraintSettings.Create(*m_body1->m_body, *m_body2->m_body);
    m_jolt->AddConstraint(m_constraint);
}
