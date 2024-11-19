#include "rotatedtranslatedshape_p.h"
#include "physicsutils_p.h"

#include <QtQuick3D/private/qquick3dobject_p.h>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h>

RotatedTranslatedShape::RotatedTranslatedShape(QQuick3DNode *parent) : AbstractShape(parent)
{
}

RotatedTranslatedShape::~RotatedTranslatedShape() = default;

AbstractShape *RotatedTranslatedShape::shape() const
{
    return m_shape;
}

void RotatedTranslatedShape::setShape(AbstractShape *shape)
{
    if (m_shape == shape)
        return;

    QQuick3DObjectPrivate::attachWatcher(this, &RotatedTranslatedShape::setShape, shape, m_shape);
    if (m_shape != nullptr)
        m_shape->disconnect(m_shapeSignalConnection);
    m_shape = shape;
    if (m_shape) {
        m_shapeSignalConnection = QObject::connect(m_shape, &AbstractShape::changed, this,
                                                   [this] { handleShapeChange(); });
    }

    handleShapeChange();
}

void RotatedTranslatedShape::createJoltShape()
{
    if (m_shape == nullptr)
        return;

    m_joltShape = new JPH::RotatedTranslatedShape(PhysicsUtils::toJoltType(position()),
                                                  PhysicsUtils::toJoltType(rotation()),
                                                  m_shape->getJoltShape());
}
