#include "abstractshape_p.h"
#include "physicsutils_p.h"

#include <QtQuick3D>

#include <Jolt/Physics/Collision/Shape/ConvexShape.h>
#include <Jolt/Physics/Collision/Shape/ScaledShape.h>
#include <Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h>

AbstractShape::AbstractShape(QQuick3DNode *parent) : QQuick3DNode(parent)
{
    connect(this, &QQuick3DNode::sceneScaleChanged, this, [this] { handleShapeChange(); });
}

AbstractShape::~AbstractShape() = default;

float AbstractShape::density() const
{
    return m_density;
}

void AbstractShape::setDensity(float density)
{
    if (qFuzzyCompare(m_density, density))
        return;

    m_density = density;
    updateJoltShapeDensity();

    emit densityChanged(m_density);
}

JPH::Ref<JPH::Shape> AbstractShape::getJoltShape()
{
    m_shapeInitialized = true;

    if (m_joltShape == nullptr)
        updateJoltShape();

    return m_joltShape;
}

void AbstractShape::updateJoltShape()
{
    if (!m_shapeInitialized)
        return;

    createJoltShape();

    Q_ASSERT(m_joltShape);

    if (m_joltShape->GetType() != JPH::EShapeType::Decorated) {
        m_innerJoltShape = m_joltShape;
        m_joltShape = new JPH::ScaledShape(m_innerJoltShape, PhysicsUtils::toJoltType(sceneScale()));
    }

    updateJoltShapeDensity();
}

void AbstractShape::handleShapeChange()
{
    updateJoltShape();
    emit changed();
}

void AbstractShape::updateJoltShapeDensity()
{
    if (m_innerJoltShape && m_innerJoltShape->GetType() == JPH::EShapeType::Convex) {
        auto *shape = reinterpret_cast<JPH::ConvexShape *>(m_innerJoltShape.GetPtr());
        shape->SetDensity(m_density);
    }
}
