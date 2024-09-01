#include "sphereshape_p.h"
#include "physicsutils_p.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/ScaledShape.h>
#include <Jolt/Physics/Collision/Shape/OffsetCenterOfMassShape.h>

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

void SphereShape::updateJoltShape()
{
    JPH::SphereShapeSettings shapeSettings(m_diameter * 0.5f);
    auto shapeResult = shapeSettings.Create();
    m_shape = shapeResult.Get();
    m_shape = new JPH::OffsetCenterOfMassShape(m_shape, PhysicsUtils::toJoltType(offsetCenterOfMass()));
    m_shape = new JPH::ScaledShape(m_shape, PhysicsUtils::toJoltType(sceneScale()));
}
