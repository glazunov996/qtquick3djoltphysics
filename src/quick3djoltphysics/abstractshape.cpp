#include "abstractshape_p.h"
#include "physicsutils_p.h"

#include <Jolt/Physics/Collision/Shape/ConvexShape.h>
#include <Jolt/Physics/Collision/Shape/OffsetCenterOfMassShape.h>

AbstractShape::AbstractShape(QQuick3DNode *parent) : QQuick3DNode(parent)
{
    connect(this, &QQuick3DNode::sceneScaleChanged, this, &AbstractShape::handleScaleChanged);
}

AbstractShape::~AbstractShape() = default;

float AbstractShape::density() const
{
    return m_density;
}

void AbstractShape::setDensity(float density)
{
    if (qFuzzyCompare(density, m_density))
        return;

    m_density = density;
    updateJoltShape();

    emit densityChanged(m_density);
    emit changed();
}

QVector3D AbstractShape::offsetCenterOfMass() const
{
    return m_offsetCenterOfMass;
}

void AbstractShape::setOffsetCenterOfMass(const QVector3D &offsetCenterOfMass)
{
    if (m_offsetCenterOfMass == offsetCenterOfMass)
        return;

    m_offsetCenterOfMass = offsetCenterOfMass;
    updateJoltShape();

    emit offsetCenterOfMassChanged(m_offsetCenterOfMass);
    emit changed();
}

JPH::Ref<JPH::Shape> AbstractShape::getJoltShape()
{
    m_shapeInitialized = true;

    if (m_shape == nullptr)
        updateJoltShape();

    return m_shape;
}

void AbstractShape::updateConvexShapeDensity()
{
    if (m_shape == nullptr || m_shape->GetType() != JPH::EShapeType::Convex)
        return;

    auto *convexShape = reinterpret_cast<JPH::ConvexShape *>(m_shape.GetPtr());
    convexShape->SetDensity(m_density);
}

void AbstractShape::updateOffsetCenterOfMass()
{
    if (m_shape == nullptr || qFuzzyCompare(m_offsetCenterOfMass, QVector3D()))
        return;

    m_shape = new JPH::OffsetCenterOfMassShape(m_shape, PhysicsUtils::toJoltType(m_offsetCenterOfMass));
}

void AbstractShape::handleScaleChanged()
{
    updateJoltShape();

    emit changed();
}
