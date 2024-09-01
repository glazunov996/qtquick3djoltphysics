#include "physicslayers.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter
{
public:
    ObjectLayerPairFilterImpl(const AbstractObjectLayerPairFilter *d) : m_d(d) {}

    bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override
    {
        return m_d->shouldCollide(static_cast<unsigned int>(inObject1), static_cast<unsigned int>(inObject2));
    }

private:
    const AbstractObjectLayerPairFilter *m_d = nullptr;
};

class BroadPhaseLayerImpl : public JPH::BroadPhaseLayerInterface
{
public:
    BroadPhaseLayerImpl(const AbstractBroadPhaseLayer *d) : m_d(d) {}

    unsigned int GetNumBroadPhaseLayers() const override
    {
        return m_d->getNumBroadPhaseLayers();
    }

    JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override
    {
        return static_cast<JPH::BroadPhaseLayer>(
            m_d->getBroadPhaseLayer(static_cast<unsigned int>(inLayer)));
    }

private:
    const AbstractBroadPhaseLayer *m_d = nullptr;
};

class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter
{
public:
    ObjectVsBroadPhaseLayerFilterImpl(const AbstractObjectVsBroadPhaseLayerFilter *d) : m_d(d) {}

    bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override
    {
        return m_d->shouldCollide(static_cast<unsigned int>(inLayer1), static_cast<unsigned int>(inLayer2.GetValue()));
    }

private:
    const AbstractObjectVsBroadPhaseLayerFilter *m_d = nullptr;
};

AbstractObjectLayerPairFilter::AbstractObjectLayerPairFilter(QObject *parent)
    : QObject(parent)
    , m_impl(new ObjectLayerPairFilterImpl(this))
{
}

AbstractObjectLayerPairFilter::~AbstractObjectLayerPairFilter()
{
    delete m_impl;
    m_impl = nullptr;
}

AbstractBroadPhaseLayer::AbstractBroadPhaseLayer(QObject *parent)
    : QObject(parent)
    , m_impl(new BroadPhaseLayerImpl(this))
{
}

AbstractBroadPhaseLayer::~AbstractBroadPhaseLayer()
{
    delete m_impl;
    m_impl = nullptr;
}

AbstractObjectVsBroadPhaseLayerFilter::AbstractObjectVsBroadPhaseLayerFilter(QObject *parent)
    : QObject(parent)
    , m_impl(new ObjectVsBroadPhaseLayerFilterImpl(this))
{
}

AbstractObjectVsBroadPhaseLayerFilter::~AbstractObjectVsBroadPhaseLayerFilter()
{
    delete m_impl;
    m_impl = nullptr;
}
