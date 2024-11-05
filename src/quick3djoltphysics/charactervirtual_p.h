#ifndef CHARACTERVIRTUAL_P_H
#define CHARACTERVIRTUAL_P_H

#include "abstractphysicscharacter_p.h"
#include "charactercontactlistener.h"
#include "qtquick3djoltphysicsglobal_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQml/QQmlEngine>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>


class Q_QUICK3DJOLTPHYSICS_EXPORT ExtendedUpdateSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D stickToFloorStepDown MEMBER stickToFloorStepDown)
    Q_PROPERTY(QVector3D walkStairsStepUp MEMBER walkStairsStepUp)
    Q_PROPERTY(float walkStairsMinStepForward MEMBER walkStairsMinStepForward)
    Q_PROPERTY(float walkStairsStepForwardTest MEMBER walkStairsStepForwardTest)
    Q_PROPERTY(float walkStairsCosAngleForwardContact MEMBER walkStairsCosAngleForwardContact)
    Q_PROPERTY(QVector3D walkStairsStepDownExtra MEMBER walkStairsStepDownExtra)
    QML_NAMED_ELEMENT(ExtendedUpdateSettings)
public:
    explicit ExtendedUpdateSettings(QObject *parent = nullptr) : QObject(parent)
        , stickToFloorStepDown(QVector3D(0, -0.5, 0))
        , walkStairsStepUp(QVector3D(0, 0.4, 0))
        , walkStairsCosAngleForwardContact(qCos(qDegreesToRadians(75)))
    {}
    ~ExtendedUpdateSettings() override = default;

    QVector3D stickToFloorStepDown;
    QVector3D walkStairsStepUp;
    float walkStairsMinStepForward = 0.02;
    float walkStairsStepForwardTest = 0.15;
    float walkStairsCosAngleForwardContact;
    QVector3D walkStairsStepDownExtra;
};

class Q_QUICK3DJOLTPHYSICS_EXPORT CharacterVirtual : public AbstractPhysicsCharacter
{
    Q_OBJECT
    Q_PROPERTY(QVector4D supportingVolume READ supportingVolume WRITE setSupportingVolume NOTIFY supportingVolumeChanged)
    Q_PROPERTY(float maxSlopeAngle READ maxSlopeAngle WRITE setMaxSlopeAngle NOTIFY maxSlopeAngleChanged)
    Q_PROPERTY(bool enhancedInternalEdgeRemoval READ enhancedInternalEdgeRemoval WRITE setEnhancedInternalEdgeRemoval NOTIFY enhancedInternalEdgeRemovalChanged)
    Q_PROPERTY(float mass READ mass WRITE setMass NOTIFY massChanged)
    Q_PROPERTY(float maxStrength READ maxStrength WRITE setMaxStrength NOTIFY maxStrengthChanged)
    Q_PROPERTY(QVector3D shapeOffset READ shapeOffset WRITE setShapeOffset NOTIFY shapeOffsetChanged)
    Q_PROPERTY(Quick3dJoltPhysics::BackFaceMode backFaceMode READ backFaceMode WRITE setBackFaceMode NOTIFY backFaceModeChanged)
    Q_PROPERTY(float predictiveContactDistance READ predictiveContactDistance WRITE setPredictiveContactDistance NOTIFY predictiveContactDistanceChanged)
    Q_PROPERTY(int maxCollisionIterations READ maxCollisionIterations WRITE setMaxCollisionIterations NOTIFY maxCollisionIterationsChanged)
    Q_PROPERTY(int maxConstraintIterations READ maxConstraintIterations WRITE setMaxConstraintIterations NOTIFY maxConstraintIterationsChanged)
    Q_PROPERTY(float minTimeRemaining READ minTimeRemaining WRITE setMinTimeRemaining NOTIFY minTimeRemainingChanged)
    Q_PROPERTY(float collisionTolerance READ collisionTolerance WRITE setCollisionTolerance NOTIFY collisionToleranceChanged)
    Q_PROPERTY(float characterPadding READ characterPadding WRITE setCharacterPadding NOTIFY characterPaddingChanged)
    Q_PROPERTY(int maxNumHits READ maxNumHits WRITE setMaxNumHits NOTIFY maxNumHitsChanged)
    Q_PROPERTY(float hitReductionCosMaxAngle READ hitReductionCosMaxAngle WRITE setHitReductionCosMaxAngle NOTIFY hitReductionCosMaxAngleChanged)
    Q_PROPERTY(float penetrationRecoverySpeed READ penetrationRecoverySpeed WRITE setPenetrationRecoverySpeed NOTIFY penetrationRecoverySpeedChanged)
    Q_PROPERTY(AbstractCharacterContactListener *characterContactListener READ characterContactListener WRITE setCharacterContactListener NOTIFY characterContactListenerChanged)
    Q_PROPERTY(int innerBodyID READ innerBodyID NOTIFY innerBodyIDChanged)
    Q_PROPERTY(int innerBodyShapeLayer READ innerBodyShapeLayer WRITE setInnerBodyShapeLayer NOTIFY innerBodyShapeLayerChanged)
    Q_PROPERTY(AbstractShape *innerBodyShape READ innerBodyShape WRITE setInnerBodyShape NOTIFY innerBodyShapeChanged)
    QML_NAMED_ELEMENT(CharacterVirtual)
public:
    explicit CharacterVirtual(QQuick3DNode *parent = nullptr);
    ~CharacterVirtual() override;

    QVector4D supportingVolume() const;
    void setSupportingVolume(const QVector4D &supportingVolume);
    float maxSlopeAngle() const;
    void setMaxSlopeAngle(float maxSlopeAngle);
    bool enhancedInternalEdgeRemoval() const;
    void setEnhancedInternalEdgeRemoval(bool enhancedInternalEdgeRemoval);
    float mass() const;
    void setMass(float mass);
    float maxStrength() const;
    void setMaxStrength(float maxStrength);
    QVector3D shapeOffset() const;
    void setShapeOffset(const QVector3D &shapeOffset);
    Quick3dJoltPhysics::BackFaceMode backFaceMode() const;
    void setBackFaceMode(Quick3dJoltPhysics::BackFaceMode backFaceMode);
    float predictiveContactDistance() const;
    void setPredictiveContactDistance(float predictiveContactDistance);
    int maxCollisionIterations() const;
    void setMaxCollisionIterations(int maxCollisionIterations);
    int maxConstraintIterations() const;
    void setMaxConstraintIterations(int maxConstraintIterations);
    float minTimeRemaining() const;
    void setMinTimeRemaining(float minTimeRemaining);
    float collisionTolerance() const;
    void setCollisionTolerance(float collisionTolerance);
    float characterPadding() const;
    void setCharacterPadding(float characterPadding);
    int maxNumHits() const;
    void setMaxNumHits(int maxNumHits);
    float hitReductionCosMaxAngle() const;
    void setHitReductionCosMaxAngle(float hitReductionCosMaxAngle);
    float penetrationRecoverySpeed() const;
    void setPenetrationRecoverySpeed(float penetrationRecoverySpeed);
    AbstractCharacterContactListener *characterContactListener() const;
    void setCharacterContactListener(AbstractCharacterContactListener *characterContactListener);
    int innerBodyID() const;
    int innerBodyShapeLayer() const;
    void setInnerBodyShapeLayer(int innerShapeLayer);
    AbstractShape *innerBodyShape() const;
    void setInnerBodyShape(AbstractShape *innerBodyShape);

    Q_INVOKABLE QVector3D getLinearVelocity() const;
    Q_INVOKABLE void setLinearVelocity(const QVector3D &linearVelocity);

    Q_INVOKABLE GroundState getGroundState() const;
    Q_INVOKABLE QVector3D getGroundPosition() const;
    Q_INVOKABLE QVector3D getGroundNormal() const;
    Q_INVOKABLE QVector3D getGroundVelocity() const;

    Q_INVOKABLE bool isSlopeTooSteep(const QVector3D &normal);

    Q_INVOKABLE void extendedUpdate(float deltaTime, const QVector3D &gravity, ExtendedUpdateSettings *updateSettings, int broadPhaseLayerFilter, int objectLayerFilter);
    Q_INVOKABLE void refreshContacts(int broadPhaseLayerFilter, int objectLayerFilter);
    Q_INVOKABLE void updateGroundVelocity();
    Q_INVOKABLE bool setShape(AbstractShape *shape, AbstractShape *innerShape, float maxPenetrationDepth, int broadPhaseLayerFilter, int objectLayerFilter);

signals:
    void supportingVolumeChanged(const QVector4D &supportingVolume);
    void maxSlopeAngleChanged(float maxSlopeAngle);
    void enhancedInternalEdgeRemovalChanged(bool enhancedInternalEdgeRemoval);
    void massChanged(float mass);
    void maxStrengthChanged(float maxStrength);
    void shapeOffsetChanged(const QVector3D &shapeOffset);
    void backFaceModeChanged(Quick3dJoltPhysics::BackFaceMode backFaceMode);
    void predictiveContactDistanceChanged(float predictiveContactDistance);
    void maxCollisionIterationsChanged(int maxCollisionIterations);
    void maxConstraintIterationsChanged(int maxConstraintIterations);
    void minTimeRemainingChanged(float minTimeRemaining);
    void collisionToleranceChanged(float collisionTolerance);
    void characterPaddingChanged(float characterPadding);
    void maxNumHitsChanged(int maxNumHits);
    void hitReductionCosMaxAngleChanged(float hitReductionCosMaxAngle);
    void penetrationRecoverySpeedChanged(float penetrationRecoverySpeed);
    void characterContactListenerChanged(AbstractCharacterContactListener *characterContactListener);
    void innerBodyIDChanged(int innerBodyID);
    void innerBodyShapeLayerChanged(int innerShapeLayer);
    void innerBodyShapeChanged(AbstractShape *shape);

protected:
    void updateJoltObject() override;
    void cleanup() override;
    void sync() override;
    void emitContactCallbacks();

private slots:
    void handleUpChanged();
    void handleScenePositionChanged();
    void handleSceneRotationChanged();

private:
    QVector4D m_supportingVolume;
    float m_maxSlopeAngle = 50.0;
    QVector3D m_shapeOffset;
    Quick3dJoltPhysics::BackFaceMode m_backFaceMode;
    AbstractCharacterContactListener *m_characterContactListener = nullptr;
    int m_innerBodyID = -1;
    AbstractShape *m_innerBodyShape = nullptr;
    QMetaObject::Connection m_innerBodyShapeSignalConnection;

    JPH::CharacterVirtual *m_character = nullptr;
    JPH::CharacterVirtualSettings m_characterSettings;
};

Q_DECLARE_METATYPE(ExtendedUpdateSettings)

#endif // CHARACTERVIRTUAL_P_H
