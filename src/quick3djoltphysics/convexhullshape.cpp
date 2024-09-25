#include "convexhullshape_p.h"
#include "physicsmeshutils_p_p.h"
#include "physicsutils_p.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/ConvexHullShape.h>
#include <Jolt/Physics/Collision/Shape/ScaledShape.h>
#include <Jolt/Physics/Collision/Shape/OffsetCenterOfMassShape.h>

ConvexHullShape::ConvexHullShape(QQuick3DNode *parent) : MeshShape(parent)
{
}

ConvexHullShape::~ConvexHullShape() = default;

void ConvexHullShape::updateJoltShape()
{
    if (m_mesh == nullptr)
        return;

    auto convexHullShapeSettings = m_mesh->convexHullShapeSettings();
    if (!convexHullShapeSettings)
        return;

    auto shapeResult = convexHullShapeSettings->Create();
    m_shape = shapeResult.Get();

    updateConvexShapeDensity();
    updateOffsetCenterOfMass();

    m_shape = new JPH::ScaledShape(m_shape, PhysicsUtils::toJoltType(sceneScale()));
}
