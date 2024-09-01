#ifndef ABSTRACTPHYSICSBODY_P_H
#define ABSTRACTPHYSICSBODY_P_H

#include "abstractshape_p.h"
#include "abstractphysicsnode_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQuick3D/private/qquick3dnode_p.h>
#include <QtQml/QQmlEngine>

class Q_QUICK3DJOLTPHYSICS_EXPORT AbstractPhysicsBody : public AbstractPhysicsNode
{
    Q_OBJECT
    Q_PROPERTY(AbstractShape *shape READ shape WRITE setShape NOTIFY shapeChanged)
    QML_NAMED_ELEMENT(PhysicsBody)
    QML_UNCREATABLE("abstract interface")
public:
    explicit AbstractPhysicsBody(QQuick3DNode *parent = nullptr);
    ~AbstractPhysicsBody() override;

    AbstractShape *shape() const;
    void setShape(AbstractShape *shape);

signals:
    void shapeChanged(AbstractShape *shape);

protected:
    AbstractShape *m_shape = nullptr;
    QMetaObject::Connection m_shapeSignalConnection;
    bool m_shapeDirty = false;
};

#endif // ABSTRACTPHYSICSBODY_P_H
