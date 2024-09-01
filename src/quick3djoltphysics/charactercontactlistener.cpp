#include "charactercontactlistener.h"
#include "charactervirtual_p.h"
#include "physicsutils_p.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>
#include <Jolt/Physics/Body/Body.h>

static inline CharacterContactSettings toCharacterContactSettings(const JPH::CharacterContactSettings &ioSettings)
{
    CharacterContactSettings settings;
    settings.canPushCharacter = ioSettings.mCanPushCharacter;
    settings.canReceiveImpulses = ioSettings.mCanReceiveImpulses;
    return settings;
}

static inline JPH::CharacterContactSettings toJoltCharacterContactSettings(const CharacterContactSettings &settings)
{
    JPH::CharacterContactSettings ioSettings;
    ioSettings.mCanPushCharacter = settings.canPushCharacter;
    ioSettings.mCanReceiveImpulses = settings.canReceiveImpulses;
    return ioSettings;
}

class CharacterContactListenerImpl : public JPH::CharacterContactListener
{
public:
    CharacterContactListenerImpl(AbstractCharacterContactListener *d) : m_d(d) {}

    void OnAdjustBodyVelocity(const JPH::CharacterVirtual *inCharacter, const JPH::Body &inBody2, JPH::Vec3 &ioLinearVelocity, JPH::Vec3 &ioAngularVelocity) override
    {
        Q_UNUSED(inCharacter);

        auto linearVelocity = PhysicsUtils::toQtType(ioLinearVelocity);
        auto angularVelocity = PhysicsUtils::toQtType(ioAngularVelocity);

        m_d->onAdjustBodyVelocity(inBody2.GetID().GetIndexAndSequenceNumber(), linearVelocity, angularVelocity);

        ioLinearVelocity = PhysicsUtils::toJoltType(linearVelocity);
        ioAngularVelocity = PhysicsUtils::toJoltType(angularVelocity);
    }

    void OnContactAdded(const JPH::CharacterVirtual *inCharacter, const JPH::BodyID &inBodyID2, const JPH::SubShapeID &inSubShapeID2, JPH::RVec3Arg inContactPosition, JPH::Vec3Arg inContactNormal, JPH::CharacterContactSettings &ioSettings) override
    {
        Q_UNUSED(inCharacter);

        auto settings = toCharacterContactSettings(ioSettings);

        m_d->onContactAdded(inBodyID2.GetIndexAndSequenceNumber(),
                            inSubShapeID2.GetValue(),
                            PhysicsUtils::toQtType(inContactPosition),
                            PhysicsUtils::toQtType(inContactNormal),
                            settings);

        ioSettings = toJoltCharacterContactSettings(settings);
    }

    void OnContactSolve(const JPH::CharacterVirtual *inCharacter, const JPH::BodyID &inBodyID2, const JPH::SubShapeID &inSubShapeID2, JPH::RVec3Arg inContactPosition, JPH::Vec3Arg inContactNormal, JPH::Vec3Arg inContactVelocity, const JPH::PhysicsMaterial *inContactMaterial, JPH::Vec3Arg inCharacterVelocity, JPH::Vec3 &ioNewCharacterVelocity) override
    {
        Q_UNUSED(inContactMaterial);

        const bool isSlopeToSteep = inCharacter->IsSlopeTooSteep(inContactNormal);

        auto newCharacterVelocity = PhysicsUtils::toQtType(ioNewCharacterVelocity);

        m_d->onContactSolve(inBodyID2.GetIndexAndSequenceNumber(),
                            inSubShapeID2.GetValue(),
                            PhysicsUtils::toQtType(inContactPosition),
                            PhysicsUtils::toQtType(inContactNormal),
                            PhysicsUtils::toQtType(inContactVelocity),
                            PhysicsUtils::toQtType(inCharacterVelocity),
                            newCharacterVelocity,
                            isSlopeToSteep);

        ioNewCharacterVelocity = PhysicsUtils::toJoltType(newCharacterVelocity);
    }

private:
    AbstractCharacterContactListener *m_d = nullptr;
};

AbstractCharacterContactListener::AbstractCharacterContactListener(QObject *parent)
    : QObject(parent)
    , m_impl(new CharacterContactListenerImpl(this))
{
}

AbstractCharacterContactListener::~AbstractCharacterContactListener()
{
    delete m_impl;
    m_impl = nullptr;
}

JPH::CharacterContactListener *AbstractCharacterContactListener::getJoltCharacterContactListener() const
{
    return m_impl;
}
