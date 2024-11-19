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
    Q_PROPERTY(bool isSensor READ isSensor WRITE setIsSensor NOTIFY isSensorChanged)
    Q_PROPERTY(CollisionGroup *collisionGroup READ collisionGroup WRITE setCollisionGroup NOTIFY collisionGroupChanged)
    Q_PROPERTY(MotionType motionType READ motionType WRITE setMotionType NOTIFY motionTypeChanged)
    Q_PROPERTY(MotionQuality motionQuality READ motionQuality WRITE setMotionQuality NOTIFY motionQualityChanged)
    Q_PROPERTY(bool enhancedInternalEdgeRemoval READ enhancedInternalEdgeRemoval WRITE setEnhancedInternalEdgeRemoval NOTIFY enhancedInternalEdgeRemovalChanged)
    Q_PROPERTY(int objectLayer READ objectLayer WRITE setObjectLayer NOTIFY objectLayerChanged)
    Q_PROPERTY(bool usedInSimulation READ usedInSimulation WRITE setUsedInSimulation NOTIFY usedInSimulationChanged)
    Q_PROPERTY(bool allowSleeping READ allowSleeping WRITE setAllowSleeping NOTIFY allowSleepingChanged)
    Q_PROPERTY(AllowedDOFs allowedDOFs READ allowedDOFs WRITE setAllowedDOFs NOTIFY allowedDOFsChanged)
    Q_PROPERTY(OverrideMassProperties overrideMassProperties READ overrideMassProperties WRITE setOverrideMassProperties NOTIFY overrideMassPropertiesChanged)
    Q_PROPERTY(float mass READ mass WRITE setMass NOTIFY massChanged)
    Q_PROPERTY(QMatrix4x4 inertia READ inertia WRITE setInertia NOTIFY inertiaChanged)
    Q_PROPERTY(QVector3D offsetCenterOfMass READ offsetCenterOfMass WRITE setOffsetCenterOfMass NOTIFY offsetCenterOfMassChanged)
    Q_PROPERTY(float maxLinearVelocity READ maxLinearVelocity WRITE setMaxLinearVelocity NOTIFY maxLinearVelocityChanged)
    Q_PROPERTY(float maxAngularVelocity READ maxAngularVelocity WRITE setMaxAngularVelocity NOTIFY maxAngularVelocityChanged)
    Q_PROPERTY(float friction READ friction WRITE setFriction NOTIFY frictionChanged)
    Q_PROPERTY(float restitution READ restitution WRITE setRestitution NOTIFY restitutionChanged)
    Q_PROPERTY(float linearDamping READ linearDamping WRITE setLinearDamping NOTIFY linearDampingChanged)
    Q_PROPERTY(float angularDamping READ angularDamping WRITE setAngularDamping NOTIFY angularDampingChanged)
    Q_PROPERTY(float gravityFactor READ gravityFactor WRITE setGravityFactor NOTIFY gravityFactorChanged)
    Q_PROPERTY(QVector3D kinematicPosition READ kinematicPosition WRITE setKinematicPosition NOTIFY kinematicPositionChanged)
    Q_PROPERTY(QQuaternion kinematicRotation READ kinematicRotation WRITE setKinematicRotation NOTIFY kinematicRotationChanged)
    Q_PROPERTY(QVector3D kinematicEulerRotation READ kinematicEulerRotation WRITE setKinematicEulerRotation NOTIFY kinematicEulerRotationChanged)
    Q_PROPERTY(QVector3D kinematicPivot READ kinematicPivot WRITE setKinematicPivot NOTIFY kinematicPivotChanged)
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
    bool isSensor() const;
    void setIsSensor(bool isSensor);
    CollisionGroup *collisionGroup() const;
    void setCollisionGroup(CollisionGroup *collisionGroup);
    MotionType motionType() const;
    void setMotionType(MotionType motionType);
    MotionQuality motionQuality() const;
    void setMotionQuality(MotionQuality motionQuality);
    bool enhancedInternalEdgeRemoval() const;
    void setEnhancedInternalEdgeRemoval(bool enhancedInternalEdgeRemoval);
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
    QVector3D offsetCenterOfMass() const;
    void setOffsetCenterOfMass(const QVector3D &offsetCenterOfMass);

    float maxLinearVelocity() const;
    void setMaxLinearVelocity(float maxLinearVelocity);
    float maxAngularVelocity() const;
    void setMaxAngularVelocity(float maxAngularVelocity);
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

    QVector3D kinematicPosition() const;
    void setKinematicPosition(const QVector3D &position);
    QQuaternion kinematicRotation() const;
    void setKinematicRotation(const QQuaternion &rotation);
    QVector3D kinematicEulerRotation() const;
    void setKinematicEulerRotation(const QVector3D &eulerRotation);
    QVector3D kinematicPivot() const;
    void setKinematicPivot(const QVector3D &pivot);

    Q_INVOKABLE QVector3D getLinearVelocity() const;
    Q_INVOKABLE void setLinearVelocity(const QVector3D &linearVelocity);
    Q_INVOKABLE QVector3D getAngularVelocity() const;
    Q_INVOKABLE void setAngularVelocity(const QVector3D &angularVelocity);

    Q_INVOKABLE QVector3D getAccumulatedForce() const;
    Q_INVOKABLE QVector3D getAccumulatedTorque() const;

    Q_INVOKABLE float getInverseMass() const;
    Q_INVOKABLE QVector3D getCenterOfMassPosition() const;
    Q_INVOKABLE QVector3D getCenterOfMass() const;

    Q_INVOKABLE void activate();
    Q_INVOKABLE void deactivate();

    Q_INVOKABLE void addForce(const QVector3D &force);
    Q_INVOKABLE void addForce(const QVector3D &force, const QVector3D &position);
    Q_INVOKABLE void addTorque(const QVector3D &torque);

    Q_INVOKABLE void resetForce();
    Q_INVOKABLE void resetTorque();
    Q_INVOKABLE void resetMotion();

    Q_INVOKABLE void addImpulse(const QVector3D &impulse);
    Q_INVOKABLE void addImpulse(const QVector3D &impulse, const QVector3D &position);
    Q_INVOKABLE void addAngularImpulse(const QVector3D &angularImpulse);

    Q_INVOKABLE void reset(const QVector3D &position, const QVector3D &eulerRotation);

signals:
    void bodyIDChanged(int bodyID);
    void isSensorChanged(bool isSensor);
    void collisionGroupChanged(CollisionGroup *collisionGroup);
    void motionTypeChanged(MotionType motionType);
    void motionQualityChanged(MotionQuality motionQuality);
    void enhancedInternalEdgeRemovalChanged(bool enhancedInternalEdgeRemoval);
    void objectLayerChanged(int objectLayer);
    void usedInSimulationChanged(bool usedInSimulation);
    void allowSleepingChanged(bool allowSleeping);

    void allowedDOFsChanged(AllowedDOFs allowedDOFs);
    void overrideMassPropertiesChanged(OverrideMassProperties overrideMassProperties);
    void massChanged(float mass);
    void inertiaChanged(const QMatrix4x4 &inertia);
    void offsetCenterOfMassChanged(const QVector3D &offsetCenterOfMass);

    void maxLinearVelocityChanged(float maxLinearVelocity);
    void maxAngularVelocityChanged(float maxAngularVelocity);
    void frictionChanged(float friction);
    void restitutionChanged(float restitution);
    void linearDampingChanged(float linearDamping);
    void angularDampingChanged(float angularDamping);
    void gravityFactorChanged(float gravityFactor);

    void kinematicPositionChanged(const QVector3D &position);
    void kinematicRotationChanged(const QQuaternion &rotation);
    void kinematicEulerRotationChanged(const QVector3D &eulerRotation);
    void kinematicPivotChanged(const QVector3D &pivot);

    void bodyContact(AbstractPhysicsBody *body);
    void bodyEntered(AbstractPhysicsBody *body);
    void bodyExited(AbstractPhysicsBody *body);

protected:
    void updateJoltObject() override;
    void cleanup() override;
    void preSync(float deltaTime, QHash<QQuick3DNode *, QMatrix4x4> &transformCache) override;
    void sync() override;

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
    QVector3D m_offsetCenterOfMass;

    bool m_usedInSimulation = true;

    QVector3D m_kinematicPosition;
    RotationData m_kinematicRotation;
    QVector3D m_kinematicPivot;

    bool m_moveKinematicNeedsReset = false;

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
