#ifndef PHYSICSBODY_P_H
#define PHYSICSBODY_P_H

#include "abstractphysicsbody_p.h"
#include "collisiongroup_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQuick3D/private/qquick3dnode_p.h>
#include <QtQml/QQmlEngine>

#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/EActivation.h>

namespace JPH {
class PhysicsSystem;
class Body;
}

class Q_QUICK3DJOLTPHYSICS_EXPORT Body : public AbstractPhysicsBody
{
    Q_OBJECT
    Q_PROPERTY(int bodyID READ bodyID NOTIFY bodyIDChanged)
    Q_PROPERTY(CollisionGroup *collisionGroup READ collisionGroup WRITE setCollisionGroup NOTIFY collisionGroupChanged)
    Q_PROPERTY(MotionType motionType READ motionType WRITE setMotionType NOTIFY motionTypeChanged)
    Q_PROPERTY(MotionQuality motionQuality READ motionQuality WRITE setMotionQuality NOTIFY motionQualityChanged)
    Q_PROPERTY(int objectLayer READ objectLayer WRITE setObjectLayer NOTIFY objectLayerChanged)
    Q_PROPERTY(bool usedInSimulation READ usedInSimulation WRITE setUsedInSimulation NOTIFY usedInSimulationChanged)
    Q_PROPERTY(bool allowSleeping READ allowSleeping WRITE setAllowSleeping NOTIFY allowSleepingChanged)
    Q_PROPERTY(AllowedDOFs allowedDOFs READ allowedDOFs WRITE setAllowedDOFs NOTIFY allowedDOFsChanged)
    Q_PROPERTY(OverrideMassProperties overrideMassProperties READ overrideMassProperties WRITE setOverrideMassProperties NOTIFY overrideMassPropertiesChanged)
    Q_PROPERTY(float mass READ mass WRITE setMass NOTIFY massChanged)
    Q_PROPERTY(QMatrix4x4 inertia READ inertia WRITE setInertia NOTIFY inertiaChanged)
    Q_PROPERTY(float friction READ friction WRITE setFriction NOTIFY frictionChanged)
    Q_PROPERTY(float restitution READ restitution WRITE setRestitution NOTIFY restitutionChanged)
    Q_PROPERTY(float linearDamping READ linearDamping WRITE setLinearDamping NOTIFY linearDampingChanged)
    Q_PROPERTY(float angularDamping READ angularDamping WRITE setAngularDamping NOTIFY angularDampingChanged)
    Q_PROPERTY(float gravityFactor READ gravityFactor WRITE setGravityFactor NOTIFY gravityFactorChanged)
    QML_NAMED_ELEMENT(Body)
public:
    explicit Body(QQuick3DNode *parent = nullptr);
    ~Body() override;

    enum class MotionType {
        Static,
        Kinematic,
        Dynamic,
    };
    Q_ENUM(MotionType)

    enum class MotionQuality {
        Discrete,
        LinearCast,
    };
    Q_ENUM(MotionQuality)

    enum class AllowedDOFs {
        None = 0b000000,
        All = 0b111111,
        TranslationX = 0b000001,
        TranslationY = 0b000010,
        TranslationZ = 0b000100,
        RotationX = 0b001000,
        RotationY = 0b010000,
        RotationZ = 0b100000,
        Plane2D = TranslationX | TranslationY | RotationZ,
    };
    Q_ENUM(AllowedDOFs)

    enum class OverrideMassProperties {
        CalculateMassAndInertia,
        CalculateInertia,
        MassAndInertiaProvided
    };
    Q_ENUM(OverrideMassProperties)

    int bodyID() const;
    CollisionGroup *collisionGroup() const;
    void setCollisionGroup(CollisionGroup *collisionGroup);
    MotionType motionType() const;
    void setMotionType(MotionType motionType);
    MotionQuality motionQuality() const;
    void setMotionQuality(MotionQuality motionQuality);
    int objectLayer() const;
    void setObjectLayer(int objectLayer);
    bool usedInSimulation() const;
    void setUsedInSimulation(bool usedInSimulation);
    bool allowSleeping() const;
    void setAllowSleeping(bool allowSleeping);

    AllowedDOFs allowedDOFs() const;
    void setAllowedDOFs(AllowedDOFs allowedDOFs);
    OverrideMassProperties overrideMassProperties() const;
    void setOverrideMassProperties(OverrideMassProperties overrideMassProperties);
    float mass() const;
    void setMass(float mass);
    QMatrix4x4 inertia() const;
    void setInertia(const QMatrix4x4 &inertia);

    float friction() const;
    void setFriction(float friction);
    float restitution() const;
    void setRestitution(float restitution);
    float linearDamping() const;
    void setLinearDamping(float linearDamping);
    float angularDamping() const;
    void setAngularDamping(float angularDamping);
    float gravityFactor() const;
    void setGravityFactor(float gravityFactor);

    Q_INVOKABLE QVector3D getLinearVelocity() const;
    Q_INVOKABLE void setLinearVelocity(const QVector3D &linearVelocity);
    Q_INVOKABLE QVector3D getAngularVelocity() const;
    Q_INVOKABLE void setAngularVelocity(const QVector3D &angularVelocity);

    Q_INVOKABLE QVector3D getAccumulatedForce() const;
    Q_INVOKABLE QVector3D getAccumulatedTorque() const;

    Q_INVOKABLE float getInverseMass() const;
    Q_INVOKABLE QVector3D getCenterOfMassPosition() const;

    Q_INVOKABLE void activate();
    Q_INVOKABLE void deactivate();

    Q_INVOKABLE void addForce(const QVector3D &force);
    Q_INVOKABLE void addForce(const QVector3D &force, const QVector3D &position);
    Q_INVOKABLE void addTorque(const QVector3D &torque);

    Q_INVOKABLE void addImpulse(const QVector3D &impulse);
    Q_INVOKABLE void addImpulse(const QVector3D &impulse, const QVector3D &position);
    Q_INVOKABLE void addAngularImpulse(const QVector3D &angularImpulse);

    Q_INVOKABLE void moveKinematic(const QVector3D &targetPosition, const QQuaternion &targetRotation, float deltaTime);

signals:
    void bodyIDChanged(int bodyID);
    void collisionGroupChanged(CollisionGroup *collisionGroup);
    void motionTypeChanged(MotionType motionType);
    void motionQualityChanged(MotionQuality motionQuality);
    void objectLayerChanged(int objectLayer);
    void usedInSimulationChanged(bool usedInSimulation);
    void allowSleepingChanged(bool allowSleeping);

    void allowedDOFsChanged(AllowedDOFs allowedDOFs);
    void overrideMassPropertiesChanged(OverrideMassProperties overrideMassProperties);
    void massChanged(float mass);
    void inertiaChanged(const QMatrix4x4 &inertia);

    void frictionChanged(float friction);
    void restitutionChanged(float restitution);
    void linearDampingChanged(float linearDamping);
    void angularDampingChanged(float angularDamping);
    void gravityFactorChanged(float gravityFactor);

protected:
    void updateJoltObject() override;
    void cleanup() override;
    void sync() override;

    virtual bool isSensor() const { return false; }

private slots:
    void handleScenePositionChanged();
    void handleSceneRotationChanged();

private:
    JPH::EActivation getActivationForMotionType() const;

    CollisionGroup *m_collisionGroup = nullptr;
    QMetaObject::Connection m_collisionGroupSignalConnection;
    MotionType m_motionType = MotionType::Static;
    MotionQuality m_motionQuality = MotionQuality::Discrete;

    AllowedDOFs m_allowedDOFs = AllowedDOFs::All;
    OverrideMassProperties m_overrideMassProperties = OverrideMassProperties::CalculateMassAndInertia;
    float m_mass = 0.0f;
    QMatrix4x4 m_inertia;

    bool m_usedInSimulation = true;

    int m_bodyID = -1;
    JPH::Body *m_body = nullptr;
    JPH::BodyCreationSettings m_bodySettings;

    friend class PointConstraint;
    friend class DistanceConstraint;
    friend class HingeConstraint;
    friend class SwingTwistConstraint;
    friend class FixedConstraint;
};

#endif // PHYSICSBODY_P_H
