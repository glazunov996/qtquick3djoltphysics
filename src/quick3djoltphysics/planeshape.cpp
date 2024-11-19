#include "planeshape_p.h"
#include "physicsutils_p.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/PlaneShape.h>

PlaneShape::PlaneShape(QQuick3DNode *parent) : AbstractShape(parent)
{
}

PlaneShape::~PlaneShape() = default;

float PlaneShape::extent() const
{
    return m_extent;
}

void PlaneShape::setExtent(float extent)
{
    if (qFuzzyCompare(m_extent, extent))
        return;

    m_extent = extent;
    updateJoltShape();

    emit extentChanged(m_extent);
    emit changed();
}

void PlaneShape::createJoltShape()
{
    m_joltShape = new JPH::PlaneShape(JPH::Plane(JPH::Vec3(0.0f, 0.0f, 1.0f), 0.0f), nullptr, m_extent * 0.5f);
}
