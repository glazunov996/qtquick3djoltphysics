#ifndef SWINGTWISTCONSTRAINT_P_H
#define SWINGTWISTCONSTRAINT_P_H

#include "abstractphysicsconstraint_p.h"
#include "body_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQuick3D/private/qquick3dnode_p.h>
#include <QtQml/QQmlEngine>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Constraints/SwingTwistConstraint.h>

#include <QVector3D>

class Q_QUICK3DJOLTPHYSICS_EXPORT SwingTwistConstraint : public AbstractPhysicsConstraint
{
    Q_OBJECT
    Q_PROPERTY(Body *body1 READ body1 WRITE setBody1 NOTIFY body1Changed)
    Q_PROPERTY(Body *body2 READ body2 WRITE setBody2 NOTIFY body2Changed)
    Q_PROPERTY(QVector3D position1 READ position1 WRITE setPosition1 NOTIFY position1Changed)
    Q_PROPERTY(QVector3D position2 READ position2 WRITE setPosition2 NOTIFY position2Changed)
    Q_PROPERTY(QVector3D twistAxis1 READ twistAxis1 WRITE setTwistAxis1 NOTIFY twistAxis1Changed)
    Q_PROPERTY(QVector3D twistAxis2 READ twistAxis2 WRITE setTwistAxis2 NOTIFY twistAxis2Changed)
    Q_PROPERTY(QVector3D planeAxis1 READ planeAxis1 WRITE setPlaneAxis1 NOTIFY planeAxis1Changed)
    Q_PROPERTY(QVector3D planeAxis2 READ planeAxis2 WRITE setPlaneAxis2 NOTIFY planeAxis2Changed)
    Q_PROPERTY(SwingType swingType READ swingType WRITE setSwingType NOTIFY swingTypeChanged)
    Q_PROPERTY(float normalHalfConeAngle READ normalHalfConeAngle WRITE setNormalHalfConeAngle NOTIFY normalHalfConeAngleChanged)
    Q_PROPERTY(float planeHalfConeAngle READ planeHalfConeAngle WRITE setPlaneHalfConeAngle NOTIFY planeHalfConeAngleChanged)
    Q_PROPERTY(float twistMinAngle READ twistMinAngle WRITE setTwistMinAngle NOTIFY twistMinAngleChanged)
    Q_PROPERTY(float twistMaxAngle READ twistMaxAngle WRITE setTwistMaxAngle NOTIFY twistMaxAngleChanged)
    Q_PROPERTY(float maxFrictionTorque READ maxFrictionTorque WRITE setMaxFrictionTorque NOTIFY maxFrictionTorqueChanged)
    QML_NAMED_ELEMENT(SwingTwistConstraint)
public:
    explicit SwingTwistConstraint(QQuick3DNode *parent = nullptr);
    ~SwingTwistConstraint() override;

    enum class SwingType {
        Cone,
        Pyramid,
    };
    Q_ENUM(SwingType)

    Body *body1() const;
    void setBody1(Body *body);

    Body *body2() const;
    void setBody2(Body *body);

    QVector3D position1() const;
    void setPosition1(const QVector3D &position1);

    QVector3D position2() const;
    void setPosition2(const QVector3D &position2);

    QVector3D twistAxis1() const;
    void setTwistAxis1(const QVector3D &twistAxis1);

    QVector3D twistAxis2() const;
    void setTwistAxis2(const QVector3D &twistAxis2);

    QVector3D planeAxis1() const;
    void setPlaneAxis1(const QVector3D &planeAxis1);

    QVector3D planeAxis2() const;
    void setPlaneAxis2(const QVector3D &planeAxis2);

    SwingType swingType() const;
    void setSwingType(SwingType swingType);

    float normalHalfConeAngle() const;
    void setNormalHalfConeAngle(float normalHalfConeAngle);

    float planeHalfConeAngle() const;
    void setPlaneHalfConeAngle(float planeHalfConeAngle);

    float twistMinAngle() const;
    void setTwistMinAngle(float twistMinAngle);

    float twistMaxAngle() const;
    void setTwistMaxAngle(float twistMaxAngle);

    float maxFrictionTorque() const;
    void setMaxFrictionTorque(float maxFrictionTorque);

signals:
    void body1Changed(Body *body1);
    void body2Changed(Body *body2);
    void position1Changed(const QVector3D &position1);
    void position2Changed(const QVector3D &position2);
    void twistAxis1Changed(const QVector3D &twistAxis1);
    void twistAxis2Changed(const QVector3D &twistAxis2);
    void planeAxis1Changed(const QVector3D &planeAxis1);
    void planeAxis2Changed(const QVector3D &planeAxis2);
    void swingTypeChanged(SwingType swingType);
    void normalHalfConeAngleChanged(float normalHalfConeAngle);
    void planeHalfConeAngleChanged(float planeHalfConeAngle);
    void twistMinAngleChanged(float twistMinAngle);
    void twistMaxAngleChanged(float twistMaxAngle);
    void maxFrictionTorqueChanged(float maxFrictionTorque);

protected:
    void updateJoltObject() override;

private:
    Body *m_body1 = nullptr;
    Body *m_body2 = nullptr;
    QVector3D m_position1;
    QVector3D m_position2;
    QVector3D m_twistAxis1 = QVector3D(1, 0, 0);
    QVector3D m_twistAxis2 = QVector3D(1, 0, 0);
    QVector3D m_planeAxis1 = QVector3D(0, 1, 0);
    QVector3D m_planeAxis2 = QVector3D(0, 1, 0);
    float m_normalHalfConeAngle = 0.0f;
    float m_planeHalfConeAngle = 0.0f;
    float m_twistMinAngle = 0.0f;
    float m_twistMaxAngle = 0.0f;
    JPH::SwingTwistConstraintSettings m_constraintSettings;
};

#endif // SWINGTWISTCONSTRAINT_P_H
