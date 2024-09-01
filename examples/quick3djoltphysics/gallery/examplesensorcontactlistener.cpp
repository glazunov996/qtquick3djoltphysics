#include "examplesensorcontactlistener.h"

ExampleSensorContactListener::ExampleSensorContactListener(QObject *parent) : AbstractContactListener(parent) {}

QList<int> ExampleSensorContactListener::sensorIDs() const
{
    return m_sensorIDs;
}

void ExampleSensorContactListener::setSensorIDs(const QList<int> &sensorIDs)
{
    if (m_sensorIDs == sensorIDs)
        return;

    m_sensorIDs = sensorIDs;
    m_bodiesInSensor.resize(m_sensorIDs.size());
    emit sensorIDsChanged(m_sensorIDs);
}

ExampleSensorContactListener::ValidateResult ExampleSensorContactListener::contactValidate(int body1ID, int body2ID, const QVector3D &baseOffset, const CollideShapeResult &collisionResult)
{
    return ValidateResult::AcceptAllContactsForThisBodyPair;
}

void ExampleSensorContactListener::contactAdded(int body1ID, int body2ID, const ContactManifold &manifold, ContactSettings &settings)
{
    for (int sensor = 0; sensor < m_sensorIDs.length(); ++sensor) {
        int sensorID = m_sensorIDs[sensor];

        int bodyID;

        if (body1ID == sensorID)
            bodyID = body2ID;
        else if (body2ID == sensorID)
            bodyID = body1ID;
        else
            continue;

        QMutexLocker locker(&m_mutex);

        BodyAndCount bodyAndCount { bodyID, 1 };
        auto &bodiesInSensor = m_bodiesInSensor[sensor];
        auto b = std::lower_bound(bodiesInSensor.begin(), bodiesInSensor.end(), bodyAndCount);
        if (b != bodiesInSensor.end() && b->bodyID == bodyID) {
            b->count++;
            return;
        }
        bodiesInSensor.insert(b, bodyAndCount);
    }
}

void ExampleSensorContactListener::contactPersisted(int body1ID, int body2ID, const ContactManifold &manifold, ContactSettings &settings)
{
}

void ExampleSensorContactListener::contactRemoved(int body1ID, int subShapeID1, int body2ID, int subShapeID2)
{
    for (int sensor = 0; sensor < m_sensorIDs.length(); ++sensor) {
        int sensorID = m_sensorIDs[sensor];

        int bodyID;

        if (body1ID == sensorID)
            bodyID = body2ID;
        else if (body2ID == sensorID)
            bodyID = body1ID;
        else
            continue;

        QMutexLocker locker(&m_mutex);

        BodyAndCount bodyAndCount { bodyID, 1 };
        auto &bodiesInSensor = m_bodiesInSensor[sensor];
        auto b = std::lower_bound(bodiesInSensor.begin(), bodiesInSensor.end(), bodyAndCount);
        if (b != bodiesInSensor.end() && b->bodyID == bodyID) {
            Q_ASSERT(b->count > 0);
            b->count--;
            if (b->count == 0)
                bodiesInSensor.erase(b);
            return;
        }
        Q_ASSERT_X(false, "Body pair not found", "");
    }
}

QList<int> ExampleSensorContactListener::getBodiesInSensor(int sensor) const
{
    QMutexLocker locker(&m_mutex);

    QList<int> outBodiesInSensor;
    auto &bodiesInSensor = m_bodiesInSensor[sensor];
    for (const auto &b : bodiesInSensor)
       outBodiesInSensor.push_back(b.bodyID);

    return outBodiesInSensor;
}
