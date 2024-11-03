#include "collideshaperesult_p.h"

CollideShapeResult::CollideShapeResult() = default;

CollideShapeResult::CollideShapeResult(const QVector3D &contactPointOn1,
                                       const QVector3D &contactPointOn2,
                                       const QVector3D &penetrationAxis,
                                       float penetrationDepth,
                                       Body *body2)
    : m_contactPointOn1(contactPointOn1)
    , m_contactPointOn2(contactPointOn2)
    , m_penetrationAxis(penetrationAxis)
    , m_penetrationDepth(penetrationDepth)
    , m_body2(body2)
{
}

QVector3D CollideShapeResult::contactPointOn1() const
{
    return m_contactPointOn1;
}

QVector3D CollideShapeResult::contactPointOn2() const
{
    return m_contactPointOn2;
}

QVector3D CollideShapeResult::penetrationAxis() const
{
    return m_penetrationAxis;
}

float CollideShapeResult::penetrationDepth() const
{
    return m_penetrationDepth;
}

Body *CollideShapeResult::body2() const
{
    return m_body2;
}
