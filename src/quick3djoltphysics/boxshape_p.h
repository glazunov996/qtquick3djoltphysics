#ifndef BOXSHAPE_P_H
#define BOXSHAPE_P_H

#include "abstractshape_p.h"

#include <QVector3D>

class Q_QUICK3DJOLTPHYSICS_EXPORT BoxShape : public AbstractShape
{
    Q_OBJECT
    Q_PROPERTY(QVector3D extents READ extents WRITE setExtents NOTIFY extentsChanged)
    QML_NAMED_ELEMENT(BoxShape)
public:
    BoxShape(QQuick3DNode *parent = nullptr);
    ~BoxShape() override;

    QVector3D extents() const;
    void setExtents(const QVector3D &extents);

protected:
    void updateJoltShape() override;

signals:
    void extentsChanged(const QVector3D &extents);

private:
    QVector3D m_extents = QVector3D(100.0f, 100.0f, 100.0f);
};

#endif // BOXSHAPE_P_H