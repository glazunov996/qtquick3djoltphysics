#ifndef SAMPLELAYERS_H
#define SAMPLELAYERS_H

#include <QtQuick3DJoltPhysics/physicslayers.h>

class ExampleObjectLayerPairFilter : public AbstractObjectLayerPairFilter
{
    Q_OBJECT
public:
    ExampleObjectLayerPairFilter() = default;

    bool shouldCollide(quint16 obj1, quint16 obj2) const override;
};

class ExampleBroadPhaseLayer : public AbstractBroadPhaseLayer
{
    Q_OBJECT
public:
    ExampleBroadPhaseLayer();

    unsigned int getNumBroadPhaseLayers() const override;
    quint8 getBroadPhaseLayer(quint16 layer) const override;

private:
    quint8 m_objectToBroadPhase[4];
};

class ExampleObjectVsBroadPhaseLayerFilter : public AbstractObjectVsBroadPhaseLayerFilter
{
    Q_OBJECT
public:
    ExampleObjectVsBroadPhaseLayerFilter() = default;

    bool shouldCollide(quint16 layer1, quint8 layer2) const override;
};

#endif // SAMPLELAYERS_H
