#include "abstractphysicscharacter_p.h"
#include "physicsutils_p.h"

#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/CylinderShape.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h>

AbstractPhysicsCharacter::AbstractPhysicsCharacter(QQuick3DNode *parent) : AbstractPhysicsBody(parent)
{
}

AbstractPhysicsCharacter::~AbstractPhysicsCharacter() = default;

JPH::Ref<JPH::Shape> AbstractPhysicsCharacter::getRotatedTranslatedJoltShape(AbstractShape *shape)
{
    if (shape == nullptr)
        return nullptr;

    const auto &joltShape = shape->getJoltShape();
    if (joltShape == nullptr)
        return nullptr;

    switch (joltShape->GetSubType()) {
    case JPH::EShapeSubType::Capsule: {
        const auto capsuleShape = reinterpret_cast<const JPH::CapsuleShape *>(joltShape.GetPtr());
        const auto halfHeight = capsuleShape->GetHalfHeightOfCylinder();
        const auto radius = capsuleShape->GetRadius();
        return new JPH::RotatedTranslatedShape(PhysicsUtils::toJoltType(QVector3D(0, halfHeight + radius, 0)), JPH::Quat::sIdentity(), joltShape);
    }
    case JPH::EShapeSubType::Cylinder: {
        const auto cylinderShape = reinterpret_cast<const JPH::CylinderShape *>(joltShape.GetPtr());
        const auto halfHeight = cylinderShape->GetHalfHeight();
        const auto radius = cylinderShape->GetRadius();
        return new JPH::RotatedTranslatedShape(PhysicsUtils::toJoltType(QVector3D(0, halfHeight + radius, 0)), JPH::Quat::sIdentity(), joltShape);
    }
    case JPH::EShapeSubType::Box: {
        const auto boxShape = reinterpret_cast<const JPH::BoxShape *>(joltShape.GetPtr());
        const auto halfExtent = boxShape->GetHalfExtent();
        return new JPH::RotatedTranslatedShape(PhysicsUtils::toJoltType(QVector3D(0, halfExtent.GetY(), 0)), JPH::Quat::sIdentity(), joltShape);
    }
    default:
        qWarning() << "AbstractPhysicsCharacter: invalid shape type.";
        return nullptr;
    }
}
