#include "boxshape_p.h"

#include "physicsutils_p.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>

BoxShape::BoxShape(QQuick3DNode *parent) : AbstractShape(parent)
{
}

BoxShape::~BoxShape() = default;

QVector3D BoxShape::extents() const
{
    return m_extents;
}

void BoxShape::setExtents(const QVector3D &extents)
{
    if (m_extents == extents)
        return;

    m_extents = extents;
    updateJoltShape();

    emit extentsChanged(m_extents);
    emit changed();
}

float BoxShape::convexRadius() const
{
    return m_convexRadius;
}

void BoxShape::setConvexRadius(float convexRadius)
{
    if (qFuzzyCompare(convexRadius, m_convexRadius))
        return;

    m_convexRadius = convexRadius;
    updateJoltShape();

    emit convexRadiusChanged(m_convexRadius);
    emit changed();
}

void BoxShape::createJoltShape()
{
    m_joltShape = new JPH::BoxShape(PhysicsUtils::toJoltType(m_extents * 0.5f), m_convexRadius);
}
