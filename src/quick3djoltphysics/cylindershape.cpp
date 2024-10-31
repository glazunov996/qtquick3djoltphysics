#include "cylindershape_p.h"
#include "physicsutils_p.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/CylinderShape.h>

CylinderShape::CylinderShape(QQuick3DNode *parent) : AbstractShape(parent)
{
}

CylinderShape::~CylinderShape() = default;

float CylinderShape::diameter() const
{
    return m_diameter;
}

void CylinderShape::setDiameter(float diameter)
{
    if (qFuzzyCompare(m_diameter, diameter))
        return;

    m_diameter = diameter;
    updateJoltShape();

    emit diameterChanged(m_diameter);
    emit changed();
}

float CylinderShape::height() const
{
    return m_height;
}

void CylinderShape::setHeight(float height)
{
    if (qFuzzyCompare(m_height, height))
        return;

    m_height = height;
    updateJoltShape();

    emit heightChanged(m_height);
    emit changed();
}

float CylinderShape::convexRadius() const
{
    return m_convexRadius;
}

void CylinderShape::setConvexRadius(float convexRadius)
{
    if (qFuzzyCompare(convexRadius, m_convexRadius))
        return;

    m_convexRadius = convexRadius;
    updateJoltShape();

    emit convexRadiusChanged(m_convexRadius);
    emit changed();
}

void CylinderShape::createJoltShape()
{
    m_shape = new JPH::CylinderShape(m_height * 0.5f, m_diameter * 0.5f, m_convexRadius);
}
