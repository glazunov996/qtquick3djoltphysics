#include "raycastresult_p.h"

RayCastResult::RayCastResult() = default;

RayCastResult::RayCastResult(Body *body, const QVector3D &position, const QVector3D &normal, float fraction)
    : m_body(body)
    , m_position(position)
    , m_normal(normal)
    , m_fraction(fraction)
{
}

Body *RayCastResult::body() const
{
    return m_body;
}

QVector3D RayCastResult::position() const
{
    return m_position;
}

QVector3D RayCastResult::normal() const
{
    return m_normal;
}

float RayCastResult::fraction() const
{
    return m_fraction;
}
