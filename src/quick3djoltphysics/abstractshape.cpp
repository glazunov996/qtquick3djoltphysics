#include "abstractshape_p.h"
#include "physicsutils_p.h"

#include <QtQuick3D>

#include <Jolt/Physics/Collision/Shape/ConvexShape.h>
#include <Jolt/Physics/Collision/Shape/OffsetCenterOfMassShape.h>
#include <Jolt/Physics/Collision/Shape/ScaledShape.h>
#include <Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h>

AbstractShape::AbstractShape(QQuick3DNode *parent) : QQuick3DNode(parent)
{
    connect(this, &QQuick3DNode::positionChanged, this, &AbstractShape::handleNodeChanged);
    connect(this, &QQuick3DNode::rotationChanged, this, &AbstractShape::handleNodeChanged);
    connect(this, &QQuick3DNode::sceneScaleChanged, this, &AbstractShape::handleNodeChanged);
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

QVector3D AbstractShape::getCenterOfMass()
{
    const auto &shape = getJoltShape();
    return PhysicsUtils::toQtType(shape->GetCenterOfMass());
}

JPH::Ref<JPH::Shape> AbstractShape::getJoltShape()
{
    m_shapeInitialized = true;

    if (m_shape == nullptr)
        updateJoltShape();

    return m_shape;
}

void AbstractShape::updateJoltShape()
{
    if (!m_shapeInitialized)
        return;

    createJoltShape();

    Q_ASSERT(m_shape);

    if (m_shape->GetType() == JPH::EShapeType::Convex) {
        auto *convexShape = reinterpret_cast<JPH::ConvexShape *>(m_shape.GetPtr());
        convexShape->SetDensity(m_density);
    }

    if (!qFuzzyCompare(m_offsetCenterOfMass, QVector3D()))
        m_shape = new JPH::OffsetCenterOfMassShape(m_shape, PhysicsUtils::toJoltType(m_offsetCenterOfMass));

    if (!qFuzzyCompare(sceneScale(), QVector3D(1, 1, 1)))
        m_shape = new JPH::ScaledShape(m_shape, PhysicsUtils::toJoltType(sceneScale()));

    if (!qFuzzyCompare(position(), QVector3D()) || !qFuzzyCompare(rotation(), QQuaternion()))
        m_shape = new JPH::RotatedTranslatedShape(PhysicsUtils::toJoltType(position()), PhysicsUtils::toJoltType(rotation()), m_shape);
}

void AbstractShape::handleNodeChanged()
{
    updateJoltShape();

    emit changed();
}
