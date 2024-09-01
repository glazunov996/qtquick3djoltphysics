#ifndef MESHSHAPE_P_H
#define MESHSHAPE_P_H

#include "abstractshape_p.h"

#include <QtQuick3D/QQuick3DGeometry>

class Mesh;

class Q_QUICK3DJOLTPHYSICS_EXPORT MeshShape : public AbstractShape
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged REVISION(6, 5))
    Q_PROPERTY(QQuick3DGeometry *geometry READ geometry WRITE setGeometry NOTIFY geometryChanged)
    QML_NAMED_ELEMENT(MeshShape)
public:
    MeshShape(QQuick3DNode *parent = nullptr);
    ~MeshShape() override;

    const QUrl &source() const;
    void setSource(const QUrl &newSource);
    QQuick3DGeometry *geometry() const;
    void setGeometry(QQuick3DGeometry *newGeometry);

signals:
    void sourceChanged();
    void geometryChanged();

protected:
    void updateJoltShape() override;

    Mesh *m_mesh = nullptr;

private:
    QUrl m_meshSource;
    QQuick3DGeometry *m_geometry = nullptr;
    QMetaObject::Connection m_geometrySignalConnection;
};

#endif // MESHSHAPE_P_H
