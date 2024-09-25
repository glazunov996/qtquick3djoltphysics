#ifndef PHYSICSLAYERS_P_H
#define PHYSICSLAYERS_P_H

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>

#include <QtQml/QQmlEngine>

namespace JPH {
class ObjectLayerPairFilter;
class BroadPhaseLayerInterface;
class ObjectVsBroadPhaseLayerFilter;
}

class Q_QUICK3DJOLTPHYSICS_EXPORT AbstractObjectLayerPairFilter : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(ObjectLayerPairFilter)
    QML_UNCREATABLE("abstract interface")
public:
    explicit AbstractObjectLayerPairFilter(QObject *parent = nullptr);
    ~AbstractObjectLayerPairFilter() override;

    virtual bool shouldCollide(quint16 obj1, quint16 obj2) const = 0;

private:
    const JPH::ObjectLayerPairFilter &impl() const { return *m_impl; }

    friend class PhysicsSystem;
    JPH::ObjectLayerPairFilter *m_impl = nullptr;
};

class Q_QUICK3DJOLTPHYSICS_EXPORT AbstractBroadPhaseLayer : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(BroadPhaseLayer)
    QML_UNCREATABLE("abstract interface")
public:
    explicit AbstractBroadPhaseLayer(QObject *parent = nullptr);
    ~AbstractBroadPhaseLayer() override;

    virtual unsigned int getNumBroadPhaseLayers() const = 0;
    virtual quint8 getBroadPhaseLayer(quint16 layer) const = 0;

private:
    const JPH::BroadPhaseLayerInterface &impl() const { return *m_impl; }

    friend class PhysicsSystem;
    JPH::BroadPhaseLayerInterface *m_impl = nullptr;
};

class Q_QUICK3DJOLTPHYSICS_EXPORT AbstractObjectVsBroadPhaseLayerFilter : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(ObjectVsBroadPhaseLayerFilter)
    QML_UNCREATABLE("abstract interface")
public:
    explicit AbstractObjectVsBroadPhaseLayerFilter(QObject *parent = nullptr);
    ~AbstractObjectVsBroadPhaseLayerFilter() override;

    virtual bool shouldCollide(quint16 layer1, quint8 layer2) const = 0;

private:
    const JPH::ObjectVsBroadPhaseLayerFilter &impl() const { return *m_impl; }

    friend class PhysicsSystem;
    JPH::ObjectVsBroadPhaseLayerFilter *m_impl = nullptr;
};

#endif // PHYSICSLAYERS_P_H
