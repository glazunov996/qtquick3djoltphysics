#include "contactlistener.h"
#include "physicsutils_p.h"
#include "body_p.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/Collision/ContactListener.h>
#include <Jolt/Physics/Collision/CollideShape.h>

static inline CollideShapeResult toCollideShapeResultType(const JPH::CollideShapeResult &inCollisionResult)
{
    CollideShapeResult collisionResult;
    collisionResult.contactPointOn1 = PhysicsUtils::toQtType(inCollisionResult.mContactPointOn1);
    collisionResult.contactPointOn2 = PhysicsUtils::toQtType(inCollisionResult.mContactPointOn2);
    collisionResult.penetrationAxis = PhysicsUtils::toQtType(inCollisionResult.mPenetrationAxis);
    collisionResult.penetrationDepth = inCollisionResult.mPenetrationDepth;
    collisionResult.bodyID2 = inCollisionResult.mBodyID2.GetIndexAndSequenceNumber();

    for (const auto &face : inCollisionResult.mShape1Face)
        collisionResult.shape1Face.push_back(PhysicsUtils::toQtType(face));

    for (const auto &face : inCollisionResult.mShape2Face)
        collisionResult.shape2Face.push_back(PhysicsUtils::toQtType(face));

    return collisionResult;
}

static inline ContactManifold toContactManifoldType(const JPH::ContactManifold &inManifold)
{
    ContactManifold manifold;
    manifold.baseOffset = PhysicsUtils::toQtType(inManifold.mBaseOffset);
    manifold.worldSpaceNormal = PhysicsUtils::toQtType(inManifold.mWorldSpaceNormal);
    manifold.penetrationDepth = inManifold.mPenetrationDepth;

    for (const auto &point : inManifold.mRelativeContactPointsOn1)
        manifold.relativeContactPointsOn1.push_back(PhysicsUtils::toQtType(point));

    for (const auto &point : inManifold.mRelativeContactPointsOn2)
        manifold.relativeContactPointsOn2.push_back(PhysicsUtils::toQtType(point));

    return manifold;
}

static inline ContactSettings toContactSettingsType(const JPH::ContactSettings &ioSettings)
{
    ContactSettings settings;
    settings.combinedFriction = ioSettings.mCombinedFriction;
    settings.combinedRestitution = ioSettings.mCombinedRestitution;
    settings.invMassScale1 = ioSettings.mInvMassScale1;
    settings.invInertiaScale1 = ioSettings.mInvInertiaScale1;
    settings.invMassScale2 = ioSettings.mInvMassScale2;
    settings.invInertiaScale2 = ioSettings.mInvInertiaScale2;
    settings.isSensor = ioSettings.mIsSensor;
    settings.relativeLinearSurfaceVelocity = PhysicsUtils::toQtType(ioSettings.mRelativeLinearSurfaceVelocity);
    settings.relativeAngularSurfaceVelocity = PhysicsUtils::toQtType(ioSettings.mRelativeAngularSurfaceVelocity);
    return settings;
}

static inline const JPH::ContactSettings toJoltContactSettingsType(const ContactSettings &settings)
{
    JPH::ContactSettings ioSettings;
    ioSettings.mCombinedFriction = settings.combinedFriction;
    ioSettings.mCombinedRestitution = settings.combinedRestitution;
    ioSettings.mInvMassScale1 = settings.invMassScale1;
    ioSettings.mInvInertiaScale1 = settings.invInertiaScale1;
    ioSettings.mInvMassScale2 = settings.invMassScale2;
    ioSettings.mInvInertiaScale2 = settings.invInertiaScale2;
    ioSettings.mIsSensor = settings.isSensor;
    ioSettings.mRelativeLinearSurfaceVelocity = PhysicsUtils::toJoltType(settings.relativeLinearSurfaceVelocity);
    ioSettings.mRelativeAngularSurfaceVelocity = PhysicsUtils::toJoltType(settings.relativeAngularSurfaceVelocity);
    return ioSettings;
}

class ContactListenerImpl : public JPH::ContactListener
{
public:
    ContactListenerImpl(AbstractContactListener *d) : m_d(d) {}

    JPH::ValidateResult OnContactValidate(const JPH::Body &inBody1, const JPH::Body &inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult &inCollisionResult) override
    {
        auto collisionResult = toCollideShapeResultType(inCollisionResult);

        AbstractContactListener::BodyContact bodyContact;
        bodyContact.bodyID1 = inBody1.GetID().GetIndexAndSequenceNumber();
        bodyContact.bodyID2 = inBody2.GetID().GetIndexAndSequenceNumber();
        bodyContact.isBodyID1Sensor = inBody1.IsSensor();
        bodyContact.isBodyID2Sensor = inBody2.IsSensor();

        return static_cast<JPH::ValidateResult>(m_d->contactValidate(bodyContact, PhysicsUtils::toQtType(inBaseOffset), collisionResult));
    }

    void OnContactAdded(const JPH::Body &inBody1, const JPH::Body &inBody2, const JPH::ContactManifold &inManifold, JPH::ContactSettings &ioSettings) override
    {
        auto manifold = toContactManifoldType(inManifold);
        auto settings = toContactSettingsType(ioSettings);

        AbstractContactListener::BodyContact bodyContact;
        bodyContact.bodyID1 = inBody1.GetID().GetIndexAndSequenceNumber();
        bodyContact.bodyID2 = inBody2.GetID().GetIndexAndSequenceNumber();
        bodyContact.isBodyID1Sensor = inBody1.IsSensor();
        bodyContact.isBodyID2Sensor = inBody2.IsSensor();

        m_d->contactAdded(bodyContact, manifold, settings);

        ioSettings = toJoltContactSettingsType(settings);
    }

    void OnContactPersisted(const JPH::Body &inBody1, const JPH::Body &inBody2, const JPH::ContactManifold &inManifold, JPH::ContactSettings &ioSettings) override
    {
        auto manifold = toContactManifoldType(inManifold);
        auto settings = toContactSettingsType(ioSettings);

        AbstractContactListener::BodyContact bodyContact;
        bodyContact.bodyID1 = inBody1.GetID().GetIndexAndSequenceNumber();
        bodyContact.bodyID2 = inBody2.GetID().GetIndexAndSequenceNumber();
        bodyContact.isBodyID1Sensor = inBody1.IsSensor();
        bodyContact.isBodyID2Sensor = inBody2.IsSensor();

        m_d->contactPersisted(bodyContact, manifold, settings);

        ioSettings = toJoltContactSettingsType(settings);
    }

    void OnContactRemoved(const JPH::SubShapeIDPair &inSubShapePair) override
    {
        AbstractContactListener::BodyContact bodyContact;
        bodyContact.bodyID1 = inSubShapePair.GetBody1ID().GetIndexAndSequenceNumber();
        bodyContact.bodyID2 = inSubShapePair.GetBody2ID().GetIndexAndSequenceNumber();
        bodyContact.isBodyID1Sensor = false;
        bodyContact.isBodyID2Sensor = false;

        m_d->contactRemoved(bodyContact);
    }

private:
    AbstractContactListener *m_d = nullptr;
};

AbstractContactListener::AbstractContactListener(QObject *parent)
    : QObject(parent)
    , m_impl(new ContactListenerImpl(this))
{
}

AbstractContactListener::~AbstractContactListener()
{
    delete m_impl;
    m_impl = nullptr;
}

void AbstractContactListener::registerBodyContact(const BodyContact &bodyContact)
{
    QMutexLocker locker(&m_mutex);
    m_bodyContacts.push_back(bodyContact);
}

void AbstractContactListener::registerEnteredBodyContact(const BodyContact &bodyContact)
{
    QMutexLocker locker(&m_mutex);
    m_enteredBodyContacts.push_back(bodyContact);
}

void AbstractContactListener::registerExitedBodyContact(const BodyContact &bodyContact)
{
    QMutexLocker locker(&m_mutex);
    m_exitedBodyContacts.push_back(bodyContact);
}

QList<AbstractContactListener::BodyContact> AbstractContactListener::takeBodyContacts()
{
    QMutexLocker locker(&m_mutex);
    auto bodyContacts = std::move(m_bodyContacts);
    m_bodyContacts.clear();
    return bodyContacts;
}

QList<AbstractContactListener::BodyContact> AbstractContactListener::takeEnteredBodyContacts()
{
    QMutexLocker locker(&m_mutex);
    auto enteredBodyContacts = std::move(m_enteredBodyContacts);
    m_enteredBodyContacts.clear();
    return enteredBodyContacts;
}

QList<AbstractContactListener::BodyContact> AbstractContactListener::takeExitedBodyContacts()
{
    QMutexLocker locker(&m_mutex);
    auto exitedBodyContacts = std::move(m_exitedBodyContacts);
    m_exitedBodyContacts.clear();
    return exitedBodyContacts;
}

JPH::ContactListener *AbstractContactListener::getJoltContactListener() const
{
    return m_impl;
}
