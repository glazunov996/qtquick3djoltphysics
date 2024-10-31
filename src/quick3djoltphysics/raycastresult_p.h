#ifndef RAYCASTRESULT_P_H
#define RAYCASTRESULT_P_H

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>

#include <QVector3D>

#include "body_p.h"

class Q_QUICK3DJOLTPHYSICS_EXPORT RayCastResult
{
    Q_GADGET
    Q_PROPERTY(Body *body READ body CONSTANT)
    Q_PROPERTY(QVector3D position READ position CONSTANT)
    Q_PROPERTY(QVector3D normal READ normal CONSTANT)
    Q_PROPERTY(float fraction READ fraction CONSTANT)
public:
    RayCastResult();
    RayCastResult(Body *body, const QVector3D &position, const QVector3D &normal, float fraction);

    Body *body() const;
    QVector3D position() const;
    QVector3D normal() const;
    float fraction() const;

private:
    Body *m_body = nullptr;
    QVector3D m_position;
    QVector3D m_normal;
    float m_fraction = 1.0f;
};

Q_DECLARE_METATYPE(RayCastResult)

#endif // RAYCASTRESULT_P_H
