#include "examplecontactlistener.h"

ExampleContactListener::ExampleContactListener(QObject *parent) : AbstractContactListener(parent) {}

QList<int> ExampleContactListener::bodyIDs() const
{
    return m_bodyIDs;
}

void ExampleContactListener::setBodyIDs(const QList<int> &bodyIDs)
{
    if (m_bodyIDs == bodyIDs)
        return;

    m_bodyIDs = bodyIDs;
    emit bodyIDsChanged(m_bodyIDs);
}

ExampleContactListener::ValidateResult ExampleContactListener::contactValidate(const BodyContact &bodyContact, const QVector3D &baseOffset, const CollideShapeResult &collisionResult)
{
    if ((bodyContact.bodyID1 == m_bodyIDs[0] && bodyContact.bodyID2 == m_bodyIDs[1]) || (bodyContact.bodyID1 == m_bodyIDs[1] && bodyContact.bodyID2 == m_bodyIDs[0]))
        return ValidateResult::RejectAllContactsForThisBodyPair;
    else
        return ValidateResult::AcceptAllContactsForThisBodyPair;
}

void ExampleContactListener::contactAdded(const BodyContact &bodyContact, const ContactManifold &manifold, ContactSettings &settings)
{
    if (bodyContact.bodyID1 == m_bodyIDs[0] || bodyContact.bodyID2 == m_bodyIDs[0]) {
        Q_ASSERT(settings.combinedRestitution == 0.0f);
        settings.combinedRestitution = 1.0f;
    }

    registerBodyContact(bodyContact);
}

void ExampleContactListener::contactPersisted(const BodyContact &bodyContact, const ContactManifold &manifold, ContactSettings &settings)
{

}

void ExampleContactListener::contactRemoved(const BodyContact &bodyContact)
{

}
