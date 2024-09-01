#include "boxshape_p.h"

#include "physicsutils_p.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/ScaledShape.h>
#include <Jolt/Physics/Collision/Shape/OffsetCenterOfMassShape.h>

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

void BoxShape::updateJoltShape()
{
    m_shape = new JPH::OffsetCenterOfMassShape(new JPH::BoxShape(PhysicsUtils::toJoltType(m_extents * 0.5f)), PhysicsUtils::toJoltType(offsetCenterOfMass()));
    m_shape = new JPH::ScaledShape(m_shape, PhysicsUtils::toJoltType(sceneScale()));
}
