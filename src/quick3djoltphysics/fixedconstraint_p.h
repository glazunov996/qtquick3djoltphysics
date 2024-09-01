#ifndef FIXEDCONSTRAINT_P_H
#define FIXEDCONSTRAINT_P_H

#include "abstractphysicsconstraint_p.h"
#include "body_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQuick3D/private/qquick3dnode_p.h>
#include <QtQml/QQmlEngine>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Constraints/FixedConstraint.h>

#include <QVector3D>

class Q_QUICK3DJOLTPHYSICS_EXPORT FixedConstraint : public AbstractPhysicsConstraint
{
    Q_OBJECT
    Q_PROPERTY(QVector3D point1 READ point1 WRITE setPoint1 NOTIFY point1Changed)
    Q_PROPERTY(QVector3D point2 READ point2 WRITE setPoint2 NOTIFY point2Changed)
    Q_PROPERTY(Body *body1 READ body1 WRITE setBody1 NOTIFY body1Changed)
    Q_PROPERTY(Body *body2 READ body2 WRITE setBody2 NOTIFY body2Changed)
    Q_PROPERTY(QVector3D axisX1 READ axisX1 WRITE setAxisX1 NOTIFY axisX1Changed)
    Q_PROPERTY(QVector3D axisX2 READ axisX2 WRITE setAxisX2 NOTIFY axisX2Changed)
    Q_PROPERTY(QVector3D axisY1 READ axisY1 WRITE setAxisY1 NOTIFY axisY1Changed)
    Q_PROPERTY(QVector3D axisY2 READ axisY2 WRITE setAxisY2 NOTIFY axisY2Changed)
    QML_NAMED_ELEMENT(FixedConstraint)
public:
    explicit FixedConstraint(QQuick3DNode *parent = nullptr);
    ~FixedConstraint() override;

    QVector3D point1() const;
    void setPoint1(const QVector3D &point);

    QVector3D point2() const;
    void setPoint2(const QVector3D &point);

    Body *body1() const;
    void setBody1(Body *body);

    Body *body2() const;
    void setBody2(Body *body);

    QVector3D axisX1() const;
    void setAxisX1(const QVector3D &axisX1);

    QVector3D axisX2() const;
    void setAxisX2(const QVector3D &axisX2);

    QVector3D axisY1() const;
    void setAxisY1(const QVector3D &axisY1);

    QVector3D axisY2() const;
    void setAxisY2(const QVector3D &axisY2);

signals:
    void point1Changed(const QVector3D &point);
    void point2Changed(const QVector3D &point);
    void body1Changed(Body *body1);
    void body2Changed(Body *body2);
    void axisX1Changed(const QVector3D &axisX);
    void axisX2Changed(const QVector3D &axisX);
    void axisY1Changed(const QVector3D &axisY);
    void axisY2Changed(const QVector3D &axisY);

protected:
    void updateJoltObject() override;

private:
    QVector3D m_point1;
    QVector3D m_point2;
    Body *m_body1 = nullptr;
    Body *m_body2 = nullptr;
    QVector3D m_axisX1 = QVector3D(1, 0, 0);
    QVector3D m_axisX2 = QVector3D(1, 0, 0);
    QVector3D m_axisY1 = QVector3D(0, 1, 0);
    QVector3D m_axisY2 = QVector3D(0, 1, 0);
    JPH::FixedConstraintSettings m_constraintSettings;
};

#endif // FIXEDCONSTRAINT_P_H
