#include "abstractphysicsnode_p.h"
#include "physicssystem_p.h"

AbstractPhysicsNode::AbstractPhysicsNode(QQuick3DNode *parent) : QQuick3DNode(parent)
{
}

AbstractPhysicsNode::~AbstractPhysicsNode()
{
    PhysicsSystem::unRegisterPhysicsNode(this);
}

void AbstractPhysicsNode::componentComplete()
{
    QQuick3DNode::componentComplete();

    PhysicsSystem::registerPhysicsNode(this);
}

void AbstractPhysicsNode::init(JPH::PhysicsSystem *jolt, JPH::TempAllocator *tempAllocator)
{
    Q_ASSERT(jolt);
    Q_ASSERT(tempAllocator);

    m_jolt = jolt;
    m_tempAllocator = tempAllocator;

    updateJoltObject();
}
