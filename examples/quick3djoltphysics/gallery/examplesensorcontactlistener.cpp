#include "examplesensorcontactlistener.h"

ExampleSensorContactListener::ExampleSensorContactListener(QObject *parent) : AbstractContactListener(parent) {}

ExampleSensorContactListener::ValidateResult ExampleSensorContactListener::contactValidate(const BodyContact &bodyContact, const QVector3D &baseOffset, const CollideShapeResult &collisionResult)
{
    return ValidateResult::AcceptAllContactsForThisBodyPair;
}

void ExampleSensorContactListener::contactAdded(const BodyContact &bodyContact, const ContactManifold &manifold, ContactSettings &settings)
{
    int sensorID;
    int bodyID;

    if (bodyContact.isBodyID1Sensor) {
        sensorID = bodyContact.bodyID1;
        bodyID = bodyContact.bodyID2;
    }
    else if (bodyContact.isBodyID2Sensor) {
        sensorID = bodyContact.bodyID2;
        bodyID = bodyContact.bodyID1;
    } else {
        return;
    }

    QMutexLocker locker(&m_mutex);
    if (!m_bodiesInSensor.contains(sensorID))
        m_bodiesInSensor[sensorID] = QList<BodyAndCount>();
    BodyAndCount bodyAndCount { bodyID, 1 };
    auto &bodiesInSensor = m_bodiesInSensor[sensorID];
    auto b = std::lower_bound(bodiesInSensor.begin(), bodiesInSensor.end(), bodyAndCount);
    if (b != bodiesInSensor.end() && b->bodyID == bodyID) {
        b->count++;
        return;
    }
    bodiesInSensor.insert(b, bodyAndCount);
    registerEnteredBodyContact(bodyContact);
}

void ExampleSensorContactListener::contactPersisted(const BodyContact &bodyContact, const ContactManifold &manifold, ContactSettings &settings)
{
}

void ExampleSensorContactListener::contactRemoved(const BodyContact &bodyContact)
{
    int sensorID;
    int bodyID;

    QMutexLocker locker(&m_mutex);
    if (m_bodiesInSensor.contains(bodyContact.bodyID1)) {
        sensorID = bodyContact.bodyID1;
        bodyID = bodyContact.bodyID2;
    } else if (m_bodiesInSensor.contains(bodyContact.bodyID2)) {
        sensorID = bodyContact.bodyID2;
        bodyID = bodyContact.bodyID1;
    } else {
        return;
    }

    BodyAndCount bodyAndCount { bodyID, 1 };
    auto &bodiesInSensor = m_bodiesInSensor[sensorID];
    auto b = std::lower_bound(bodiesInSensor.begin(), bodiesInSensor.end(), bodyAndCount);
    if (b != bodiesInSensor.end() && b->bodyID == bodyID) {
        Q_ASSERT(b->count > 0);
        b->count--;
        if (b->count == 0) {
            bodiesInSensor.erase(b);
            registerExitedBodyContact(bodyContact);
        }
        return;
    }
    Q_ASSERT_X(false, "Body pair not found", "");
}

QList<int> ExampleSensorContactListener::getBodiesInSensor(int sensor) const
{
    QMutexLocker locker(&m_mutex);

    QList<int> outBodiesInSensor;
    if (!m_bodiesInSensor.contains(sensor))
        return outBodiesInSensor;

    auto &bodiesInSensor = m_bodiesInSensor[sensor];
    for (const auto &b : bodiesInSensor)
       outBodiesInSensor.push_back(b.bodyID);

    return outBodiesInSensor;
}
