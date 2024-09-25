#ifndef ABSTRACTPHYSICSCHARACTER_P_H
#define ABSTRACTPHYSICSCHARACTER_P_H

#include "abstractphysicsbody_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQml/QQmlEngine>

#include <Jolt/Jolt.h>

class Q_QUICK3DJOLTPHYSICS_EXPORT AbstractPhysicsCharacter : public AbstractPhysicsBody
{
    Q_OBJECT
    QML_NAMED_ELEMENT(PhysicsCharacter)
    QML_UNCREATABLE("abstract interface")
public:
    explicit AbstractPhysicsCharacter(QQuick3DNode *parent = nullptr);
    ~AbstractPhysicsCharacter() override;

    enum class GroundState {
        OnGround,
        OnSteepGround,
        NotSupported,
        InAir
    };
    Q_ENUM(GroundState)

protected:
    JPH::Ref<JPH::Shape> getRotatedTranslatedJoltShape();
};

#endif // ABSTRACTPHYSICSCHARACTER_P_H
