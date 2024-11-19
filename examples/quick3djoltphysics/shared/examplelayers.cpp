#include "examplelayers.h"

namespace Layers {
static constexpr int NON_MOVING = 0;
static constexpr int MOVING = 1;
static constexpr int DEBRIS = 2;
static constexpr int SENSOR = 3;
}

bool ExampleObjectLayerPairFilter::shouldCollide(quint16 obj1, quint16 obj2) const
{
    switch (obj1) {
    case Layers::NON_MOVING:
        return obj2 == Layers::MOVING || obj2 == Layers::DEBRIS;
    case Layers::MOVING:
        return obj2 == Layers::NON_MOVING || obj2 == Layers::MOVING || obj2 == Layers::SENSOR;
    case Layers::DEBRIS:
        return obj2 == Layers::NON_MOVING;
    case Layers::SENSOR:
        return obj2 == Layers::MOVING;
    default:
        Q_ASSERT(false);
        return false;
    }
}

ExampleBroadPhaseLayer::ExampleBroadPhaseLayer()
{
    m_objectToBroadPhase[0] = Layers::NON_MOVING;
    m_objectToBroadPhase[1] = Layers::MOVING;
    m_objectToBroadPhase[2] = Layers::DEBRIS;
    m_objectToBroadPhase[3] = Layers::SENSOR;
}

unsigned int ExampleBroadPhaseLayer::getNumBroadPhaseLayers() const
{
    return 4;
}

quint8 ExampleBroadPhaseLayer::getBroadPhaseLayer(quint16 layer) const
{
    return m_objectToBroadPhase[layer];
}

bool ExampleObjectVsBroadPhaseLayerFilter::shouldCollide(quint16 layer1, quint8 layer2) const
{
    switch (layer1) {
    case Layers::NON_MOVING:
        return layer2 == Layers::MOVING || layer2 == Layers::DEBRIS;
    case Layers::MOVING:
        return layer2 == Layers::NON_MOVING || layer2 == Layers::MOVING || layer2 == Layers::SENSOR;
    case Layers::DEBRIS:
        return layer2 == Layers::NON_MOVING;
    case Layers::SENSOR:
        return layer2 == Layers::MOVING;
    default:
        Q_ASSERT(false);
        return false;
    }
}
