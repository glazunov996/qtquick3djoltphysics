#ifndef SPRINGSETTINGS_P_H
#define SPRINGSETTINGS_P_H

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQml/QQmlEngine>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Constraints/SpringSettings.h>

class Q_QUICK3DJOLTPHYSICS_EXPORT SpringSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SpringMode mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(float frequency READ frequency WRITE setFrequency NOTIFY frequencyChanged)
    Q_PROPERTY(float stiffness READ stiffness WRITE setStiffness NOTIFY stiffnessChanged)
    Q_PROPERTY(float damping READ damping WRITE setDamping NOTIFY dampingChanged)
    QML_NAMED_ELEMENT(SpringSettings)
public:
    explicit SpringSettings(QObject *parent = nullptr);

    enum class SpringMode {
        FrequencyAndDamping,
        StiffnessAndDamping,
    };
    Q_ENUM(SpringMode)

    SpringMode mode() const;
    void setMode(SpringMode mode);

    float frequency() const;
    void setFrequency(float frequency);

    float stiffness() const;
    void setStiffness(float stiffness);

    float damping() const;
    void setDamping(float damping);

signals:
    void modeChanged(SpringMode mode);
    void frequencyChanged(float frequency);
    void stiffnessChanged(float stiffness);
    void dampingChanged(float damping);
    void changed();

private:
    JPH::SpringSettings getJoltSpringSettings() const;
    friend class DistanceConstraint;
    friend class HingeConstraint;

    SpringMode m_mode = SpringMode::FrequencyAndDamping;
    float m_frequency = 0.0f;
    float m_stiffness = 0.0f;
    float m_damping = 0.0f;
};

#endif // SPRINGSETTINGS_P_H
