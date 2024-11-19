#include "sphereshape_p.h"
#include "physicsutils_p.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>

SphereShape::SphereShape(QQuick3DNode *parent) : AbstractShape(parent)
{
}

SphereShape::~SphereShape() = default;

float SphereShape::diameter() const
{
    return m_diameter;
}

void SphereShape::setDiameter(float diameter)
{
    if (qFuzzyCompare(m_diameter, diameter))
        return;

    m_diameter = diameter;
    updateJoltShape();

    emit diameterChanged(m_diameter);
    emit changed();
}

void SphereShape::createJoltShape()
{
    m_joltShape = new JPH::SphereShape(m_diameter * 0.5f);
}
