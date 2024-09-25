#ifndef CHARACTERCONTACTLISTENER_H
#define CHARACTERCONTACTLISTENER_H

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>

#include <QtQml/QQmlEngine>

namespace JPH {
class CharacterContactListener;
}

struct Q_QUICK3DJOLTPHYSICS_EXPORT CharacterContactSettings
{
    bool canPushCharacter;
    bool canReceiveImpulses;
};

class Q_QUICK3DJOLTPHYSICS_EXPORT AbstractCharacterContactListener : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(CharacterContactListener)
    QML_UNCREATABLE("abstract interface")
public:
    explicit AbstractCharacterContactListener(QObject *parent = nullptr);
    ~AbstractCharacterContactListener() override;

    virtual void onAdjustBodyVelocity(int bodyID2, QVector3D &linearVelocity, QVector3D &angularVelocity) = 0;
    virtual void onContactAdded(int bodyID2, int subShapeID2, const QVector3D &contactPosition, const QVector3D &contactNormal, CharacterContactSettings &settings) = 0;
    virtual void onContactSolve(int bodyID2, int subShapeID2, const QVector3D &contactPosition, const QVector3D &contactNormal, const QVector3D &contactVelocity, const QVector3D &characterVelocity, QVector3D &newCharacterVelocity, bool isSlopeTooSteep) = 0;

private:
    JPH::CharacterContactListener *getJoltCharacterContactListener() const;

    friend class CharacterContactListenerImpl;
    friend class CharacterVirtual;

    JPH::CharacterContactListener *m_impl = nullptr;
};

#endif // CHARACTERCONTACTLISTENER_H
