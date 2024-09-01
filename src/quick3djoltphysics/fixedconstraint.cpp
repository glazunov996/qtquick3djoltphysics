#include "fixedconstraint_p.h"
#include "physicsutils_p.h"

#include <Jolt/Physics/PhysicsSystem.h>

FixedConstraint::FixedConstraint(QQuick3DNode *parent) : AbstractPhysicsConstraint(parent)
{
}

FixedConstraint::~FixedConstraint() = default;

QVector3D FixedConstraint::point1() const
{
    return m_point1;
}

void FixedConstraint::setPoint1(const QVector3D &point)
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

QVector3D FixedConstraint::point2() const
{
    return m_point2;
}

void FixedConstraint::setPoint2(const QVector3D &point)
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

Body *FixedConstraint::body1() const
{
    return m_body1;
}

void FixedConstraint::setBody1(Body *body)
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

Body *FixedConstraint::body2() const
{
    return m_body2;
}

void FixedConstraint::setBody2(Body *body)
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

QVector3D FixedConstraint::axisX1() const
{
    return m_axisX1;
}

void FixedConstraint::setAxisX1(const QVector3D &axisX1)
{
    if (m_axisX1 == axisX1)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'axisX1' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_axisX1 = axisX1;
    emit axisX1Changed(m_axisX1);
}

QVector3D FixedConstraint::axisX2() const
{
    return m_axisX2;
}

void FixedConstraint::setAxisX2(const QVector3D &axisX2)
{
    if (m_axisX2 == axisX2)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'axisX2' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_axisX2 = axisX2;
    emit axisX2Changed(m_axisX2);
}

QVector3D FixedConstraint::axisY1() const
{
    return m_axisY1;
}

void FixedConstraint::setAxisY1(const QVector3D &axisY1)
{
    if (m_axisY1 == axisY1)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'axisY1' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_axisY1 = axisY1;
    emit axisY1Changed(m_axisY1);
}

QVector3D FixedConstraint::axisY2() const
{
    return m_axisY2;
}

void FixedConstraint::setAxisY2(const QVector3D &axisY2)
{
    if (m_axisY2 == axisY2)
        return;

    if (m_constraint) {
        qWarning() << "Warning: Changing 'axisY2' after constraint is initialized will have "
                      "no effect";
        return;
    }

    m_axisY2 = axisY2;
    emit axisY2Changed(m_axisY2);
}

void FixedConstraint::updateJoltObject()
{
    if (m_jolt == nullptr || m_body1 == nullptr || m_body2 == nullptr || m_constraint)
        return;

    m_constraintSettings.mPoint1 = PhysicsUtils::toJoltType(m_point1);
    m_constraintSettings.mPoint2 = PhysicsUtils::toJoltType(m_point2);
    m_constraintSettings.mAxisX1 = PhysicsUtils::toJoltType(m_axisX1);
    m_constraintSettings.mAxisX2 = PhysicsUtils::toJoltType(m_axisX2);
    m_constraintSettings.mAxisY1 = PhysicsUtils::toJoltType(m_axisY1);
    m_constraintSettings.mAxisY2 = PhysicsUtils::toJoltType(m_axisY2);

    m_constraint = m_constraintSettings.Create(*m_body1->m_body, *m_body2->m_body);
    m_jolt->AddConstraint(m_constraint);
}
