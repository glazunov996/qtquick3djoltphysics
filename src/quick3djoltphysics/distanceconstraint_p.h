#ifndef DISTANCECONSTRAINT_P_H
#define DISTANCECONSTRAINT_P_H

#include "abstractphysicsconstraint_p.h"
#include "body_p.h"
#include "springsettings_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQuick3D/private/qquick3dnode_p.h>
#include <QtQml/QQmlEngine>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Constraints/DistanceConstraint.h>

#include <QVector3D>

class Q_QUICK3DJOLTPHYSICS_EXPORT DistanceConstraint : public AbstractPhysicsConstraint
{
    Q_OBJECT
    Q_PROPERTY(QVector3D point1 READ point1 WRITE setPoint1 NOTIFY point1Changed)
    Q_PROPERTY(QVector3D point2 READ point2 WRITE setPoint2 NOTIFY point2Changed)
    Q_PROPERTY(Body *body1 READ body1 WRITE setBody1 NOTIFY body1Changed)
    Q_PROPERTY(Body *body2 READ body2 WRITE setBody2 NOTIFY body2Changed)
    Q_PROPERTY(float minDistance READ minDistance WRITE setMinDistance NOTIFY minDistanceChanged)
    Q_PROPERTY(float maxDistance READ maxDistance WRITE setMaxDistance NOTIFY maxDistanceChanged)
    Q_PROPERTY(SpringSettings *limitsSpringSettings READ limitsSpringSettings WRITE setLimitsSpringSettings NOTIFY limitsSpringSettingsChanged)
    QML_NAMED_ELEMENT(DistanceConstraint)
public:
    explicit DistanceConstraint(QQuick3DNode *parent = nullptr);
    ~DistanceConstraint() override;

    QVector3D point1() const;
    void setPoint1(const QVector3D &point);

    QVector3D point2() const;
    void setPoint2(const QVector3D &point);

    Body *body1() const;
    void setBody1(Body *body);

    Body *body2() const;
    void setBody2(Body *body);

    float minDistance() const;
    void setMinDistance(float minDistance);

    float maxDistance() const;
    void setMaxDistance(float maxDistance);

    SpringSettings *limitsSpringSettings() const;
    void setLimitsSpringSettings(SpringSettings *limitsSpringSettings);

signals:
    void point1Changed(const QVector3D &point);
    void point2Changed(const QVector3D &point);
    void body1Changed(Body *body1);
    void body2Changed(Body *body2);
    void minDistanceChanged(float minDistance);
    void maxDistanceChanged(float maxDistance);
    void limitsSpringSettingsChanged(SpringSettings *limitsSpringSettings);

protected:
    void updateJoltObject() override;

private:
    QVector3D m_point1;
    QVector3D m_point2;
    Body *m_body1 = nullptr;
    Body *m_body2 = nullptr;
    SpringSettings *m_limitsSpringSettings = nullptr;
    QMetaObject::Connection m_limitsSpringSettingsConnection;
    JPH::DistanceConstraintSettings m_constraintSettings;
};

#endif // DISTANCECONSTRAINT_P_H
