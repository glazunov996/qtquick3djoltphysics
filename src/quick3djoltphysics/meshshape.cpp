#include "meshshape_p.h"
#include "physicsmeshutils_p_p.h"
#include "physicsutils_p.h"
#include "physicssystem_p.h"

#include <QFile>
#include <QFileInfo>
#include <QtQuick3D/QQuick3DGeometry>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>
#include <Jolt/Physics/Collision/Shape/ConvexHullShape.h>

#include <QDebug>

static QQuick3DGeometry::Attribute
attributeBySemantic(const QQuick3DGeometry *geometry,
                    QQuick3DGeometry::Attribute::Semantic semantic)
{
    for (int i = 0; i < geometry->attributeCount(); i++) {
        const auto attr = geometry->attribute(i);
        if (attr.semantic == semantic)
            return attr;
    }

    Q_UNREACHABLE();
    return QQuick3DGeometry::Attribute();
}

Mesh::~Mesh()
{
    delete m_convexHullShapeSettings;
    m_convexHullShapeSettings = nullptr;
    delete m_meshShapeSettings;
    m_meshShapeSettings = nullptr;
}

JPH::ConvexHullShapeSettings *Mesh::convexHullShapeSettings()
{
    if (m_convexHullShapeSettings)
        return m_convexHullShapeSettings;

    if (m_meshGeometry)
        return convexHullShapeSettingsGeometrySource();
    if (!m_meshPath.isEmpty())
        return convexHullShapeSettingsSource();
    return nullptr;
}

JPH::MeshShapeSettings *Mesh::meshShapeSettings()
{
    if (m_meshShapeSettings)
        return m_meshShapeSettings;

    if (m_meshGeometry)
        return meshShapeSettingsGeometrySource();
    if (!m_meshPath.isEmpty())
        return meshShapeSettingsSource();
    return nullptr;
}

void Mesh::loadSsgMesh()
{
    if (m_ssgMesh.isValid())
        return;

    static const char *compTypes[] = { "Null",  "UnsignedInt8",  "Int8",    "UnsignedInt16",
                                       "Int16", "UnsignedInt32", "Int32",   "UnsignedInt64",
                                       "Int64", "Float16",       "Float32", "Float64" };

    QFileInfo fileInfo = QFileInfo(m_meshPath);
    if (fileInfo.exists()) {
        QFile file(fileInfo.absoluteFilePath());
        if (file.open(QFile::ReadOnly))
            m_ssgMesh = QSSGMesh::Mesh::loadMesh(&file);
    }

    qCDebug(lcQuick3dJoltPhysics) << "Loaded SSG mesh from" << m_meshPath << m_ssgMesh.isValid()
                                  << "draw" << int(m_ssgMesh.drawMode()) << "wind"
                                  << int(m_ssgMesh.winding()) << "subs" << m_ssgMesh.subsets().count()
                                  << "attrs" << m_ssgMesh.vertexBuffer().entries.count()
                                  << m_ssgMesh.vertexBuffer().data.size() << "stride"
                                  << m_ssgMesh.vertexBuffer().stride << "verts"
                                  << m_ssgMesh.vertexBuffer().data.size()
                        / m_ssgMesh.vertexBuffer().stride;

    for (auto &v : m_ssgMesh.vertexBuffer().entries) {
        qCDebug(lcQuick3dJoltPhysics) << "  attr" << v.name << compTypes[int(v.componentType)] << "cc"
                                      << v.componentCount << "offs" << v.offset;
        Q_ASSERT(v.componentType == QSSGMesh::Mesh::ComponentType::Float32);
        if (v.name == "attr_pos")
            m_posOffset = v.offset;
    }

    if (m_ssgMesh.isValid()) {
        auto sub = m_ssgMesh.subsets().constFirst();
        qCDebug(lcQuick3dJoltPhysics) << "..." << sub.name << "count" << sub.count << "bounds"
                                      << sub.bounds.min << sub.bounds.max << "offset" << sub.offset;
    }

    if (!m_ssgMesh.isValid())
        qCWarning(lcQuick3dJoltPhysics) << "Could not read mesh from" << m_meshPath;
}

JPH::ConvexHullShapeSettings *Mesh::convexHullShapeSettingsSource()
{
    delete m_convexHullShapeSettings;
    m_convexHullShapeSettings = nullptr;

    loadSsgMesh();

    if (!m_ssgMesh.isValid())
        return nullptr;

    const auto stride = m_ssgMesh.vertexBuffer().stride;
    const auto numVertices = m_ssgMesh.vertexBuffer().data.size() / stride;
    const auto vStride = stride / sizeof(float);
    const auto vertexData = reinterpret_cast<const float *>(m_ssgMesh.vertexBuffer().data.constData()) + m_posOffset;

    if (stride == 0) {
        qWarning() << "Mesh: Invalid geometry, stride is zero.";
        return nullptr;
    }

    // NOTE: Since we are making a mesh for the convex hull and are only
    // interested in the positions we can Skip the index array.

    JPH::Array<JPH::Vec3> points;

    for (int i = 0; i < numVertices / 3; ++i) {
        auto i0 = i * 3;
        auto i1 = i * 3 + 1;
        auto i2 = i * 3 + 2;

        if (i2 >= numVertices) {
            qWarning() << "Mesh: Invalid geometry, not enough vertices to form a triangle.";
            return nullptr;
        }

        auto p0 = JPH::Vec3(vertexData[i0 * vStride], vertexData[i0 * vStride + 1], vertexData[i0 * vStride + 2]);
        auto p1 = JPH::Vec3(vertexData[i1 * vStride], vertexData[i1 * vStride + 1], vertexData[i1 * vStride + 2]);
        auto p2 = JPH::Vec3(vertexData[i2 * vStride], vertexData[i2 * vStride + 1], vertexData[i2 * vStride + 2]);

        points.push_back(p0);
        points.push_back(p1);
        points.push_back(p2);
    }

    m_convexHullShapeSettings = new JPH::ConvexHullShapeSettings(points);
    return m_convexHullShapeSettings;
}

JPH::ConvexHullShapeSettings *Mesh::convexHullShapeSettingsGeometrySource()
{
    delete m_convexHullShapeSettings;
    m_convexHullShapeSettings = nullptr;

    auto vertexBuffer = m_meshGeometry->vertexData();

    if (m_meshGeometry->primitiveType() != QQuick3DGeometry::PrimitiveType::Triangles) {
        qWarning() << "Mesh: Invalid geometry primitive type, must be Triangles. ";
        return nullptr;
    }

    if (!vertexBuffer.size()) {
        qWarning() << "Mesh: Invalid geometry, vertexData is empty. ";
        return nullptr;
    }

    const auto vertexAttribute =
        attributeBySemantic(m_meshGeometry, QQuick3DGeometry::Attribute::PositionSemantic);
    Q_ASSERT(vertexAttribute.componentType == QQuick3DGeometry::Attribute::F32Type);

    const auto stride = m_meshGeometry->stride();
    const auto numVertices = vertexBuffer.size() / stride;
    const auto vStride = stride / sizeof(float);
    const auto vertexData = reinterpret_cast<const float *>(vertexBuffer.constData()) + vertexAttribute.offset;

    if (stride == 0) {
        qWarning() << "Mesh: Invalid geometry, stride is zero.";
        return nullptr;
    }

    // NOTE: Since we are making a mesh for the convex hull and are only
    // interested in the positions we can Skip the index array.

    JPH::Array<JPH::Vec3> points;

    for (int i = 0; i < numVertices / 3; ++i) {
        auto i0 = i * 3;
        auto i1 = i * 3 + 1;
        auto i2 = i * 3 + 2;

        if (i2 >= numVertices) {
            qWarning() << "Mesh: Invalid geometry, not enough vertices to form a triangle.";
            return nullptr;
        }

        auto p0 = JPH::Vec3(vertexData[i0 * vStride], vertexData[i0 * vStride + 1], vertexData[i0 * vStride + 2]);
        auto p1 = JPH::Vec3(vertexData[i1 * vStride], vertexData[i1 * vStride + 1], vertexData[i1 * vStride + 2]);
        auto p2 = JPH::Vec3(vertexData[i2 * vStride], vertexData[i2 * vStride + 1], vertexData[i2 * vStride + 2]);

        points.push_back(p0);
        points.push_back(p1);
        points.push_back(p2);
    }

    m_convexHullShapeSettings = new JPH::ConvexHullShapeSettings(points);
    return m_convexHullShapeSettings;
}

JPH::MeshShapeSettings *Mesh::meshShapeSettingsSource()
{
    delete m_meshShapeSettings;
    m_meshShapeSettings = nullptr;

    loadSsgMesh();

    if (!m_ssgMesh.isValid())
        return nullptr;

    const auto stride = m_ssgMesh.vertexBuffer().stride;
    const auto numVertices = m_ssgMesh.vertexBuffer().data.size() / stride;
    const auto vStride = stride / sizeof(float);
    const auto vertexData = reinterpret_cast<const float *>(m_ssgMesh.vertexBuffer().data.constData()) + m_posOffset;
    const auto indexBuffer = m_ssgMesh.indexBuffer().data;

    if (stride == 0) {
        qWarning() << "Mesh: Invalid geometry, stride is zero.";
        return nullptr;
    }

    if (indexBuffer.size()) {
        const bool u16IndexType =
            m_ssgMesh.indexBuffer().componentType == QSSGMesh::Mesh::ComponentType::UnsignedInt16;

        Q_ASSERT(m_ssgMesh.indexBuffer().componentType
                        == QSSGMesh::Mesh::ComponentType::UnsignedInt16
                 || m_ssgMesh.indexBuffer().componentType
                        == QSSGMesh::Mesh::ComponentType::UnsignedInt32);

        JPH::VertexList vertices;
        JPH::IndexedTriangleList indexes;

        for (int i = 0; i < numVertices; ++i) {
            auto v = JPH::Float3(vertexData[i * vStride], vertexData[i * vStride + 1], vertexData[i * vStride + 2]);
            vertices.push_back(v);
        }

        if (u16IndexType) {
            const auto indexData16 = reinterpret_cast<const unsigned short *>(indexBuffer.constData());
            for (int i = 0; i < indexBuffer.size() / sizeof(unsigned short); i += 3) {
                auto i0 = indexData16[i];
                auto i1 = indexData16[i + 1];
                auto i2 = indexData16[i + 2];

                auto index = JPH::IndexedTriangle(i0, i1, i2, 0);
                indexes.push_back(index);
            }
        } else {
            const auto indexData32 = reinterpret_cast<const unsigned int *>(indexBuffer.constData());
            for (int i = 0; i < indexBuffer.size() / sizeof(unsigned int); i += 3) {
                auto i0 = indexData32[i];
                auto i1 = indexData32[i + 1];
                auto i2 = indexData32[i + 2];

                auto index = JPH::IndexedTriangle(i0, i1, i2, 0);
                indexes.push_back(index);
            }
        }

        m_meshShapeSettings = new JPH::MeshShapeSettings(std::move(vertices), std::move(indexes));
    } else {
        JPH::TriangleList triangles;
        for (int i = 0; i < numVertices / 3; ++i) {
            auto i0 = i * 3;
            auto i1 = i * 3 + 1;
            auto i2 = i * 3 + 2;

            if (i2 >= numVertices) {
                qWarning() << "Mesh: Invalid geometry, not enough vertices to form a triangle.";
                return nullptr;
            }

            auto triangle = JPH::Triangle(
                JPH::Vec3(vertexData[i0 * vStride], vertexData[i0 * vStride + 1], vertexData[i0 * vStride + 2]),
                JPH::Vec3(vertexData[i1 * vStride], vertexData[i1 * vStride + 1], vertexData[i1 * vStride + 2]),
                JPH::Vec3(vertexData[i2 * vStride], vertexData[i2 * vStride + 1], vertexData[i2 * vStride + 2]));

            triangles.push_back(triangle);
        }

        m_meshShapeSettings = new JPH::MeshShapeSettings(triangles);
    }

    return m_meshShapeSettings;
}

JPH::MeshShapeSettings *Mesh::meshShapeSettingsGeometrySource()
{
    delete m_meshShapeSettings;
    m_meshShapeSettings = nullptr;

    auto vertexBuffer = m_meshGeometry->vertexData();

    if (m_meshGeometry->primitiveType() != QQuick3DGeometry::PrimitiveType::Triangles) {
        qWarning() << "Mesh: Invalid geometry primitive type, must be Triangles. ";
        return nullptr;
    }

    if (!vertexBuffer.size()) {
        qWarning() << "Mesh: Invalid geometry, vertexData is empty. ";
        return nullptr;
    }

    const auto vertexAttribute =
        attributeBySemantic(m_meshGeometry, QQuick3DGeometry::Attribute::PositionSemantic);
    Q_ASSERT(vertexAttribute.componentType == QQuick3DGeometry::Attribute::F32Type);

    const auto stride = m_meshGeometry->stride();
    const auto numVertices = vertexBuffer.size() / stride;
    const auto vStride = stride / sizeof(float);
    const auto vertexData = reinterpret_cast<const float *>(vertexBuffer.constData()) + vertexAttribute.offset;
    const auto indexBuffer = m_meshGeometry->indexData();

    if (stride == 0) {
        qWarning() << "Mesh: Invalid geometry, stride is zero.";
        return nullptr;
    }

    if (indexBuffer.size()) {
        const auto indexAttribute =
            attributeBySemantic(m_meshGeometry, QQuick3DGeometry::Attribute::IndexSemantic);
        const bool u16IndexType =
            indexAttribute.componentType == QQuick3DGeometry::Attribute::U16Type;

        Q_ASSERT(indexAttribute.componentType == QQuick3DGeometry::Attribute::U16Type
                 || indexAttribute.componentType == QQuick3DGeometry::Attribute::U32Type);

        JPH::VertexList vertices;
        JPH::IndexedTriangleList indexes;

        for (int i = 0; i < numVertices; ++i) {
            auto v = JPH::Float3(vertexData[i * vStride], vertexData[i * vStride + 1], vertexData[i * vStride + 2]);
            vertices.push_back(v);
        }

        if (u16IndexType) {
            const auto indexData16 = reinterpret_cast<const unsigned short *>(indexBuffer.constData());
            for (int i = 0; i < indexBuffer.size() / sizeof(unsigned short); i += 3) {
                auto i0 = indexData16[i];
                auto i1 = indexData16[i + 1];
                auto i2 = indexData16[i + 2];

                auto index = JPH::IndexedTriangle(i0, i1, i2, 0);
                indexes.push_back(index);
            }
        } else {
            const auto indexData32 = reinterpret_cast<const unsigned int *>(indexBuffer.constData());
            for (int i = 0; i < indexBuffer.size() / sizeof(unsigned int); i += 3) {
                auto i0 = indexData32[i];
                auto i1 = indexData32[i + 1];
                auto i2 = indexData32[i + 2];

                auto index = JPH::IndexedTriangle(i0, i1, i2, 0);
                indexes.push_back(index);
            }
        }

        m_meshShapeSettings = new JPH::MeshShapeSettings(std::move(vertices), std::move(indexes));
    } else {
        JPH::TriangleList triangles;

        for (int i = 0; i < numVertices / 3; ++i) {
            auto i0 = i * 3;
            auto i1 = i * 3 + 1;
            auto i2 = i * 3 + 2;

            if (i2 >= numVertices) {
                qWarning() << "Mesh: Invalid geometry, not enough vertices to form a triangle.";
                return nullptr;
            }

            auto triangle = JPH::Triangle(
                JPH::Vec3(vertexData[i0 * vStride], vertexData[i0 * vStride + 1], vertexData[i0 * vStride + 2]),
                JPH::Vec3(vertexData[i1 * vStride], vertexData[i1 * vStride + 1], vertexData[i1 * vStride + 2]),
                JPH::Vec3(vertexData[i2 * vStride], vertexData[i2 * vStride + 1], vertexData[i2 * vStride + 2]));

            triangles.push_back(triangle);
        }

        m_meshShapeSettings = new JPH::MeshShapeSettings(triangles);
    }

    return m_meshShapeSettings;
}

QHash<QString, Mesh *> MeshManager::g_sourceMeshHash;
QHash<QQuick3DGeometry *, Mesh *> MeshManager::m_geometryMeshHash;

Mesh *MeshManager::getMesh(const QUrl &source, const QObject *contextObject)
{
    const QQmlContext *context = qmlContext(contextObject);
    const auto resolvedUrl = context ? context->resolvedUrl(source) : source;
    const auto qmlSource = QQmlFile::urlToLocalFileOrQrc(resolvedUrl);
    auto *mesh = g_sourceMeshHash.value(qmlSource);
    if (!mesh) {
        mesh = new Mesh(qmlSource);
        g_sourceMeshHash[qmlSource] = mesh;
    }
    mesh->ref();
    return mesh;
}

Mesh *MeshManager::getMesh(QQuick3DGeometry *source)
{
    auto *mesh = m_geometryMeshHash.value(source);
    if (!mesh) {
        mesh = new Mesh(source);
        m_geometryMeshHash.insert(source, mesh);
    }
    mesh->ref();
    return mesh;
}

void MeshManager::releaseMesh(Mesh *mesh)
{
    if (mesh == nullptr || mesh->deref() > 0)
        return;

    qCDebug(lcQuick3dJoltPhysics()) << "deleting mesh" << mesh;
    erase_if(m_geometryMeshHash, [mesh](std::pair<QQuick3DGeometry *, Mesh *&> h) {
        return h.second == mesh;
    });
    delete mesh;
}

MeshShape::MeshShape(QQuick3DNode *parent) : AbstractShape(parent)
{
}

MeshShape::~MeshShape()
{
    if (m_mesh)
        MeshManager::releaseMesh(m_mesh);
}

const QUrl &MeshShape::source() const
{
    return m_meshSource;
}

void MeshShape::setSource(const QUrl &newSource)
{
    if (m_meshSource == newSource)
        return;
    m_meshSource = newSource;

    // If we get a new source and our mesh was from the old source
    // (meaning it was NOT from a geometry) we deref
    if (m_geometry == nullptr) {
        MeshManager::releaseMesh(m_mesh);
        m_mesh = nullptr;
    }

    if (m_geometry == nullptr && !newSource.isEmpty())
        m_mesh = MeshManager::getMesh(m_meshSource, this);

    updateJoltShape();

    emit sourceChanged();
    emit changed();
}

QQuick3DGeometry *MeshShape::geometry() const
{
    return m_geometry;
}

void MeshShape::setGeometry(QQuick3DGeometry *newGeometry)
{
    if (m_geometry == newGeometry)
        return;
    if (m_geometry)
        m_geometry->disconnect(m_geometrySignalConnection);

    m_geometry = newGeometry;

    connect(m_geometry, &QObject::destroyed, this,
            [this]
    {
        m_geometry = nullptr;
    });
    m_geometrySignalConnection = connect(m_geometry, &QQuick3DGeometry::geometryChanged, this,
            [this]
    {
        updateJoltShape();
    });

    // New geometry means we get a new mesh so deref the old one
    MeshManager::releaseMesh(m_mesh);
    m_mesh = MeshManager::getMesh(m_geometry);

    updateJoltShape();

    emit geometryChanged();
    emit changed();
}

float MeshShape::activeEdgeCosThresholdAngle() const
{
    return m_activeEdgeCosThresholdAngle;
}

void MeshShape::setActiveEdgeCosThresholdAngle(float activeEdgeCosThresholdAngle)
{
    if (qFuzzyCompare(activeEdgeCosThresholdAngle, m_activeEdgeCosThresholdAngle))
        return;

    m_activeEdgeCosThresholdAngle = activeEdgeCosThresholdAngle;

    updateJoltShape();

    emit activeEdgeCosThresholdAngleChanged();
    emit changed();
}

void MeshShape::createJoltShape()
{
    if (m_mesh == nullptr)
        return;

    auto meshShapeSettings = m_mesh->meshShapeSettings();
    if (!meshShapeSettings)
        return;

    meshShapeSettings->mActiveEdgeCosThresholdAngle = m_activeEdgeCosThresholdAngle;

    auto shapeResult = meshShapeSettings->Create();
    m_shape = shapeResult.Get();
}
