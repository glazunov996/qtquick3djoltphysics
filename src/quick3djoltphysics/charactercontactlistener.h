#ifndef CHARACTERCONTACTLISTENER_H
#define CHARACTERCONTACTLISTENER_H

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>

#include <QtQml/QQmlEngine>
#include <QVector3D>
#include <QList>
#include <QMutex>

namespace JPH {
class CharacterContactListener;
}

class Q_QUICK3DJOLTPHYSICS_EXPORT AbstractCharacterContactListener : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(CharacterContactListener)
    QML_UNCREATABLE("abstract interface")
public:
    explicit AbstractCharacterContactListener(QObject *parent = nullptr);
    ~AbstractCharacterContactListener() override;

    struct CharacterContactSettings
    {
        bool canPushCharacter;
        bool canReceiveImpulses;
    };

    struct Contact
    {
        int bodyID2;
        QVector3D contactPosition;
        QVector3D contactNormal;
    };

    virtual void onAdjustBodyVelocity(int bodyID2, QVector3D &linearVelocity, QVector3D &angularVelocity) = 0;
    virtual void onContactAdded(const Contact &contact, CharacterContactSettings &settings) = 0;
    virtual void onContactSolve(const Contact &contact, const QVector3D &contactVelocity, const QVector3D &characterVelocity, QVector3D &newCharacterVelocity, bool isSlopeTooSteep) = 0;

protected:
    void registerContact(const Contact &contact);

private:
    QList<Contact> takeContacts();

    JPH::CharacterContactListener *getJoltCharacterContactListener() const;

    friend class CharacterContactListenerImpl;
    friend class CharacterVirtual;

    JPH::CharacterContactListener *m_impl = nullptr;

    QMutex m_mutex;
    QList<Contact> m_contacts;
};

#endif // CHARACTERCONTACTLISTENER_H
