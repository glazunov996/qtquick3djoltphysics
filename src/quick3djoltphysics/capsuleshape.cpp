#include "capsuleshape_p.h"
#include "physicsutils_p.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>

CapsuleShape::CapsuleShape(QQuick3DNode *parent) : AbstractShape(parent)
{
}

CapsuleShape::~CapsuleShape() = default;

float CapsuleShape::diameter() const
{
    return m_diameter;
}

void CapsuleShape::setDiameter(float diameter)
{
    if (qFuzzyCompare(m_diameter, diameter))
        return;

    m_diameter = diameter;
    updateJoltShape();

    emit diameterChanged(m_diameter);
    emit changed();
}

float CapsuleShape::height() const
{
    return m_height;
}

void CapsuleShape::setHeight(float height)
{
    if (qFuzzyCompare(m_height, height))
        return;

    m_height = height;
    updateJoltShape();

    emit heightChanged(m_height);
    emit changed();
}

void CapsuleShape::createJoltShape()
{
    m_joltShape = new JPH::CapsuleShape(m_height * 0.5f, m_diameter * 0.5f);
}
