#ifndef SENSOR_P_H
#define SENSOR_P_H

#include "body_p.h"

class Q_QUICK3DJOLTPHYSICS_EXPORT Sensor : public Body
{
    Q_OBJECT
    QML_NAMED_ELEMENT(Sensor)
public:
    explicit Sensor(QQuick3DNode *parent = nullptr);

protected:
    bool isSensor() const override { return true; }
};

#endif // SENSOR_P_H
