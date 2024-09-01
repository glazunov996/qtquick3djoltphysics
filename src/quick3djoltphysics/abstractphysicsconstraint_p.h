#ifndef ABSTRACTCONSTRAINT_P_H
#define ABSTRACTCONSTRAINT_P_H

#include "abstractphysicsnode_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQuick3D/private/qquick3dnode_p.h>

#include <QtQml/QQmlEngine>
#include <QObject>

namespace JPH {
class Constraint;
}

class Q_QUICK3DJOLTPHYSICS_EXPORT AbstractPhysicsConstraint : public AbstractPhysicsNode
{
    Q_OBJECT
    QML_NAMED_ELEMENT(PhysicsConstraint)
    QML_UNCREATABLE("abstract interface")
public:
    explicit AbstractPhysicsConstraint(QQuick3DNode *parent = nullptr);
    ~AbstractPhysicsConstraint() override;

protected:
    void cleanup() override;

    JPH::Constraint *m_constraint = nullptr;
};

#endif // ABSTRACTCONSTRAINT_P_H
