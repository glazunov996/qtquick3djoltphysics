#include "examplegeometry.h"

#include <QVector3D>

ExampleTriangleGeometry::ExampleTriangleGeometry()
{
    updateData();
}

//! [update data]
void ExampleTriangleGeometry::updateData()
{
    clear();

    constexpr int kStride = sizeof(QVector3D);
    QByteArray vertexData(3 * kStride, Qt::Initialization::Uninitialized);
    QVector3D *p = reinterpret_cast<QVector3D *>(vertexData.data());

    *p = QVector3D(-5.0f, -5.0f, 0.0f); p++;
    *p = QVector3D(5.0f, -5.0f, 0.0f); p++;
    *p = QVector3D(0.0f, 5.0f, 0.0f); p++;

    setVertexData(vertexData);
    setStride(kStride);
    setBounds(QVector3D(-5.0f, -5.0f, 0.0f), QVector3D(+5.0f, +5.0f, 0.0f));
    setPrimitiveType(QQuick3DGeometry::PrimitiveType::Triangles);
    addAttribute(QQuick3DGeometry::Attribute::PositionSemantic,
                 0,
                 QQuick3DGeometry::Attribute::F32Type);
}

ExampleBoxGeometry::ExampleBoxGeometry()
{
    updateData();
}

//! [update data]
void ExampleBoxGeometry::updateData()
{
    clear();

    constexpr int kStride = sizeof(QVector3D);
    QByteArray vertexData(36 * kStride, Qt::Initialization::Uninitialized);
    QVector3D *p = reinterpret_cast<QVector3D *>(vertexData.data());

    *p = QVector3D(-1.0f,-5.0f,-1.0f); p++;
    *p = QVector3D(-1.0f,-5.0f, 1.0f); p++;
    *p = QVector3D(-1.0f, 5.0f, 1.0f); p++;
    *p = QVector3D(+1.0f, 5.0f,-1.0f); p++;
    *p = QVector3D(-1.0f,-5.0f,-1.0f); p++;
    *p = QVector3D(-1.0f, 5.0f,-1.0f); p++;
    *p = QVector3D(+1.0f,-5.0f, 1.0f); p++;
    *p = QVector3D(-1.0f,-5.0f,-1.0f); p++;
    *p = QVector3D(+1.0f,-5.0f,-1.0f); p++;
    *p = QVector3D(+1.0f, 5.0f,-1.0f); p++;
    *p = QVector3D(+1.0f,-5.0f,-1.0f); p++;
    *p = QVector3D(-1.0f,-5.0f,-1.0f); p++;
    *p = QVector3D(-1.0f,-5.0f,-1.0f); p++;
    *p = QVector3D(-1.0f, 5.0f, 1.0f); p++;
    *p = QVector3D(-1.0f, 5.0f,-1.0f); p++;
    *p = QVector3D(+1.0f,-5.0f, 1.0f); p++;
    *p = QVector3D(-1.0f,-5.0f, 1.0f); p++;
    *p = QVector3D(-1.0f,-5.0f,-1.0f); p++;
    *p = QVector3D(-1.0f, 5.0f, 1.0f); p++;
    *p = QVector3D(-1.0f,-5.0f, 1.0f); p++;
    *p = QVector3D(+1.0f,-5.0f, 1.0f); p++;
    *p = QVector3D(+1.0f, 5.0f, 1.0f); p++;
    *p = QVector3D(+1.0f,-5.0f,-1.0f); p++;
    *p = QVector3D(+1.0f, 5.0f,-1.0f); p++;
    *p = QVector3D(+1.0f,-5.0f,-1.0f); p++;
    *p = QVector3D(+1.0f, 5.0f, 1.0f); p++;
    *p = QVector3D(+1.0f,-5.0f, 1.0f); p++;
    *p = QVector3D(+1.0f, 5.0f, 1.0f); p++;
    *p = QVector3D(+1.0f, 5.0f,-1.0f); p++;
    *p = QVector3D(-1.0f, 5.0f,-1.0f); p++;
    *p = QVector3D(+1.0f, 5.0f, 1.0f); p++;
    *p = QVector3D(-1.0f, 5.0f,-1.0f); p++;
    *p = QVector3D(-1.0f, 5.0f, 1.0f); p++;
    *p = QVector3D(+1.0f, 5.0f, 1.0f); p++;
    *p = QVector3D(-1.0f, 5.0f, 1.0f); p++;
    *p = QVector3D(+1.0f,-5.0f, 1.0f); p++;

    setVertexData(vertexData);
    setStride(kStride);
    setBounds(QVector3D(-5.0f, -5.0f, -5.0f), QVector3D(+5.0f, +5.0f, +5.0f));
    setPrimitiveType(QQuick3DGeometry::PrimitiveType::Triangles);
    addAttribute(QQuick3DGeometry::Attribute::PositionSemantic, 0,
                 QQuick3DGeometry::Attribute::F32Type);
}

ExampleMeshGridGeometry::ExampleMeshGridGeometry()
{
    updateData();
}

void ExampleMeshGridGeometry::updateData()
{
    clear();

    constexpr int kStride = sizeof(QVector3D);
    QByteArray vertexData(2400 * kStride, Qt::Initialization::Uninitialized);
    QVector3D *p = reinterpret_cast<QVector3D *>(vertexData.data());

    for (int x = -10; x < 10; ++x) {
        for (int z = -10; z < 10; ++z) {
            float x1 = 10.0f * x;
            float z1 = 10.0f * z;
            float x2 = x1 + 10.0f;
            float z2 = z1 + 10.0f;

            auto v1 = QVector3D(x1, 0, z1);
            auto v2 = QVector3D(x2, 0, z1);
            auto v3 = QVector3D(x1, 0, z2);
            auto v4 = QVector3D(x2, 0, z2);

            *p = v1; ++p;
            *p = v3; ++p;
            *p = v4; ++p;
            *p = v1; ++p;
            *p = v4; ++p;
            *p = v2; ++p;
        }
    }

    setVertexData(vertexData);
    setStride(kStride);
    setBounds(QVector3D(-100.0f, 0.0f, -100.0f), QVector3D(+100.0f, +0.0f, +100.0f));
    setPrimitiveType(QQuick3DGeometry::PrimitiveType::Triangles);
    addAttribute(QQuick3DGeometry::Attribute::PositionSemantic, 0,
                 QQuick3DGeometry::Attribute::F32Type);
}

ExampleStairsGridGeometry::ExampleStairsGridGeometry()
{
    updateData();
}

void ExampleStairsGridGeometry::updateData()
{
    clear();

    constexpr int kStride = sizeof(QVector3D);
    QByteArray vertexData(3 * 6 * 10 * kStride, Qt::Initialization::Uninitialized);
    QVector3D *p = reinterpret_cast<QVector3D *>(vertexData.data());

    constexpr float stairsStepHeight = 0.3f;
    constexpr float rearZ = 10 * stairsStepHeight;
    for (int i = 0; i < 10; ++i) {
        // Start of step
        auto base = QVector3D(0, stairsStepHeight * i, stairsStepHeight * i);
        // Left side
        auto b1 = base + QVector3D(2.0f, 0, 0);
        auto s1 = b1 + QVector3D(0, stairsStepHeight, 0);
        auto p1 = s1 + QVector3D(0, 0 , stairsStepHeight);

        // Right side
        auto width = QVector3D(-4.0f, 0, 0);
        auto b2 = b1 + width;
        auto s2 = s1 + width;
        auto p2 = p1 + width;

        *p = s1; p++;
        *p = b1; p++;
        *p = s2; p++;

        *p = b1; p++;
        *p = b2; p++;
        *p = s2; p++;

        *p = s1; p++;
        *p = p2; p++;
        *p = p1; p++;

        *p = s1; p++;
        *p = s2; p++;
        *p = p2; p++;

        // Side of stairs
        auto rb2 = b2;
        rb2.setZ(rearZ);
        auto rs2 = s2;
        rs2.setZ(rearZ);

        *p = s2; p++;
        *p = b2; p++;
        *p = rs2; p++;

        *p = rs2; p++;
        *p = b2; p++;
        *p = rb2; p++;

        p1 = p2;
    }

    setVertexData(vertexData);
    setStride(kStride);
    setPrimitiveType(QQuick3DGeometry::PrimitiveType::Triangles);
    addAttribute(QQuick3DGeometry::Attribute::PositionSemantic,
                 0,
                 QQuick3DGeometry::Attribute::F32Type);
}
