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
    QML_NAMED_ELEMENT(Shape)
    QML_UNCREATABLE("abstract interface")
public:
    explicit AbstractShape(QQuick3DNode *parent = nullptr);
    virtual ~AbstractShape();

    float density() const;
    void setDensity(float density);

    virtual JPH::Ref<JPH::Shape> getJoltShape();

signals:
    void changed();
    void densityChanged(float density);

protected:
    virtual void createJoltShape() = 0;
    void updateJoltShape();
    void handleShapeChange();

    JPH::Ref<JPH::Shape> m_innerJoltShape = nullptr;
    JPH::Ref<JPH::Shape> m_joltShape = nullptr;
    bool m_shapeInitialized = false;

private:
    void updateJoltShapeDensity();

    float m_density = 1000.0f;

    friend class Body;
    friend class StaticCompoundShape;
};

#endif // ABSTRACTSHAPE_P_H
