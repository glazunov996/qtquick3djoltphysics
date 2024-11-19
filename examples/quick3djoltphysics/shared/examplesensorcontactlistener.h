#ifndef EXAMPLESENSORCONTACTLISTENER_H
#define EXAMPLESENSORCONTACTLISTENER_H

#include <QtQuick3DJoltPhysics/contactlistener.h>
#include <QObject>
#include <QList>
#include <QMutex>

class ExampleSensorContactListener : public AbstractContactListener
{
    Q_OBJECT
public:
    explicit ExampleSensorContactListener(QObject *parent = nullptr);

public:
    ValidateResult contactValidate(const BodyContact &bodyContact, const QVector3D &baseOffset, const CollideShapeResult &collisionResult) override;
    void contactAdded(const BodyContact &bodyContact, const ContactManifold &manifold, ContactSettings &settings) override;
    void contactPersisted(const BodyContact &bodyContact, const ContactManifold &manifold, ContactSettings &settings) override;
    void contactRemoved(const BodyContact &bodyContact) override;

    struct BodyAndCount {
        int bodyID;
        unsigned int count;

        bool operator < (const BodyAndCount &rhs) const { return bodyID < rhs.bodyID; }
    };

    Q_INVOKABLE QList<int> getBodiesInSensor(int sensor) const;

private:
    QHash<int, QList<BodyAndCount>> m_bodiesInSensor;
    mutable QMutex m_mutex;
};

#endif // EXAMPLESENSORCONTACTLISTENER_H
