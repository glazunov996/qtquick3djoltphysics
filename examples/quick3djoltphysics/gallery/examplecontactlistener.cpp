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

ExampleContactListener::ValidateResult ExampleContactListener::contactValidate(int body1ID, int body2ID, const QVector3D &baseOffset, const CollideShapeResult &collisionResult)
{
    if ((body1ID == m_bodyIDs[0] && body2ID == m_bodyIDs[1]) || (body1ID == m_bodyIDs[1] && body2ID == m_bodyIDs[0]))
        return ValidateResult::RejectAllContactsForThisBodyPair;
    else
        return ValidateResult::AcceptAllContactsForThisBodyPair;
}

void ExampleContactListener::contactAdded(int body1ID, int body2ID, const ContactManifold &manifold, ContactSettings &settings)
{
    if (body1ID == m_bodyIDs[0] || body2ID == m_bodyIDs[0]) {
        Q_ASSERT(settings.combinedRestitution == 0.0f);
        settings.combinedRestitution = 1.0f;
    }
}

void ExampleContactListener::contactPersisted(int body1ID, int body2ID, const ContactManifold &manifold, ContactSettings &settings)
{

}

void ExampleContactListener::contactRemoved(int body1ID, int subShapeID1, int body2ID, int subShapeID2)
{

}
