#include "examplecharactercontactlistener.h"

#include <QtGui/QVector3D>
#include <QDebug>

ExampleCharacterContactListener::ExampleCharacterContactListener(QObject *parent) : AbstractCharacterContactListener(parent) {}

QList<int> ExampleCharacterContactListener::rampBlockIDs() const
{
    return m_rampBlockIDs;
}

void ExampleCharacterContactListener::setRampBlockIDs(const QList<int> &rampBlockIDs)
{
    if (m_rampBlockIDs == rampBlockIDs)
        return;

    m_rampBlockIDs = rampBlockIDs;
    emit rampBlockIDsChanged(m_rampBlockIDs);
}

bool ExampleCharacterContactListener::allowSliding() const
{
    return m_allowSliding;
}

void ExampleCharacterContactListener::setAllowSliding(bool allowSliding)
{
    if (m_allowSliding == allowSliding)
        return;

    m_allowSliding = allowSliding;
    emit allowSlidingChanged(m_allowSliding);
}

void ExampleCharacterContactListener::onAdjustBodyVelocity(int bodyID2, QVector3D &linearVelocity, QVector3D &angularVelocity)
{
}

void ExampleCharacterContactListener::onContactAdded(int bodyID2, int subShapeID2, const QVector3D &contactPosition, const QVector3D &contactNormal, CharacterContactSettings &settings)
{
    auto i = std::find(m_rampBlockIDs.begin(), m_rampBlockIDs.end(), bodyID2);
    if (i != m_rampBlockIDs.end()) {
        size_t index = i - m_rampBlockIDs.begin();
        settings.canPushCharacter = (index & 1) != 0;
        settings.canReceiveImpulses = (index & 2) != 0;
    }

    emit enableSliding(settings.canPushCharacter, bodyID2);
}

void ExampleCharacterContactListener::onContactSolve(int bodyID2, int subShapeID2, const QVector3D &contactPosition, const QVector3D &contactNormal, const QVector3D &contactVelocity, const QVector3D &characterVelocity, QVector3D &newCharacterVelocity, bool isSlopeToSteep)
{
    if (!m_allowSliding && qFuzzyCompare(contactVelocity, QVector3D()) && !isSlopeToSteep)
        newCharacterVelocity = QVector3D();
}
