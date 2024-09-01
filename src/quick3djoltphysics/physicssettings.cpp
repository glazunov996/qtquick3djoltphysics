#include "physicssettings_p.h"

#include <Jolt/Math/Math.h>

PhysicsSettings::PhysicsSettings(QObject *parent) : QObject(parent)
{
}

int PhysicsSettings::maxInFlightBodyPairs() const
{
    return m_physicsSettings.mMaxInFlightBodyPairs;
}

void PhysicsSettings::setMaxInFlightBodyPairs(int maxInFlightBodyPairs)
{
    if (m_physicsSettings.mMaxInFlightBodyPairs == maxInFlightBodyPairs)
        return;

    m_physicsSettings.mMaxInFlightBodyPairs = maxInFlightBodyPairs;
    emit maxInFlightBodyPairsChanged(maxInFlightBodyPairs);
    emit changed();
}

int PhysicsSettings::stepListenersBatchSize() const
{
    return m_physicsSettings.mStepListenersBatchSize;
}

void PhysicsSettings::setStepListenersBatchSize(int stepListenersBatchSize)
{
    if (m_physicsSettings.mStepListenersBatchSize == stepListenersBatchSize)
        return;

    m_physicsSettings.mStepListenersBatchSize = stepListenersBatchSize;
    emit stepListenersBatchSizeChanged(stepListenersBatchSize);
    emit changed();
}

int PhysicsSettings::stepListenerBatchesPerJob() const
{
    return m_physicsSettings.mStepListenerBatchesPerJob;
}

void PhysicsSettings::setStepListenerBatchesPerJob(int stepListenerBatchesPerJob)
{
    if (m_physicsSettings.mStepListenerBatchesPerJob == stepListenerBatchesPerJob)
        return;

    m_physicsSettings.mStepListenerBatchesPerJob = stepListenerBatchesPerJob;
    emit stepListenerBatchesPerJobChanged(stepListenerBatchesPerJob);
    emit changed();
}

float PhysicsSettings::baumgarte() const
{
    return m_physicsSettings.mBaumgarte;
}

void PhysicsSettings::setBaumgarte(float baumgarte)
{
    if (m_physicsSettings.mBaumgarte == baumgarte)
        return;

    m_physicsSettings.mBaumgarte = baumgarte;
    emit baumgarteChanged(baumgarte);
    emit changed();
}

float PhysicsSettings::speculativeContactDistance() const
{
    return m_physicsSettings.mSpeculativeContactDistance;
}

void PhysicsSettings::setSpeculativeContactDistance(float speculativeContactDistance)
{
    if (qFuzzyCompare(m_physicsSettings.mSpeculativeContactDistance, speculativeContactDistance))
        return;

    m_physicsSettings.mSpeculativeContactDistance = speculativeContactDistance;
    emit speculativeContactDistanceChanged(speculativeContactDistance);
    emit changed();
}

float PhysicsSettings::penetrationSlop() const
{
    return m_physicsSettings.mPenetrationSlop;
}

void PhysicsSettings::setPenetrationSlop(float penetrationSlop)
{
    if (qFuzzyCompare(m_physicsSettings.mPenetrationSlop, penetrationSlop))
        return;

    m_physicsSettings.mPenetrationSlop = penetrationSlop;
    emit penetrationSlopChanged(penetrationSlop);
    emit changed();
}

float PhysicsSettings::linearCastThreshold() const
{
    return m_physicsSettings.mLinearCastThreshold;
}

void PhysicsSettings::setLinearCastThreshold(float linearCastThreshold)
{
    if (qFuzzyCompare(m_physicsSettings.mLinearCastThreshold, linearCastThreshold))
        return;

    m_physicsSettings.mLinearCastThreshold = linearCastThreshold;
    emit linearCastThresholdChanged(linearCastThreshold);
    emit changed();
}

float PhysicsSettings::linearCastMaxPenetration() const
{
    return m_physicsSettings.mLinearCastMaxPenetration;
}

void PhysicsSettings::setLinearCastMaxPenetration(float linearCastMaxPenetration)
{
    if (qFuzzyCompare(m_physicsSettings.mLinearCastMaxPenetration, linearCastMaxPenetration))
        return;

    m_physicsSettings.mLinearCastMaxPenetration = linearCastMaxPenetration;
    emit linearCastMaxPenetrationChanged(linearCastMaxPenetration);
    emit changed();
}

float PhysicsSettings::manifoldToleranceSq() const
{
    return m_physicsSettings.mManifoldToleranceSq;
}

void PhysicsSettings::setManifoldToleranceSq(float manifoldToleranceSq)
{
    if (qFuzzyCompare(m_physicsSettings.mManifoldToleranceSq, manifoldToleranceSq))
        return;

    m_physicsSettings.mManifoldToleranceSq = manifoldToleranceSq;
    emit manifoldToleranceSqChanged(manifoldToleranceSq);
    emit changed();
}

float PhysicsSettings::maxPenetrationDistance() const
{
    return m_physicsSettings.mMaxPenetrationDistance;
}

void PhysicsSettings::setMaxPenetrationDistance(float maxPenetrationDistance)
{
    if (qFuzzyCompare(m_physicsSettings.mMaxPenetrationDistance, maxPenetrationDistance))
        return;

    m_physicsSettings.mMaxPenetrationDistance = maxPenetrationDistance;
    emit maxPenetrationDistanceChanged(maxPenetrationDistance);
    emit changed();
}

float PhysicsSettings::bodyPairCacheMaxDeltaPositionSq() const
{
    return m_physicsSettings.mBodyPairCacheMaxDeltaPositionSq;
}

void PhysicsSettings::setBodyPairCacheMaxDeltaPositionSq(float bodyPairCacheMaxDeltaPositionSq)
{
    auto square = JPH::Square(bodyPairCacheMaxDeltaPositionSq);
    if (qFuzzyCompare(m_physicsSettings.mBodyPairCacheMaxDeltaPositionSq, square))
        return;

    m_physicsSettings.mBodyPairCacheMaxDeltaPositionSq = square;
    emit bodyPairCacheMaxDeltaPositionSqChanged(square);
    emit changed();
}

float PhysicsSettings::bodyPairCacheCosMaxDeltaRotationDiv2() const
{
    return m_physicsSettings.mBodyPairCacheCosMaxDeltaRotationDiv2;
}

void PhysicsSettings::setBodyPairCacheCosMaxDeltaRotationDiv2(float bodyPairCacheCosMaxDeltaRotationDiv2)
{
    if (qFuzzyCompare(m_physicsSettings.mBodyPairCacheCosMaxDeltaRotationDiv2, bodyPairCacheCosMaxDeltaRotationDiv2))
        return;

    m_physicsSettings.mBodyPairCacheCosMaxDeltaRotationDiv2 = bodyPairCacheCosMaxDeltaRotationDiv2;
    emit bodyPairCacheCosMaxDeltaRotationDiv2Changed(bodyPairCacheCosMaxDeltaRotationDiv2);
    emit changed();
}

float PhysicsSettings::contactNormalCosMaxDeltaRotation() const
{
    return m_physicsSettings.mContactNormalCosMaxDeltaRotation;
}

void PhysicsSettings::setContactNormalCosMaxDeltaRotation(float contactNormalCosMaxDeltaRotation)
{
    if (qFuzzyCompare(m_physicsSettings.mContactNormalCosMaxDeltaRotation, contactNormalCosMaxDeltaRotation))
        return;

    m_physicsSettings.mContactNormalCosMaxDeltaRotation = contactNormalCosMaxDeltaRotation;
    emit contactNormalCosMaxDeltaRotationChanged(contactNormalCosMaxDeltaRotation);
    emit changed();
}

float PhysicsSettings::contactPointPreserveLambdaMaxDistSq() const
{
    return m_physicsSettings.mContactPointPreserveLambdaMaxDistSq;
}

void PhysicsSettings::setContactPointPreserveLambdaMaxDistSq(float contactPointPreserveLambdaMaxDistSq)
{
    auto square = JPH::Square(contactPointPreserveLambdaMaxDistSq);
    if (qFuzzyCompare(m_physicsSettings.mContactPointPreserveLambdaMaxDistSq, square))
        return;

    m_physicsSettings.mContactPointPreserveLambdaMaxDistSq = square;
    emit contactPointPreserveLambdaMaxDistSqChanged(square);
    emit changed();
}

unsigned int PhysicsSettings::numVelocitySteps() const
{
    return m_physicsSettings.mNumVelocitySteps;
}

void PhysicsSettings::setNumVelocitySteps(unsigned int numVelocitySteps)
{
    if (m_physicsSettings.mNumVelocitySteps == numVelocitySteps)
        return;

    m_physicsSettings.mNumVelocitySteps = numVelocitySteps;
    emit numVelocityStepsChanged(numVelocitySteps);
    emit changed();
}

unsigned int PhysicsSettings::numPositionSteps() const
{
    return m_physicsSettings.mNumPositionSteps;
}

void PhysicsSettings::setNumPositionSteps(unsigned int numPositionSteps)
{
    if (m_physicsSettings.mNumPositionSteps == numPositionSteps)
        return;

    m_physicsSettings.mNumPositionSteps = numPositionSteps;
    emit numPositionStepsChanged(numPositionSteps);
    emit changed();
}

float PhysicsSettings::minVelocityForRestitution() const
{
    return m_physicsSettings.mMinVelocityForRestitution;
}

void PhysicsSettings::setMinVelocityForRestitution(float minVelocityForRestitution)
{
    if (qFuzzyCompare(m_physicsSettings.mMinVelocityForRestitution, minVelocityForRestitution))
        return;

    m_physicsSettings.mMinVelocityForRestitution = minVelocityForRestitution;
    emit minVelocityForRestitutionChanged(minVelocityForRestitution);
    emit changed();
}

float PhysicsSettings::timeBeforeSleep() const
{
    return m_physicsSettings.mTimeBeforeSleep;
}


void PhysicsSettings::setTimeBeforeSleep(float timeBeforeSleep)
{
    if (qFuzzyCompare(m_physicsSettings.mTimeBeforeSleep, timeBeforeSleep))
        return;

    m_physicsSettings.mTimeBeforeSleep = timeBeforeSleep;
    emit timeBeforeSleepChanged(timeBeforeSleep);
    emit changed();
}

float PhysicsSettings::pointVelocitySleepThreshold() const
{
    return m_physicsSettings.mPointVelocitySleepThreshold;
}

void PhysicsSettings::setPointVelocitySleepThreshold(float pointVelocitySleepThreshold)
{
    if (qFuzzyCompare(m_physicsSettings.mPointVelocitySleepThreshold, pointVelocitySleepThreshold))
        return;

    m_physicsSettings.mPointVelocitySleepThreshold = pointVelocitySleepThreshold;
    emit pointVelocitySleepThresholdChanged(pointVelocitySleepThreshold);
    emit changed();
}

bool PhysicsSettings::deterministicSimulation() const
{
    return m_physicsSettings.mDeterministicSimulation;
}

void PhysicsSettings::setDeterministicSimulation(bool deterministicSimulation)
{
    if (m_physicsSettings.mDeterministicSimulation == deterministicSimulation)
        return;

    m_physicsSettings.mDeterministicSimulation = deterministicSimulation;
    emit deterministicSimulationChanged(deterministicSimulation);
    emit changed();
}

bool PhysicsSettings::constraintWarmStart() const
{
    return m_physicsSettings.mConstraintWarmStart;
}

void PhysicsSettings::setConstraintWarmStart(bool constraintWarmStart)
{
    if (m_physicsSettings.mConstraintWarmStart == constraintWarmStart)
        return;

    m_physicsSettings.mConstraintWarmStart = constraintWarmStart;
    emit constraintWarmStartChanged(constraintWarmStart);
    emit changed();
}

bool PhysicsSettings::useBodyPairContactCache() const
{
    return m_physicsSettings.mUseBodyPairContactCache;
}

void PhysicsSettings::setUseBodyPairContactCache(bool useBodyPairContactCache)
{
    if (m_physicsSettings.mUseBodyPairContactCache == useBodyPairContactCache)
        return;

    m_physicsSettings.mUseBodyPairContactCache = useBodyPairContactCache;
    emit useBodyPairContactCacheChanged(useBodyPairContactCache);
    emit changed();
}

bool PhysicsSettings::useManifoldReduction() const
{
    return m_physicsSettings.mUseManifoldReduction;
}

void PhysicsSettings::setUseManifoldReduction(bool useManifoldReduction)
{
    if (m_physicsSettings.mUseManifoldReduction == useManifoldReduction)
        return;

    m_physicsSettings.mUseManifoldReduction = useManifoldReduction;
    emit useManifoldReductionChanged(useManifoldReduction);
    emit changed();
}

bool PhysicsSettings::useLargeIslandSplitter() const
{
    return m_physicsSettings.mUseLargeIslandSplitter;
}

void PhysicsSettings::setUseLargeIslandSplitter(bool useLargeIslandSplitter)
{
    if (m_physicsSettings.mUseLargeIslandSplitter == useLargeIslandSplitter)
        return;

    m_physicsSettings.mUseLargeIslandSplitter = useLargeIslandSplitter;
    emit useLargeIslandSplitterChanged(useLargeIslandSplitter);
    emit changed();
}

bool PhysicsSettings::allowSleeping() const
{
    return m_physicsSettings.mAllowSleeping;
}

void PhysicsSettings::setAllowSleeping(bool allowSleeping)
{
    if (m_physicsSettings.mAllowSleeping == allowSleeping)
        return;

    m_physicsSettings.mAllowSleeping = allowSleeping;
    emit allowSleepingChanged(allowSleeping);
    emit changed();
}

bool PhysicsSettings::checkActiveEdges() const
{
    return m_physicsSettings.mCheckActiveEdges;
}

void PhysicsSettings::setCheckActiveEdges(bool checkActiveEdges)
{
    if (m_physicsSettings.mCheckActiveEdges == checkActiveEdges)
        return;

    m_physicsSettings.mCheckActiveEdges = checkActiveEdges;
    emit checkActiveEdgesChanged(checkActiveEdges);
    emit changed();
}

JPH::PhysicsSettings PhysicsSettings::getJoltPhysicsSettings() const
{
    return m_physicsSettings;
}
