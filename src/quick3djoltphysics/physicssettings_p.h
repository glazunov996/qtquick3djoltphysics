#ifndef PHYSICSSETTINGS_P_H
#define PHYSICSSETTINGS_P_H

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQml/QQmlEngine>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSettings.h>

class Q_QUICK3DJOLTPHYSICS_EXPORT PhysicsSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int maxInFlightBodyPairs READ maxInFlightBodyPairs WRITE setMaxInFlightBodyPairs NOTIFY maxInFlightBodyPairsChanged)
    Q_PROPERTY(int stepListenersBatchSize READ stepListenersBatchSize WRITE setStepListenersBatchSize NOTIFY stepListenersBatchSizeChanged)
    Q_PROPERTY(int stepListenerBatchesPerJob READ stepListenerBatchesPerJob WRITE setStepListenerBatchesPerJob NOTIFY stepListenerBatchesPerJobChanged)
    Q_PROPERTY(float baumgarte READ baumgarte WRITE setBaumgarte NOTIFY baumgarteChanged)
    Q_PROPERTY(float speculativeContactDistance READ speculativeContactDistance WRITE setSpeculativeContactDistance NOTIFY speculativeContactDistanceChanged)
    Q_PROPERTY(float penetrationSlop READ penetrationSlop WRITE setPenetrationSlop NOTIFY penetrationSlopChanged)
    Q_PROPERTY(float linearCastThreshold READ linearCastThreshold WRITE setLinearCastThreshold NOTIFY linearCastThresholdChanged)
    Q_PROPERTY(float linearCastMaxPenetration READ linearCastMaxPenetration WRITE setLinearCastMaxPenetration NOTIFY linearCastMaxPenetrationChanged)
    Q_PROPERTY(float manifoldToleranceSq READ manifoldToleranceSq WRITE setManifoldToleranceSq NOTIFY manifoldToleranceSqChanged)
    Q_PROPERTY(float maxPenetrationDistance READ maxPenetrationDistance WRITE setMaxPenetrationDistance NOTIFY maxPenetrationDistanceChanged)
    Q_PROPERTY(float bodyPairCacheMaxDeltaPositionSq READ bodyPairCacheMaxDeltaPositionSq WRITE setBodyPairCacheMaxDeltaPositionSq NOTIFY bodyPairCacheMaxDeltaPositionSqChanged)
    Q_PROPERTY(float bodyPairCacheCosMaxDeltaRotationDiv2 READ bodyPairCacheCosMaxDeltaRotationDiv2 WRITE setBodyPairCacheCosMaxDeltaRotationDiv2 NOTIFY bodyPairCacheCosMaxDeltaRotationDiv2Changed)
    Q_PROPERTY(float contactNormalCosMaxDeltaRotation READ contactNormalCosMaxDeltaRotation WRITE setContactNormalCosMaxDeltaRotation NOTIFY contactNormalCosMaxDeltaRotationChanged)
    Q_PROPERTY(float contactPointPreserveLambdaMaxDistSq READ contactPointPreserveLambdaMaxDistSq WRITE setContactPointPreserveLambdaMaxDistSq NOTIFY contactPointPreserveLambdaMaxDistSqChanged)
    Q_PROPERTY(unsigned int numVelocitySteps READ numVelocitySteps WRITE setNumVelocitySteps NOTIFY numVelocityStepsChanged)
    Q_PROPERTY(unsigned numPositionSteps READ numPositionSteps WRITE setNumPositionSteps NOTIFY numPositionStepsChanged)
    Q_PROPERTY(float minVelocityForRestitution READ minVelocityForRestitution WRITE setMinVelocityForRestitution NOTIFY minVelocityForRestitutionChanged)
    Q_PROPERTY(float timeBeforeSleep READ timeBeforeSleep WRITE setTimeBeforeSleep NOTIFY timeBeforeSleepChanged)
    Q_PROPERTY(float pointVelocitySleepThreshold READ pointVelocitySleepThreshold WRITE setPointVelocitySleepThreshold NOTIFY pointVelocitySleepThresholdChanged)
    Q_PROPERTY(bool deterministicSimulation READ deterministicSimulation WRITE setDeterministicSimulation NOTIFY deterministicSimulationChanged)
    Q_PROPERTY(bool constraintWarmStart READ constraintWarmStart WRITE setConstraintWarmStart NOTIFY constraintWarmStartChanged)
    Q_PROPERTY(bool useBodyPairContactCache READ useBodyPairContactCache WRITE setUseBodyPairContactCache NOTIFY useBodyPairContactCacheChanged)
    Q_PROPERTY(bool useManifoldReduction READ useManifoldReduction WRITE setUseManifoldReduction NOTIFY useManifoldReductionChanged)
    Q_PROPERTY(bool useLargeIslandSplitter READ useLargeIslandSplitter WRITE setUseLargeIslandSplitter NOTIFY useLargeIslandSplitterChanged)
    Q_PROPERTY(bool allowSleeping READ allowSleeping WRITE setAllowSleeping NOTIFY allowSleepingChanged)
    Q_PROPERTY(bool checkActiveEdges READ checkActiveEdges WRITE setCheckActiveEdges NOTIFY checkActiveEdgesChanged)
    QML_NAMED_ELEMENT(PhysicsSettings)
public:
    explicit PhysicsSettings(QObject *parent = nullptr);

    int maxInFlightBodyPairs() const;
    void setMaxInFlightBodyPairs(int maxInFlightBodyPairs);

    int stepListenersBatchSize() const;
    void setStepListenersBatchSize(int stepListenersBatchSize);

    int stepListenerBatchesPerJob() const;
    void setStepListenerBatchesPerJob(int stepListenerBatchesPerJob);

    float baumgarte() const;
    void setBaumgarte(float baumgarte);

    float speculativeContactDistance() const;
    void setSpeculativeContactDistance(float speculativeContactDistance);

    float penetrationSlop() const;
    void setPenetrationSlop(float penetrationSlop);

    float linearCastThreshold() const;
    void setLinearCastThreshold(float linearCastThreshold);

    float linearCastMaxPenetration() const;
    void setLinearCastMaxPenetration(float linearCastMaxPenetration);

    float manifoldToleranceSq() const;
    void setManifoldToleranceSq(float manifoldToleranceSq);

    float maxPenetrationDistance() const;
    void setMaxPenetrationDistance(float maxPenetrationDistance);

    float bodyPairCacheMaxDeltaPositionSq() const;
    void setBodyPairCacheMaxDeltaPositionSq(float bodyPairCacheMaxDeltaPositionSq);

    float bodyPairCacheCosMaxDeltaRotationDiv2() const;
    void setBodyPairCacheCosMaxDeltaRotationDiv2(float bodyPairCacheCosMaxDeltaRotationDiv2);

    float contactNormalCosMaxDeltaRotation() const;
    void setContactNormalCosMaxDeltaRotation(float contactNormalCosMaxDeltaRotation);

    float contactPointPreserveLambdaMaxDistSq() const;
    void setContactPointPreserveLambdaMaxDistSq(float contactPointPreserveLambdaMaxDistSq);

    unsigned int numVelocitySteps() const;
    void setNumVelocitySteps(unsigned int numVelocitySteps);

    unsigned int numPositionSteps() const;
    void setNumPositionSteps(unsigned int numPositionSteps);

    float minVelocityForRestitution() const;
    void setMinVelocityForRestitution(float minVelocityForRestitution);

    float timeBeforeSleep() const;
    void setTimeBeforeSleep(float timeBeforeSleep);

    float pointVelocitySleepThreshold() const;
    void setPointVelocitySleepThreshold(float pointVelocitySleepThreshold);

    bool deterministicSimulation() const;
    void setDeterministicSimulation(bool deterministicSimulation);

    bool constraintWarmStart() const;
    void setConstraintWarmStart(bool constraintWarmStart);

    bool useBodyPairContactCache() const;
    void setUseBodyPairContactCache(bool useBodyPairContactCache);

    bool useManifoldReduction() const;
    void setUseManifoldReduction(bool useManifoldReduction);

    bool useLargeIslandSplitter() const;
    void setUseLargeIslandSplitter(bool useLargeIslandSplitter);

    bool allowSleeping() const;
    void setAllowSleeping(bool allowSleeping);

    bool checkActiveEdges() const;
    void setCheckActiveEdges(bool checkActiveEdges);

signals:
    void maxInFlightBodyPairsChanged(int maxInFlightBodyPairs);
    void stepListenersBatchSizeChanged(int stepListenersBatchSize);
    void stepListenerBatchesPerJobChanged(int stepListenerBatchesPerJob);
    void baumgarteChanged(float baumgarte);
    void speculativeContactDistanceChanged(float speculativeContactDistance);
    void penetrationSlopChanged(float penetrationSlop);
    void linearCastThresholdChanged(float linearCastThreshold);
    void linearCastMaxPenetrationChanged(float linearCastMaxPenetration);
    void manifoldToleranceSqChanged(float manifoldToleranceSq);
    void maxPenetrationDistanceChanged(float maxPenetrationDistance);
    void bodyPairCacheMaxDeltaPositionSqChanged(float bodyPairCacheMaxDeltaPositionSq);
    void bodyPairCacheCosMaxDeltaRotationDiv2Changed(float bodyPairCacheCosMaxDeltaRotationDiv2);
    void contactNormalCosMaxDeltaRotationChanged(float contactNormalCosMaxDeltaRotation);
    void contactPointPreserveLambdaMaxDistSqChanged(float contactPointPreserveLambdaMaxDistSq);
    void numVelocityStepsChanged(unsigned int numVelocitySteps);
    void numPositionStepsChanged(unsigned int numPositionSteps);
    void minVelocityForRestitutionChanged(float minVelocityForRestitution);
    void timeBeforeSleepChanged(float timeBeforeSleep);
    void pointVelocitySleepThresholdChanged(float pointVelocitySleepThreshold);
    void deterministicSimulationChanged(float deterministicSimulation);
    void constraintWarmStartChanged(bool constraintWarmStart);
    void useBodyPairContactCacheChanged(bool useBodyPairContactCache);
    void useManifoldReductionChanged(bool useManifoldReduction);
    void useLargeIslandSplitterChanged(bool useLargeIslandSplitter);
    void allowSleepingChanged(bool allowSleeping);
    void checkActiveEdgesChanged(bool checkActiveEdges);
    void changed();

private:
    JPH::PhysicsSettings getJoltPhysicsSettings() const;
    friend class PhysicsSystem;

    JPH::PhysicsSettings m_physicsSettings;
};

#endif // PHYSICSSETTINGS_P_H
