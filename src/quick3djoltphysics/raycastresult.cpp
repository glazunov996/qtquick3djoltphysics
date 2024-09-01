#include "raycastresult_p.h"

RayCastResult::RayCastResult() = default;

RayCastResult::RayCastResult(Body *body, const QVector3D &position, float fraction)
    : m_body(body)
    , m_fraction(fraction)
    , m_position(position)
{
}

Body *RayCastResult::body() const
{
    return m_body;
}

float RayCastResult::fraction() const
{
    return m_fraction;
}

QVector3D RayCastResult::position() const
{
    return m_position;
}
