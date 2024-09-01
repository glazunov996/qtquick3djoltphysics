#ifndef CHARACTERVIRTUAL_P_H
#define CHARACTERVIRTUAL_P_H

#include "abstractphysicsbody_p.h"
#include "charactercontactlistener.h"

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

class Q_QUICK3DJOLTPHYSICS_EXPORT CharacterVirtual : public AbstractPhysicsBody
{
    Q_OBJECT
    Q_PROPERTY(QVector4D supportingVolume READ supportingVolume WRITE setSupportingVolume NOTIFY supportingVolumeChanged)
    Q_PROPERTY(float maxSlopeAngle READ maxSlopeAngle WRITE setMaxSlopeAngle NOTIFY maxSlopeAngleChanged)
    Q_PROPERTY(float mass READ mass WRITE setMass NOTIFY massChanged)
    Q_PROPERTY(float maxStrength READ maxStrength WRITE setMaxStrength NOTIFY maxStrengthChanged)
    Q_PROPERTY(QVector3D shapeOffset READ shapeOffset WRITE setShapeOffset NOTIFY shapeOffsetChanged)
    Q_PROPERTY(BackFaceMode backFaceMode READ backFaceMode WRITE setBackFaceMode NOTIFY backFaceModeChanged)
    Q_PROPERTY(float predictiveContactDistance READ predictiveContactDistance WRITE setPredictiveContactDistance NOTIFY predictiveContactDistanceChanged)
    Q_PROPERTY(quint8 maxCollisionIterations READ maxCollisionIterations WRITE setMaxCollisionIterations NOTIFY maxCollisionIterationsChanged)
    Q_PROPERTY(quint8 maxConstraintIterations READ maxConstraintIterations WRITE setMaxConstraintIterations NOTIFY maxConstraintIterationsChanged)
    Q_PROPERTY(float minTimeRemaining READ minTimeRemaining WRITE setMinTimeRemaining NOTIFY minTimeRemainingChanged)
    Q_PROPERTY(float collisionTolerance READ collisionTolerance WRITE setCollisionTolerance NOTIFY collisionToleranceChanged)
    Q_PROPERTY(float characterPadding READ characterPadding WRITE setCharacterPadding NOTIFY characterPaddingChanged)
    Q_PROPERTY(quint8 maxNumHits READ maxNumHits WRITE setMaxNumHits NOTIFY maxNumHitsChanged)
    Q_PROPERTY(float hitReductionCosMaxAngle READ hitReductionCosMaxAngle WRITE setHitReductionCosMaxAngle NOTIFY hitReductionCosMaxAngleChanged)
    Q_PROPERTY(float penetrationRecoverySpeed READ penetrationRecoverySpeed WRITE setPenetrationRecoverySpeed NOTIFY penetrationRecoverySpeedChanged)
    Q_PROPERTY(AbstractCharacterContactListener *characterContactListener READ characterContactListener WRITE setCharacterContactListener NOTIFY characterContactListenerChanged)
    QML_NAMED_ELEMENT(CharacterVirtual)
public:
    explicit CharacterVirtual(QQuick3DNode *parent = nullptr);
    ~CharacterVirtual() override;

    enum class BackFaceMode {
        IgnoreBackFaces,
        CollideWithBackFaces
    };
    Q_ENUM(BackFaceMode)

    enum class GroundState {
        OnGround,
        OnSteepGround,
        NotSupported,
        InAir
    };
    Q_ENUM(GroundState)

    QVector4D supportingVolume() const;
    void setSupportingVolume(const QVector4D &supportingVolume);
    float maxSlopeAngle() const;
    void setMaxSlopeAngle(float maxSlopeAngle);
    float mass() const;
    void setMass(float mass);
    float maxStrength() const;
    void setMaxStrength(float maxStrength);
    QVector3D shapeOffset() const;
    void setShapeOffset(const QVector3D &shapeOffset);
    BackFaceMode backFaceMode() const;
    void setBackFaceMode(BackFaceMode backFaceMode);
    float predictiveContactDistance() const;
    void setPredictiveContactDistance(float predictiveContactDistance);
    quint8 maxCollisionIterations() const;
    void setMaxCollisionIterations(quint8 maxCollisionIterations);
    quint8 maxConstraintIterations() const;
    void setMaxConstraintIterations(quint8 maxConstraintIterations);
    float minTimeRemaining() const;
    void setMinTimeRemaining(float minTimeRemaining);
    float collisionTolerance() const;
    void setCollisionTolerance(float collisionTolerance);
    float characterPadding() const;
    void setCharacterPadding(float characterPadding);
    quint8 maxNumHits() const;
    void setMaxNumHits(quint8 maxNumHits);
    float hitReductionCosMaxAngle() const;
    void setHitReductionCosMaxAngle(float hitReductionCosMaxAngle);
    float penetrationRecoverySpeed() const;
    void setPenetrationRecoverySpeed(float penetrationRecoverySpeed);
    AbstractCharacterContactListener *characterContactListener() const;
    void setCharacterContactListener(AbstractCharacterContactListener *characterContactListener);

    Q_INVOKABLE QVector3D getLinearVelocity() const;
    Q_INVOKABLE void setLinearVelocity(const QVector3D &linearVelocity);

    Q_INVOKABLE GroundState getGroundState() const;
    Q_INVOKABLE QVector3D getGroundPosition() const;
    Q_INVOKABLE QVector3D getGroundNormal() const;
    Q_INVOKABLE QVector3D getGroundVelocity() const;

    Q_INVOKABLE QMatrix4x4 getCenterOfMassTransform() const;

    Q_INVOKABLE bool isSlopeTooSteep(const QVector3D &normal);
    Q_INVOKABLE void extendedUpdate(float deltaTime,
                                    const QVector3D &gravity,
                                    ExtendedUpdateSettings *updateSettings,
                                    unsigned int broadPhaseLayerFilter,
                                    unsigned int objectLayerFilter);
    Q_INVOKABLE void updateGroundVelocity();
    Q_INVOKABLE void setShape(AbstractShape *shape,
                              float maxPenetrationDepth,
                              unsigned int broadPhaseLayerFilter,
                              unsigned int objectLayerFilter);

signals:
    void supportingVolumeChanged(const QVector4D &supportingVolume);
    void maxSlopeAngleChanged(float maxSlopeAngle);
    void massChanged(float mass);
    void maxStrengthChanged(float maxStrength);
    void shapeOffsetChanged(const QVector3D &shapeOffset);
    void backFaceModeChanged(BackFaceMode backFaceMode);
    void predictiveContactDistanceChanged(float predictiveContactDistance);
    void maxCollisionIterationsChanged(quint8 maxCollisionIterations);
    void maxConstraintIterationsChanged(quint8 maxConstraintIterations);
    void minTimeRemainingChanged(float minTimeRemaining);
    void collisionToleranceChanged(float collisionTolerance);
    void characterPaddingChanged(float characterPadding);
    void maxNumHitsChanged(quint8 maxNumHits);
    void hitReductionCosMaxAngleChanged(float hitReductionCosMaxAngle);
    void penetrationRecoverySpeedChanged(float penetrationRecoverySpeed);
    void characterContactListenerChanged(AbstractCharacterContactListener *characterContactListener);

protected:
    void updateJoltObject() override;
    void cleanup() override;
    void sync() override;

private slots:
    void handleUpChanged();
    void handleSceneRotationChanged();

private:
    QVector4D m_supportingVolume;
    float m_maxSlopeAngle = 50.0;
    QVector3D m_shapeOffset;
    BackFaceMode m_backFaceMode;
    AbstractCharacterContactListener *m_characterContactListener = nullptr;

    float m_maxPenetrationDepth;
    unsigned int m_broadPhaseLayerFilter;
    unsigned int m_objectLayerFilter;
    bool m_updateShapeNeeded = false;

    JPH::CharacterVirtual *m_character = nullptr;
    JPH::CharacterVirtualSettings m_characterSettings;
};

Q_DECLARE_METATYPE(ExtendedUpdateSettings)

#endif // CHARACTERVIRTUAL_P_H
