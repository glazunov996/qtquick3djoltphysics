#ifndef ABSTRACTPHYSICSNODE_P_H
#define ABSTRACTPHYSICSNODE_P_H

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQuick3D/private/qquick3dnode_p.h>

namespace JPH {
class PhysicsSystem;
class TempAllocator;
}

class PhysicsSystem;

class Q_QUICK3DJOLTPHYSICS_EXPORT AbstractPhysicsNode : public QQuick3DNode
{
    Q_OBJECT
    QML_NAMED_ELEMENT(PhysicsNode)
    QML_UNCREATABLE("abstract interface")
public:
    explicit AbstractPhysicsNode(QQuick3DNode *parent = nullptr);
    ~AbstractPhysicsNode() override;

    void componentComplete() override;

protected:
    virtual void updateJoltObject() = 0;
    virtual void cleanup() {}
    virtual void sync() {}

    JPH::PhysicsSystem *m_jolt = nullptr;
    JPH::TempAllocator *m_tempAllocator = nullptr;

private:
    void init(JPH::PhysicsSystem *jolt, JPH::TempAllocator *tempAllocator);
    friend class PhysicsSystem;
};

#endif // ABSTRACTPHYSICSNODE_P_H
