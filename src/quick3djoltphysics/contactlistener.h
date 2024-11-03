#ifndef CONTACTLISTENER_P_H
#define CONTACTLISTENER_P_H

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>

#include <QtQml/QQmlEngine>
#include <QVector3D>
#include <QList>
#include <QPair>
#include <QMutex>

namespace JPH {
class ContactListener;
}

class Q_QUICK3DJOLTPHYSICS_EXPORT AbstractContactListener : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(ContactListener)
    QML_UNCREATABLE("abstract interface")
public:
    explicit AbstractContactListener(QObject *parent = nullptr);
    ~AbstractContactListener() override;

    enum class ValidateResult {
        AcceptAllContactsForThisBodyPair,
        AcceptContact,
        RejectContact,
        RejectAllContactsForThisBodyPair
    };
    Q_ENUM(ValidateResult)

    struct CollideShapeResult
    {
        QVector3D contactPointOn1;
        QVector3D contactPointOn2;
        QVector3D penetrationAxis;
        float penetrationDepth;
        int bodyID2;
    };

    struct ContactManifold
    {
        QVector3D baseOffset;
        QVector3D worldSpaceNormal;
        float penetrationDepth;
        QList<QVector3D> relativeContactPointsOn1;
        QList<QVector3D> relativeContactPointsOn2;
    };

    struct ContactSettings
    {
        float combinedFriction;
        float combinedRestitution;
        float invMassScale1;
        float invInertiaScale1;
        float invMassScale2;
        float invInertiaScale2;
        bool isSensor;
        QVector3D relativeLinearSurfaceVelocity;
        QVector3D relativeAngularSurfaceVelocity;
    };

    struct BodyContact
    {
        int bodyID1;
        int bodyID2;
        bool isBodyID1Sensor;
        bool isBodyID2Sensor;
    };

    virtual ValidateResult contactValidate(const BodyContact &bodyContact, const QVector3D &baseOffset, const CollideShapeResult &collisionResult) = 0;
    virtual void contactAdded(const BodyContact &bodyContact, const ContactManifold &manifold, ContactSettings &settings) = 0;
    virtual void contactPersisted(const BodyContact &bodyContact, const ContactManifold &manifold, ContactSettings &settings) = 0;
    virtual void contactRemoved(const BodyContact &bodyContact) = 0;

protected:
    void registerBodyContact(const BodyContact &bodyContact);
    void registerEnteredBodyContact(const BodyContact &bodyContact);
    void registerExitedBodyContact(const BodyContact &bodyContact);

private:
    QList<BodyContact> takeBodyContacts();
    QList<BodyContact> takeEnteredBodyContacts();
    QList<BodyContact> takeExitedBodyContacts();

    JPH::ContactListener *getJoltContactListener() const;

    friend class ContactListenerImpl;
    friend class PhysicsSystem;

    JPH::ContactListener *m_impl = nullptr;

    QMutex m_mutex;
    QList<BodyContact> m_bodyContacts;
    QList<BodyContact> m_enteredBodyContacts;
    QList<BodyContact> m_exitedBodyContacts;
};

#endif // CONTACTLISTENER_P_H
