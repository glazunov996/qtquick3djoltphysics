#include "abstractphysicsbody_p.h"

#include <QtQuick3D/private/qquick3dobject_p.h>

AbstractPhysicsBody::AbstractPhysicsBody(QQuick3DNode *parent) : AbstractPhysicsNode(parent)
{
}

AbstractPhysicsBody::~AbstractPhysicsBody()
{
    if (m_shape != nullptr)
        m_shape->disconnect();
}

AbstractShape *AbstractPhysicsBody::shape() const
{
    return m_shape;
}

void AbstractPhysicsBody::setShape(AbstractShape *shape)
{
    if (m_shape == shape)
        return;

    if (m_shape != nullptr)
        m_shape->disconnect(m_shapeSignalConnection);

    m_shape = shape;

    if (m_shape->parentItem() == nullptr) {
        QQuick3DObject *parentItem = qobject_cast<QQuick3DObject *>(shape->parent());
        if (parentItem) {
            m_shape->setParentItem(parentItem);
        } else {
            const auto &sceneManager = QQuick3DObjectPrivate::get(this)->sceneManager;
            if (sceneManager)
                QQuick3DObjectPrivate::refSceneManager(m_shape, *sceneManager);
        }
    }

    m_shapeSignalConnection = QObject::connect(m_shape, &AbstractShape::changed, this,
                                               [this]
    {
        m_shapeDirty = true;
        updateJoltObject();
    });

    QObject::connect(m_shape, &QObject::destroyed, this,
                     [this]
    {
        if (m_shape->parentItem() == nullptr)
            QQuick3DObjectPrivate::get(m_shape)->derefSceneManager();
        m_shape = nullptr;
    });

    m_shapeDirty = true;
    updateJoltObject();

    emit shapeChanged(m_shape);
}
