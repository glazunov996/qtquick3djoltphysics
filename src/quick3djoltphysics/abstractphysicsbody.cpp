#include "abstractphysicsbody_p.h"

#include <QtQuick3D/private/qquick3dobject_p.h>

AbstractPhysicsBody::AbstractPhysicsBody(QQuick3DNode *parent) : AbstractPhysicsNode(parent)
{
}

AbstractPhysicsBody::~AbstractPhysicsBody() = default;

AbstractShape *AbstractPhysicsBody::shape() const
{
    return m_shape;
}

void AbstractPhysicsBody::setShape(AbstractShape *shape)
{
    if (m_shape == shape)
        return;

    QQuick3DObjectPrivate::attachWatcher(this, &AbstractPhysicsBody::setShape, shape, m_shape);
    if (m_shape != nullptr)
        m_shape->disconnect(m_shapeSignalConnection);
    m_shape = shape;
    if (m_shape) {
        if (shape->parentItem() == nullptr) {
            QQuick3DObject *parentItem = qobject_cast<QQuick3DObject *>(shape->parent());
            m_shape->setParentItem(parentItem);
        }
        m_shapeSignalConnection = QObject::connect(m_shape, &AbstractShape::changed, this,
                                                   [this] { updateJoltObject(); });
    }

    updateJoltObject();
    emit shapeChanged(m_shape);
}

void AbstractPhysicsBody::cleanup()
{
    setShape(nullptr);
}
