#include "emptyshape_p.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/EmptyShape.h>

EmptyShape::EmptyShape(QQuick3DNode *parent) : AbstractShape(parent)
{
}

EmptyShape::~EmptyShape() = default;

void EmptyShape::createJoltShape()
{
    m_shape = new JPH::EmptyShape();
}
