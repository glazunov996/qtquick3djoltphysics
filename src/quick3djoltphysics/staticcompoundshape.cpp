#include "staticcompoundshape_p.h"
#include "physicsutils_p.h"
#include <QtQuick3D/private/qquick3dobject_p.h>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/StaticCompoundShape.h>
#include <Jolt/Physics/Collision/Shape/ScaledShape.h>
#include <Jolt/Physics/Collision/Shape/OffsetCenterOfMassShape.h>

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

void StaticCompoundShape::updateJoltShape()
{
    JPH::StaticCompoundShapeSettings staticCompoundSettings;
    for (auto *shape : std::as_const(m_shapes)) {
        staticCompoundSettings.AddShape(
            PhysicsUtils::toJoltType(shape->position()), PhysicsUtils::toJoltType(shape->rotation()), shape->getJoltShape());
    }

    auto shapeResult = staticCompoundSettings.Create();
    m_shape = shapeResult.Get();
    m_shape = new JPH::OffsetCenterOfMassShape(m_shape, PhysicsUtils::toJoltType(m_offsetCenterOfMass));
    m_shape = new JPH::ScaledShape(m_shape, PhysicsUtils::toJoltType(sceneScale()));
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

    if (shape->parentItem() == nullptr) {
        // If the material has no parent, check if it has a hierarchical parent that's a
        // QQuick3DObject and re-parent it to that, e.g., inline materials
        QQuick3DObject *parentItem = qobject_cast<QQuick3DObject *>(shape->parent());
        if (parentItem) {
            shape->setParentItem(parentItem);
        } else {
            const auto &scenManager = QQuick3DObjectPrivate::get(self)->sceneManager;
            if (scenManager)
                QQuick3DObjectPrivate::refSceneManager(shape, *scenManager);
            // else: If there's no scene manager, defer until one is set, see itemChange()
        }
    }

    emit self->changed();
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
    for (auto *shape : std::as_const(self->m_shapes)) {
        if (shape->parentItem() == nullptr)
            QQuick3DObjectPrivate::get(shape)->derefSceneManager();
    }
    self->m_shapes.clear();

    emit self->changed();
}
