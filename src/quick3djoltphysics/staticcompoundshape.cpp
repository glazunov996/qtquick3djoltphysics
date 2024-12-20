#include "staticcompoundshape_p.h"
#include "physicsutils_p.h"
#include <QtQuick3D/private/qquick3dobject_p.h>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/StaticCompoundShape.h>

StaticCompoundShape::StaticCompoundShape(QQuick3DNode *parent) : AbstractShape(parent)
{
}

StaticCompoundShape::~StaticCompoundShape() = default;

QQmlListProperty<AbstractShape> StaticCompoundShape::shapes()
{
    return QQmlListProperty<AbstractShape>(
        this, nullptr, StaticCompoundShape::qmlAppendShape,
        StaticCompoundShape::qmlShapeCount, StaticCompoundShape::qmlShapeAt, nullptr);
}

const QVector<AbstractShape *> &StaticCompoundShape::getShapesList() const
{
    return m_shapes;
}

void StaticCompoundShape::createJoltShape()
{
    JPH::StaticCompoundShapeSettings staticCompoundSettings;
    for (auto *shape : std::as_const(m_shapes)) {
        staticCompoundSettings.AddShape(
            PhysicsUtils::toJoltType(shape->position()), PhysicsUtils::toJoltType(shape->rotation()), shape->getJoltShape());
    }

    auto shapeResult = staticCompoundSettings.Create();
    m_joltShape = shapeResult.Get();
}

void StaticCompoundShape::qmlAppendShape(QQmlListProperty<AbstractShape> *list, AbstractShape *shape)
{
    if (shape == nullptr)
        return;
    StaticCompoundShape *self = static_cast<StaticCompoundShape *>(list->object);
    self->m_shapes.push_back(shape);

    connect(shape, &QObject::destroyed, self, [self](QObject *shape) {
        self->m_shapes.removeAll(static_cast<AbstractShape *>(shape));
    });

    connect(shape, &AbstractShape::changed, self, [self] {
        self->handleShapeChange();
    });

    self->handleShapeChange();
}

AbstractShape *StaticCompoundShape::qmlShapeAt(QQmlListProperty<AbstractShape> *list, qsizetype index)
{
    StaticCompoundShape *self = static_cast<StaticCompoundShape *>(list->object);
    return self->m_shapes.at(index);
}

qsizetype StaticCompoundShape::qmlShapeCount(QQmlListProperty<AbstractShape> *list)
{
    StaticCompoundShape *self = static_cast<StaticCompoundShape *>(list->object);
    return self->m_shapes.count();
}

void StaticCompoundShape::qmlClearShapes(QQmlListProperty<AbstractShape> *list)
{
    StaticCompoundShape *self = static_cast<StaticCompoundShape *>(list->object);
    for (auto *shape : std::as_const(self->m_shapes))
        shape->disconnect(self);

    self->m_shapes.clear();
    self->handleShapeChange();
}
