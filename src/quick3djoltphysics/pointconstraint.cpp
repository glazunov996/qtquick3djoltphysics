#include "pointconstraint_p.h"
#include "physicsutils_p.h"

#include <Jolt/Physics/PhysicsSystem.h>

PointConstraint::PointConstraint(QQuick3DNode *parent) : AbstractPhysicsConstraint(parent)
{
}

PointConstraint::~PointConstraint() = default;

QVector3D PointConstraint::point1() const
{
    return m_point1;
}

void PointConstraint::setPoint1(const QVector3D &point)
{
    if (m_point1 == point)
        return;

    m_point1 = point;

    if (m_constraint) {
        auto *constraint = static_cast<JPH::PointConstraint *>(m_constraint);
        constraint->SetPoint1(JPH::EConstraintSpace::WorldSpace, PhysicsUtils::toJoltType(m_point1));
    }

    emit point1Changed(m_point1);
}

QVector3D PointConstraint::point2() const
{
    return m_point2;
}

void PointConstraint::setPoint2(const QVector3D &point)
{
    if (m_point2 == point)
        return;

    m_point2 = point;

    if (m_constraint) {
        auto *constraint = static_cast<JPH::PointConstraint *>(m_constraint);
        constraint->SetPoint2(JPH::EConstraintSpace::WorldSpace, PhysicsUtils::toJoltType(m_point2));
    }

    emit point2Changed(m_point2);
}

Body *PointConstraint::body1() const
{
    return m_body1;
}

void PointConstraint::setBody1(Body *body)
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

Body *PointConstraint::body2() const
{
    return m_body2;
}

void PointConstraint::setBody2(Body *body)
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

void PointConstraint::updateJoltObject()
{
    if (m_jolt == nullptr || m_body1 == nullptr || m_body2 == nullptr || m_constraint)
        return;

    m_constraintSettings.mPoint1 = PhysicsUtils::toJoltType(m_point1);
    m_constraintSettings.mPoint2 = PhysicsUtils::toJoltType(m_point2);
    m_constraint = m_constraintSettings.Create(*m_body1->m_body, *m_body2->m_body);
    m_jolt->AddConstraint(m_constraint);
}
