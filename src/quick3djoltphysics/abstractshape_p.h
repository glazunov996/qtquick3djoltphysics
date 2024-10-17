#ifndef ABSTRACTSHAPE_P_H
#define ABSTRACTSHAPE_P_H

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQuick3D/private/qquick3dnode_p.h>
#include <QtQml/QQmlEngine>

#include <Jolt/Jolt.h>
#include <Jolt/Core/Reference.h>
#include <Jolt/Physics/Collision/Shape/Shape.h>

class Q_QUICK3DJOLTPHYSICS_EXPORT AbstractShape : public QQuick3DNode
{
    Q_OBJECT
    Q_PROPERTY(float density READ density WRITE setDensity NOTIFY densityChanged)
    Q_PROPERTY(QVector3D offsetCenterOfMass READ offsetCenterOfMass WRITE setOffsetCenterOfMass NOTIFY offsetCenterOfMassChanged)
    QML_NAMED_ELEMENT(Shape)
    QML_UNCREATABLE("abstract interface")
public:
    explicit AbstractShape(QQuick3DNode *parent = nullptr);
    virtual ~AbstractShape();

    float density() const;
    void setDensity(float density);

    QVector3D offsetCenterOfMass() const;
    void setOffsetCenterOfMass(const QVector3D &offsetCenterOfMass);

signals:
    void densityChanged(float density);
    void offsetCenterOfMassChanged(const QVector3D &offsetCenterOfMass);
    void changed();

protected:
    virtual void createJoltShape() = 0;
    virtual JPH::Ref<JPH::Shape> getJoltShape();

    void updateJoltShape();

    JPH::Ref<JPH::Shape> m_shape = nullptr;
    bool m_shapeInitialized = false;

private slots:
    void handleNodeChanged();

private:
    float m_density = 1000.0f;
    QVector3D m_offsetCenterOfMass;

    friend class Body;
    friend class StaticCompoundShape;
    friend class CharacterVirtual;
    friend class Character;
};

#endif // ABSTRACTSHAPE_P_H
