#include "convexhullshape_p.h"
#include "physicsmeshutils_p_p.h"
#include "physicsutils_p.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/ConvexHullShape.h>
#include <Jolt/Physics/Collision/Shape/ScaledShape.h>
#include <Jolt/Physics/Collision/Shape/OffsetCenterOfMassShape.h>
#include <Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h>

ConvexHullShape::ConvexHullShape(QQuick3DNode *parent) : MeshShape(parent)
{
}

ConvexHullShape::~ConvexHullShape() = default;

void ConvexHullShape::createJoltShape()
{
    if (m_mesh == nullptr)
        return;

    auto convexHullShapeSettings = m_mesh->convexHullShapeSettings();
    if (!convexHullShapeSettings)
        return;

    auto shapeResult = convexHullShapeSettings->Create();
    m_shape = shapeResult.Get();
}
