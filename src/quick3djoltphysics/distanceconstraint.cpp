#include "distanceconstraint_p.h"
#include "physicsutils_p.h"

#include <Jolt/Physics/PhysicsSystem.h>

DistanceConstraint::DistanceConstraint(QQuick3DNode *parent) : AbstractPhysicsConstraint(parent)
{
}

DistanceConstraint::~DistanceConstraint() = default;

QVector3D DistanceConstraint::point1() const
{
    return m_point1;
}

void DistanceConstraint::setPoint1(const QVector3D &point)
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

QVector3D DistanceConstraint::point2() const
{
    return m_point2;
}

void DistanceConstraint::setPoint2(const QVector3D &point)
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

Body *DistanceConstraint::body1() const
{
    return m_body1;
}

void DistanceConstraint::setBody1(Body *body)
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

Body *DistanceConstraint::body2() const
{
    return m_body2;
}

void DistanceConstraint::setBody2(Body *body)
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

float DistanceConstraint::minDistance() const
{
    return m_constraintSettings.mMinDistance;
}

void DistanceConstraint::setMinDistance(float minDistance)
{
    if (qFuzzyCompare(m_constraintSettings.mMinDistance, minDistance))
        return;

    m_constraintSettings.mMinDistance = minDistance;
    if (m_constraint)
        static_cast<JPH::DistanceConstraint *>(m_constraint)->SetDistance(m_constraintSettings.mMinDistance, m_constraintSettings.mMaxDistance);

    emit minDistanceChanged(minDistance);
}

float DistanceConstraint::maxDistance() const
{
    return m_constraintSettings.mMaxDistance;
}

void DistanceConstraint::setMaxDistance(float maxDistance)
{
    if (qFuzzyCompare(m_constraintSettings.mMaxDistance, maxDistance))
        return;

    m_constraintSettings.mMaxDistance = maxDistance;
    if (m_constraint)
        static_cast<JPH::DistanceConstraint *>(m_constraint)->SetDistance(m_constraintSettings.mMinDistance, m_constraintSettings.mMaxDistance);

    emit maxDistanceChanged(maxDistance);
}

SpringSettings *DistanceConstraint::limitsSpringSettings() const
{
    return m_limitsSpringSettings;
}

void DistanceConstraint::setLimitsSpringSettings(SpringSettings *limitsSpringSettings)
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
            static_cast<JPH::DistanceConstraint *>(m_constraint)->SetLimitsSpringSettings(m_limitsSpringSettings->getJoltSpringSettings());
    });
    QObject::connect(m_limitsSpringSettings, &QObject::destroyed, this,
                        [this]
    {
        m_limitsSpringSettings = nullptr;
        if (m_constraint)
            static_cast<JPH::DistanceConstraint *>(m_constraint)->SetLimitsSpringSettings(JPH::SpringSettings());
    });

    if (m_constraint)
        static_cast<JPH::DistanceConstraint *>(m_constraint)->SetLimitsSpringSettings(m_limitsSpringSettings->getJoltSpringSettings());

    emit limitsSpringSettingsChanged(m_limitsSpringSettings);
}

void DistanceConstraint::updateJoltObject()
{
    if (m_jolt == nullptr || m_body1 == nullptr || m_body2 == nullptr || m_constraint)
        return;

    m_constraintSettings.mPoint1 = PhysicsUtils::toJoltType(m_point1);
    m_constraintSettings.mPoint2 = PhysicsUtils::toJoltType(m_point2);

    if (m_limitsSpringSettings)
        m_constraintSettings.mLimitsSpringSettings = m_limitsSpringSettings->getJoltSpringSettings();

    m_constraint = m_constraintSettings.Create(*m_body1->m_body, *m_body2->m_body);
    m_jolt->AddConstraint(m_constraint);
}
