#include "abstractphysicscharacter_p.h"
#include "physicsutils_p.h"



AbstractPhysicsCharacter::AbstractPhysicsCharacter(QQuick3DNode *parent) : AbstractPhysicsBody(parent)
{
}

AbstractPhysicsCharacter::~AbstractPhysicsCharacter() = default;
