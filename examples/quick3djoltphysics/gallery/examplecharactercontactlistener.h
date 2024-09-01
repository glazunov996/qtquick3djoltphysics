#ifndef EXAMPLECHARACTERCONTACTLISTENER_H
#define EXAMPLECHARACTERCONTACTLISTENER_H

#include <QtQuick3DJoltPhysics/charactercontactlistener.h>

class ExampleCharacterContactListener : public AbstractCharacterContactListener
{
    Q_OBJECT
    Q_PROPERTY(QList<int> rampBlockIDs READ rampBlockIDs WRITE setRampBlockIDs NOTIFY rampBlockIDsChanged)
    Q_PROPERTY(bool allowSliding READ allowSliding WRITE setAllowSliding NOTIFY allowSlidingChanged)
    QML_NAMED_ELEMENT(ExampleCharacterContactListener)
public:
    explicit ExampleCharacterContactListener(QObject *parent = nullptr);

    QList<int> rampBlockIDs() const;
    void setRampBlockIDs(const QList<int> &rampBlockIDs);

    void onAdjustBodyVelocity(int bodyID2, QVector3D &linearVelocity, QVector3D &angularVelocity) override;
    void onContactAdded(int bodyID2, int subShapeID2, const QVector3D &contactPosition, const QVector3D &contactNormal, CharacterContactSettings &settings) override;
    void onContactSolve(int bodyID2, int subShapeID2, const QVector3D &contactPosition, const QVector3D &contactNormal, const QVector3D &contactVelocity, const QVector3D &characterVelocity, QVector3D &newCharacterVelocity, bool isSlopeToSteep) override;

    bool allowSliding() const;
    void setAllowSliding(bool allowSliding);

signals:
    void rampBlockIDsChanged(const QList<int> &rampBlockIDs);
    void allowSlidingChanged(bool allowSliding);
    void enableSliding(bool canPushCharacter, int bodyID2);

private:
    bool m_allowSliding = false;
    QList<int> m_rampBlockIDs;
};

#endif // EXAMPLECHARACTERCONTACTLISTENER_H
