#ifndef PLANESHAPE_P_H
#define PLANESHAPE_P_H

#include "abstractshape_p.h"

class Q_QUICK3DJOLTPHYSICS_EXPORT PlaneShape : public AbstractShape
{
    Q_OBJECT
    Q_PROPERTY(float extent READ extent WRITE setExtent NOTIFY extentChanged)
    QML_NAMED_ELEMENT(PlaneShape)
public:
    PlaneShape(QQuick3DNode *parent = nullptr);
    ~PlaneShape() override;

    float extent() const;
    void setExtent(float extent);

signals:
    void extentChanged(float extent);

protected:
    void createJoltShape() override;

private:
    float m_extent = 2000.0f;
};

#endif // PLANESHAPE_P_H
