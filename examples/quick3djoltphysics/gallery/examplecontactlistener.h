#ifndef EXAMPLECONTACTLISTENER_P_H
#define EXAMPLECONTACTLISTENER_P_H

#include <QtQuick3DJoltPhysics/contactlistener.h>
#include <QObject>
#include <QList>

class ExampleContactListener : public AbstractContactListener
{
    Q_OBJECT
    Q_PROPERTY(QList<int> bodyIDs READ bodyIDs WRITE setBodyIDs NOTIFY bodyIDsChanged)
    QML_NAMED_ELEMENT(ExampleContactListener)
public:
    explicit ExampleContactListener(QObject *parent = nullptr);

    QList<int> bodyIDs() const;
    void setBodyIDs(const QList<int> &bodyIDs);

signals:
    void bodyIDsChanged(const QList<int> &bodyIDs);

public:
    ValidateResult contactValidate(int body1ID, int body2ID, const QVector3D &baseOffset, const CollideShapeResult &collisionResult) override;
    void contactAdded(int body1ID, int body2ID, const ContactManifold &manifold, ContactSettings &settings) override;
    void contactPersisted(int body1ID, int body2ID, const ContactManifold &manifold, ContactSettings &settings) override;
    void contactRemoved(int body1ID, int subShapeID1, int body2ID, int subShapeID2) override;

private:
    QList<int> m_bodyIDs;
};

#endif // EXAMPLECONTACTLISTENER_P_H
