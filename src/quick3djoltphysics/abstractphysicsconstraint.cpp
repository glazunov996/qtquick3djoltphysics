#include "abstractphysicsconstraint_p.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>

AbstractPhysicsConstraint::AbstractPhysicsConstraint(QQuick3DNode *parent) : AbstractPhysicsNode(parent)
{
}

AbstractPhysicsConstraint::~AbstractPhysicsConstraint()
{
    AbstractPhysicsConstraint::cleanup();
}

void AbstractPhysicsConstraint::cleanup()
{
    if (m_jolt && m_constraint)
        m_jolt->RemoveConstraint(m_constraint);

    m_constraint = nullptr;
}
