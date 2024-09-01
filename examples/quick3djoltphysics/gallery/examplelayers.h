#ifndef SAMPLELAYERS_H
#define SAMPLELAYERS_H

#include <QtQuick3DJoltPhysics/physicslayers.h>

class ExampleObjectLayerPairFilter : public AbstractObjectLayerPairFilter
{
    Q_OBJECT
    QML_NAMED_ELEMENT(ExampleObjectLayerPairFilter)
public:
    ExampleObjectLayerPairFilter() = default;

    bool shouldCollide(unsigned int obj1, unsigned int obj2) const override;
};

class ExampleBroadPhaseLayer : public AbstractBroadPhaseLayer
{
    Q_OBJECT
    QML_NAMED_ELEMENT(ExampleBroadPhaseLayer)
public:
    ExampleBroadPhaseLayer();

    unsigned int getNumBroadPhaseLayers() const override;
    unsigned int getBroadPhaseLayer(unsigned int layer) const override;

private:
    unsigned int m_objectToBroadPhase[4];
};

class ExampleObjectVsBroadPhaseLayerFilter : public AbstractObjectVsBroadPhaseLayerFilter
{
    Q_OBJECT
    QML_NAMED_ELEMENT(ExampleObjectVsBroadPhaseLayerFilter)
public:
    ExampleObjectVsBroadPhaseLayerFilter() = default;

    bool shouldCollide(unsigned int layer1, unsigned int layer2) const override;
};

#endif // SAMPLELAYERS_H
