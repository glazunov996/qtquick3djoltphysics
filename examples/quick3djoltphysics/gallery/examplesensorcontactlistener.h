#ifndef EXAMPLESENSORCONTACTLISTENER_H
#define EXAMPLESENSORCONTACTLISTENER_H

#include <QtQuick3DJoltPhysics/contactlistener.h>
#include <QObject>
#include <QList>
#include <QMutex>

class ExampleSensorContactListener : public AbstractContactListener
{
    Q_OBJECT
    Q_PROPERTY(QList<int> sensorIDs READ sensorIDs WRITE setSensorIDs NOTIFY sensorIDsChanged)
    QML_NAMED_ELEMENT(ExampleSensorContactListener)
public:
    explicit ExampleSensorContactListener(QObject *parent = nullptr);

    QList<int> sensorIDs() const;

public slots:
    void setSensorIDs(const QList<int> &sensorIDs);

signals:
    void sensorIDsChanged(const QList<int> &sensorIDs);

public:
    ValidateResult contactValidate(int body1ID, int body2ID, const QVector3D &baseOffset, const CollideShapeResult &collisionResult) override;
    void contactAdded(int body1ID, int body2ID, const ContactManifold &manifold, ContactSettings &settings) override;
    void contactPersisted(int body1ID, int body2ID, const ContactManifold &manifold, ContactSettings &settings) override;
    void contactRemoved(int body1ID, int subShapeID1, int body2ID, int subShapeID2) override;

    struct BodyAndCount {
        int bodyID;
        unsigned int count;

        bool operator < (const BodyAndCount &rhs) const { return bodyID < rhs.bodyID; }
    };

    Q_INVOKABLE QList<int> getBodiesInSensor(int sensor) const;

private:
    QList<int> m_sensorIDs;
    QList<QList<BodyAndCount>> m_bodiesInSensor;
    mutable QMutex m_mutex;
};

#endif // EXAMPLESENSORCONTACTLISTENER_H
