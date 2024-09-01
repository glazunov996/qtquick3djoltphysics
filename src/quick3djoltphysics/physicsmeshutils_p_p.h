#ifndef PHYSICSMESHUTILS_P_P_H
#define PHYSICSMESHUTILS_P_P_H

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQuick3DUtils/private/qssgmesh_p.h>

#include <QVector3D>
#include <QHash>

#include <Jolt/Jolt.h>
#include <Jolt/Geometry/Triangle.h>

#include <optional>

class QQuick3DGeometry;

namespace JPH {
class ConvexHullShapeSettings;
class MeshShapeSettings;
}

class Mesh
{
public:
    Mesh(const QString &qmlSource) : m_meshPath(qmlSource) { }
    Mesh(const QQuick3DGeometry *geometrySource) : m_meshGeometry(geometrySource) {}
    ~Mesh();

    void ref() { ++m_refCount; }
    int deref() { return --m_refCount; }

    JPH::ConvexHullShapeSettings *convexHullShapeSettings();
    JPH::MeshShapeSettings *meshShapeSettings();

private:
    void loadSsgMesh();
    JPH::ConvexHullShapeSettings *convexHullShapeSettingsSource();
    JPH::ConvexHullShapeSettings *convexHullShapeSettingsGeometrySource();
    JPH::MeshShapeSettings *meshShapeSettingsSource();
    JPH::MeshShapeSettings *meshShapeSettingsGeometrySource();

    QString m_meshPath;
    const QQuick3DGeometry *m_meshGeometry = nullptr;
    QSSGMesh::Mesh m_ssgMesh;
    int m_posOffset = 0;

    JPH::ConvexHullShapeSettings *m_convexHullShapeSettings = nullptr;
    JPH::MeshShapeSettings *m_meshShapeSettings = nullptr;
    int m_refCount = 0;
};

class MeshManager
{
public:
    static Mesh *getMesh(const QUrl &source, const QObject *contextObject);
    static Mesh *getMesh(QQuick3DGeometry *source);
    static void releaseMesh(Mesh *mesh);

private:
    static QHash<QString, Mesh *> g_sourceMeshHash;
    static QHash<QQuick3DGeometry *, Mesh *> m_geometryMeshHash;
};

#endif // PHYSICSMESHUTILS_P_P_H
