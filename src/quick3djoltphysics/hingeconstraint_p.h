#ifndef HINGECONSTRAINT_P_H
#define HINGECONSTRAINT_P_H

#include "abstractphysicsconstraint_p.h"
#include "body_p.h"
#include "springsettings_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQuick3D/private/qquick3dnode_p.h>
#include <QtQml/QQmlEngine>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Constraints/HingeConstraint.h>

#include <QVector3D>

class Q_QUICK3DJOLTPHYSICS_EXPORT HingeConstraint : public AbstractPhysicsConstraint
{
    Q_OBJECT
    Q_PROPERTY(QVector3D point1 READ point1 WRITE setPoint1 NOTIFY point1Changed)
    Q_PROPERTY(QVector3D point2 READ point2 WRITE setPoint2 NOTIFY point2Changed)
    Q_PROPERTY(Body *body1 READ body1 WRITE setBody1 NOTIFY body1Changed)
    Q_PROPERTY(Body *body2 READ body2 WRITE setBody2 NOTIFY body2Changed)
    Q_PROPERTY(QVector3D hingeAxis1 READ hingeAxis1 WRITE setHingeAxis1 NOTIFY hingeAxis1Changed)
    Q_PROPERTY(QVector3D hingeAxis2 READ hingeAxis2 WRITE setHingeAxis2 NOTIFY hingeAxis2Changed)
    Q_PROPERTY(QVector3D normalAxis1 READ normalAxis1 WRITE setNormalAxis1 NOTIFY normalAxis1Changed)
    Q_PROPERTY(QVector3D normalAxis2 READ normalAxis2 WRITE setNormalAxis2 NOTIFY normalAxis2Changed)
    Q_PROPERTY(float limitsMin READ limitsMin WRITE setLimitsMin NOTIFY limitsMinChanged)
    Q_PROPERTY(float limitsMax READ limitsMax WRITE setLimitsMax NOTIFY limitsMaxChanged)
    Q_PROPERTY(SpringSettings *limitsSpringSettings READ limitsSpringSettings WRITE setLimitsSpringSettings NOTIFY limitsSpringSettingsChanged)
    Q_PROPERTY(float maxFrictionTorque READ maxFrictionTorque WRITE setMaxFrictionTorque NOTIFY maxFrictionTorqueChanged)
    QML_NAMED_ELEMENT(HingeConstraint)
public:
    explicit HingeConstraint(QQuick3DNode *parent = nullptr);
    ~HingeConstraint() override;

    QVector3D point1() const;
    void setPoint1(const QVector3D &point);

    QVector3D point2() const;
    void setPoint2(const QVector3D &point);

    Body *body1() const;
    void setBody1(Body *body);

    Body *body2() const;
    void setBody2(Body *body);

    QVector3D hingeAxis1() const;
    void setHingeAxis1(const QVector3D &hingeAxis1);

    QVector3D hingeAxis2() const;
    void setHingeAxis2(const QVector3D &hingeAxis2);

    QVector3D normalAxis1() const;
    void setNormalAxis1(const QVector3D &normalAxis1);

    QVector3D normalAxis2() const;
    void setNormalAxis2(const QVector3D &normalAxis2);

    float limitsMin() const;
    void setLimitsMin(float limitsMin);

    float limitsMax() const;
    void setLimitsMax(float limitsMax);

    SpringSettings *limitsSpringSettings() const;
    void setLimitsSpringSettings(SpringSettings *limitsSpringSettings);

    float maxFrictionTorque() const;
    void setMaxFrictionTorque(float maxFrictionTorque);

signals:
    void point1Changed(const QVector3D &point);
    void point2Changed(const QVector3D &point);
    void body1Changed(Body *body1);
    void body2Changed(Body *body2);
    void hingeAxis1Changed(const QVector3D &hingeAxis1);
    void hingeAxis2Changed(const QVector3D &hingeAxis2);
    void normalAxis1Changed(const QVector3D &normalAxis1);
    void normalAxis2Changed(const QVector3D &normalAxis2);
    void limitsMinChanged(float limitsMin);
    void limitsMaxChanged(float limitsMax);
    void limitsSpringSettingsChanged(SpringSettings *limitsSpringSettings);
    void maxFrictionTorqueChanged(float maxFrictionTorque);

protected:
    void updateJoltObject() override;

private:
    QVector3D m_point1;
    QVector3D m_point2;
    Body *m_body1 = nullptr;
    Body *m_body2 = nullptr;
    QVector3D m_hingeAxis1 = QVector3D(0, 1, 0);
    QVector3D m_hingeAxis2 = QVector3D(0, 1, 0);
    QVector3D m_normalAxis1 = QVector3D(1, 0, 0);
    QVector3D m_normalAxis2 = QVector3D(1, 0, 0);
    float m_limitsMin = -180.0;
    float m_limitsMax = 180.0;
    SpringSettings *m_limitsSpringSettings = nullptr;
    QMetaObject::Connection m_limitsSpringSettingsConnection;
    JPH::HingeConstraintSettings m_constraintSettings;
};

#endif // HINGECONSTRAINT_P_H
