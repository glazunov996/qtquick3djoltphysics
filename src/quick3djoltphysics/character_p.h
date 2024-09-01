#ifndef CHARACTER_P_H
#define CHARACTER_P_H

#include "abstractphysicsbody_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQml/QQmlEngine>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Character/Character.h>

class Q_QUICK3DJOLTPHYSICS_EXPORT Character : public AbstractPhysicsBody
{
    Q_OBJECT
    Q_PROPERTY(unsigned int bodyID READ bodyID NOTIFY bodyIDChanged)
    Q_PROPERTY(QVector4D supportingVolume READ supportingVolume WRITE setSupportingVolume NOTIFY supportingVolumeChanged)
    Q_PROPERTY(float maxSlopeAngle READ maxSlopeAngle WRITE setMaxSlopeAngle NOTIFY maxSlopeAngleChanged)
    Q_PROPERTY(unsigned int layer READ layer WRITE setLayer NOTIFY layerChanged)
    Q_PROPERTY(float mass READ mass WRITE setMass NOTIFY massChanged)
    Q_PROPERTY(float friction READ friction WRITE setFriction NOTIFY frictionChanged)
    Q_PROPERTY(float gravityFactor READ gravityFactor WRITE setGravityFactor NOTIFY gravityFactorChanged)
    QML_NAMED_ELEMENT(Character)
public:
    explicit Character(QQuick3DNode *parent = nullptr);
    ~Character() override;

    enum class GroundState {
        OnGround,
        OnSteepGround,
        NotSupported,
        InAir
    };
    Q_ENUM(GroundState)

    int bodyID() const;
    QVector4D supportingVolume() const;
    void setSupportingVolume(const QVector4D &supportingVolume);
    float maxSlopeAngle() const;
    void setMaxSlopeAngle(float maxSlopeAngle);
    unsigned int layer() const;
    void setLayer(unsigned int layer);
    float mass() const;
    void setMass(float mass);
    float friction() const;
    void setFriction(float friction);
    float gravityFactor() const;
    void setGravityFactor(float gravityFactor);

    Q_INVOKABLE QVector3D getLinearVelocity() const;
    Q_INVOKABLE void setLinearVelocity(const QVector3D &linearVelocity);

    Q_INVOKABLE GroundState getGroundState() const;
    Q_INVOKABLE QVector3D getGroundNormal() const;

    Q_INVOKABLE void postSimulation(float maxSeparationDistance);
    Q_INVOKABLE void setShape(AbstractShape *shape, float maxPenetrationDepth);

signals:
    void bodyIDChanged(unsigned int bodyID);
    void supportingVolumeChanged(const QVector4D &supportingVolume);
    void maxSlopeAngleChanged(float maxSlopeAngle);
    void layerChanged(unsigned int layer);
    void massChanged(float mass);
    void frictionChanged(float friction);
    void gravityFactorChanged(float gravityFactor);

protected:
    void updateJoltObject() override;
    void cleanup() override;
    void sync() override;

private:
    QVector4D m_supportingVolume;
    float m_maxSlopeAngle = 50.0;

    float m_maxPenetrationDepth;
    bool m_updateShapeNeeded = false;

    int m_bodyID = -1;
    JPH::Character *m_character = nullptr;
    JPH::CharacterSettings m_characterSettings;
};

#endif // CHARACTER_P_H
