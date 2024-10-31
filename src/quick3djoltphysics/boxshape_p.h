#ifndef BOXSHAPE_P_H
#define BOXSHAPE_P_H

#include "abstractshape_p.h"

#include <QVector3D>

class Q_QUICK3DJOLTPHYSICS_EXPORT BoxShape : public AbstractShape
{
    Q_OBJECT
    Q_PROPERTY(QVector3D extents READ extents WRITE setExtents NOTIFY extentsChanged)
    Q_PROPERTY(float convexRadius READ convexRadius WRITE setConvexRadius NOTIFY convexRadiusChanged)
    QML_NAMED_ELEMENT(BoxShape)
public:
    BoxShape(QQuick3DNode *parent = nullptr);
    ~BoxShape() override;

    QVector3D extents() const;
    void setExtents(const QVector3D &extents);

    float convexRadius() const;
    void setConvexRadius(float convexRadius);

protected:
    void createJoltShape() override;

signals:
    void extentsChanged(const QVector3D &extents);
    void convexRadiusChanged(float convexRadius);

private:
    QVector3D m_extents = QVector3D(1.0f, 1.0f, 1.0f);
    float m_convexRadius = 0.05f;
};

#endif // BOXSHAPE_P_H
