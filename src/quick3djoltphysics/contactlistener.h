#ifndef CONTACTLISTENER_P_H
#define CONTACTLISTENER_P_H

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>

#include <QtQml/QQmlEngine>
#include <QtGui/QVector3D>
#include <QtCore/QList>

namespace JPH {
class ContactListener;
}

struct Q_QUICK3DJOLTPHYSICS_EXPORT CollideShapeResult
{
    QVector3D contactPointOn1;
    QVector3D contactPointOn2;
    QVector3D penetrationAxis;
    float penetrationDepth;
    int subShapeID1;
    int subShapeID2;
    int bodyID2;
    QList<QVector3D> shape1Face;
    QList<QVector3D> shape2Face;
};

struct Q_QUICK3DJOLTPHYSICS_EXPORT ContactManifold
{
    QVector3D baseOffset;
    QVector3D worldSpaceNormal;
    float penetrationDepth;
    int subShapeID1;
    int subShapeID2;
    QList<QVector3D> relativeContactPointsOn1;
    QList<QVector3D> relativeContactPointsOn2;
};

struct Q_QUICK3DJOLTPHYSICS_EXPORT ContactSettings
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

    virtual ValidateResult contactValidate(int body1ID, int body2ID, const QVector3D &baseOffset, const CollideShapeResult &collisionResult) = 0;
    virtual void contactAdded(int body1ID, int body2ID, const ContactManifold &manifold, ContactSettings &settings) = 0;
    virtual void contactPersisted(int body1ID, int body2ID, const ContactManifold &manifold, ContactSettings &settings) = 0;
    virtual void contactRemoved(int body1ID, int subShapeID1, int body2ID, int subShapeID2) = 0;

private:
    JPH::ContactListener *getJoltContactListener() const;

    friend class ContactListenerImpl;
    friend class PhysicsSystem;

    JPH::ContactListener *m_impl = nullptr;
};

Q_DECLARE_METATYPE(ContactManifold)

#endif // CONTACTLISTENER_P_H
