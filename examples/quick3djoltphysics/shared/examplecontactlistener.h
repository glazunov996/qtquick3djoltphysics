#ifndef EXAMPLECONTACTLISTENER_P_H
#define EXAMPLECONTACTLISTENER_P_H

#include <QtQuick3DJoltPhysics/contactlistener.h>
#include <QObject>
#include <QList>

class ExampleContactListener : public AbstractContactListener
{
    Q_OBJECT
    Q_PROPERTY(QList<int> bodyIDs READ bodyIDs WRITE setBodyIDs NOTIFY bodyIDsChanged)
public:
    explicit ExampleContactListener(QObject *parent = nullptr);

    QList<int> bodyIDs() const;
    void setBodyIDs(const QList<int> &bodyIDs);

signals:
    void bodyIDsChanged(const QList<int> &bodyIDs);

public:
    ValidateResult contactValidate(const BodyContact &bodyContact, const QVector3D &baseOffset, const CollideShapeResult &collisionResult) override;
    void contactAdded(const BodyContact &bodyContact, const ContactManifold &manifold, ContactSettings &settings) override;
    void contactPersisted(const BodyContact &bodyContact, const ContactManifold &manifold, ContactSettings &settings) override;
    void contactRemoved(const BodyContact &bodyContact) override;

private:
    QList<int> m_bodyIDs;
};

#endif // EXAMPLECONTACTLISTENER_P_H
