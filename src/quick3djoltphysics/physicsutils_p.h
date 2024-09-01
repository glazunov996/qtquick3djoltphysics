#ifndef PHYSICSUTILS_P_H
#define PHYSICSUTILS_P_H

#include <QVector3D>
#include <QVector4D>
#include <QQuaternion>
#include <QMatrix4x4>

#include <Jolt/Jolt.h>
#include <Jolt/Math/Vec3.h>
#include <Jolt/Math/Quat.h>
#include <Jolt/Math/Mat44.h>
#include <Jolt/Geometry/Plane.h>

namespace PhysicsUtils {

Q_ALWAYS_INLINE JPH::Vec3 toJoltType(const QVector3D &qvec)
{
    return JPH::Vec3(qvec.x(), qvec.y(), qvec.z());
}

Q_ALWAYS_INLINE JPH::Vec4 toJoltType(const QVector4D &qvec)
{
    return JPH::Vec4(qvec.x(), qvec.y(), qvec.z(), qvec.w());
}

Q_ALWAYS_INLINE JPH::Quat toJoltType(const QQuaternion &qquat)
{
    return JPH::Quat(qquat.x(), qquat.y(), qquat.z(), qquat.scalar());
}

Q_ALWAYS_INLINE JPH::Mat44 toJoltType(const QMatrix4x4 &mat)
{
    return JPH::Mat44(toJoltType(mat.column(0)),
                      toJoltType(mat.column(1)),
                      toJoltType(mat.column(2)),
                      toJoltType(mat.column(3)));
}

Q_ALWAYS_INLINE QVector3D toQtType(const JPH::Vec3 &vec)
{
    return QVector3D(vec.GetX(), vec.GetY(), vec.GetZ());
}

Q_ALWAYS_INLINE QVector4D toQtType(const JPH::Vec4 &vec)
{
    return QVector4D(vec.GetX(), vec.GetY(), vec.GetZ(), vec.GetW());
}

Q_ALWAYS_INLINE QQuaternion toQtType(const JPH::Quat &quat)
{
    return QQuaternion(quat.GetW(), quat.GetX(), quat.GetY(), quat.GetZ());
}

Q_ALWAYS_INLINE QMatrix4x4 toQtType(const JPH::Mat44 &mat)
{
    QMatrix4x4 m;
    m.setColumn(0, PhysicsUtils::toQtType(mat.GetColumn4(0)));
    m.setColumn(1, PhysicsUtils::toQtType(mat.GetColumn4(1)));
    m.setColumn(2, PhysicsUtils::toQtType(mat.GetColumn4(2)));
    m.setColumn(3, PhysicsUtils::toQtType(mat.GetColumn4(3)));
    return m;
}
}

#endif // PHYSICSUTILS_P_H
