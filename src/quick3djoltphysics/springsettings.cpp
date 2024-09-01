#include "springsettings_p.h"

static JPH::ESpringMode convertToJoltSpringMode(SpringSettings::SpringMode springMode)
{
    switch (springMode) {
    case SpringSettings::SpringMode::FrequencyAndDamping:
        return JPH::ESpringMode::FrequencyAndDamping;
    case SpringSettings::SpringMode::StiffnessAndDamping:
        return JPH::ESpringMode::StiffnessAndDamping;
    default:
        Q_ASSERT(false);
    }
}

SpringSettings::SpringSettings(QObject *parent) : QObject(parent)
{
}

SpringSettings::SpringMode SpringSettings::mode() const
{
    return m_mode;
}

void SpringSettings::setMode(SpringMode mode)
{
    if (m_mode == mode)
        return;

    m_mode = mode;
    emit modeChanged(m_mode);
    emit changed();
}

float SpringSettings::frequency() const
{
    return m_frequency;
}

void SpringSettings::setFrequency(float frequency)
{
    if (qFuzzyCompare(m_frequency, frequency))
        return;

    m_frequency = frequency;
    emit frequencyChanged(m_frequency);
    emit changed();
}

float SpringSettings::stiffness() const
{
    return m_stiffness;
}

void SpringSettings::setStiffness(float stiffness)
{
    if (qFuzzyCompare(m_stiffness, stiffness))
        return;

    m_stiffness = stiffness;
    emit stiffnessChanged(m_stiffness);
    emit changed();
}

float SpringSettings::damping() const
{
    return m_damping;
}

void SpringSettings::setDamping(float damping)
{
    if (qFuzzyCompare(m_damping, damping))
        return;

    m_damping = damping;
    emit dampingChanged(m_damping);
    emit changed();
}

JPH::SpringSettings SpringSettings::getJoltSpringSettings() const
{
    JPH::SpringSettings springSettings;
    springSettings.mMode = convertToJoltSpringMode(m_mode);
    switch (m_mode) {
    case SpringSettings::SpringMode::FrequencyAndDamping:
        springSettings.mFrequency = m_frequency;
        break;
    case SpringSettings::SpringMode::StiffnessAndDamping:
        springSettings.mStiffness = m_stiffness;
        break;
    default:
        break;
    }
    springSettings.mDamping = m_damping;
    return springSettings;
}
