#include "abstractshape_p.h"

#include <Jolt/Physics/Collision/Shape/ConvexShape.h>

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
    if (m_shape == nullptr)
        updateJoltShape();

    if (m_shape->GetType() == JPH::EShapeType::Convex) {
        auto *convexShape = reinterpret_cast<JPH::ConvexShape *>(m_shape.GetPtr());
        convexShape->SetDensity(m_density);
    } else {
        qWarning() << "Density could be set only for convex shape";
    }

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

void AbstractShape::handleScaleChanged()
{
    updateJoltShape();

    emit changed();
}

JPH::Ref<JPH::Shape> AbstractShape::getJoltShape()
{
    if (m_shape == nullptr)
        updateJoltShape();

    return m_shape;
}
