#include "cylindershape_p.h"
#include "physicsutils_p.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/CylinderShape.h>
#include <Jolt/Physics/Collision/Shape/ScaledShape.h>
#include <Jolt/Physics/Collision/Shape/OffsetCenterOfMassShape.h>

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

void CylinderShape::updateJoltShape()
{
    JPH::CylinderShapeSettings cylinderSettings(m_height * 0.5, m_diameter * 0.5);
    auto shapeResult = cylinderSettings.Create();
    m_shape = shapeResult.Get();
    m_shape = new JPH::OffsetCenterOfMassShape(m_shape, PhysicsUtils::toJoltType(offsetCenterOfMass()));
    m_shape = new JPH::ScaledShape(m_shape, PhysicsUtils::toJoltType(sceneScale()));
}
