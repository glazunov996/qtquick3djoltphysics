#ifndef EXAMPLEGEOMETRY_H
#define EXAMPLEGEOMETRY_H

#include <QQuick3DGeometry>
#include <QByteArray>

class ExampleTriangleGeometry : public QQuick3DGeometry
{
    Q_OBJECT
    QML_NAMED_ELEMENT(ExampleTriangleGeometry)

public:
    ExampleTriangleGeometry();

private:
    void updateData();
};

class ExampleBoxGeometry : public QQuick3DGeometry
{
    Q_OBJECT
    QML_NAMED_ELEMENT(ExampleBoxGeometry)

public:
    ExampleBoxGeometry();

private:
    void updateData();
};

class ExampleMeshGridGeometry: public QQuick3DGeometry
{
    Q_OBJECT
    QML_NAMED_ELEMENT(ExampleMeshGridGeometry)

public:
    ExampleMeshGridGeometry();

private:
    void updateData();
};

class ExampleStairsGridGeometry : public QQuick3DGeometry
{
    Q_OBJECT
    QML_NAMED_ELEMENT(ExampleStairsGridGeometry)

public:
    ExampleStairsGridGeometry();

private:
    void updateData();
};

#endif // EXAMPLEGEOMETRY_H
