#ifndef SPHERESHAPE_P_H
#define SPHERESHAPE_P_H

#include "abstractshape_p.h"

class Q_QUICK3DJOLTPHYSICS_EXPORT SphereShape : public AbstractShape
{
    Q_OBJECT
    Q_PROPERTY(float diameter READ diameter WRITE setDiameter NOTIFY diameterChanged)
    QML_NAMED_ELEMENT(SphereShape)
public:
    SphereShape(QQuick3DNode *parent = nullptr);
    ~SphereShape() override;

    float diameter() const;
    void setDiameter(float diameter);

signals:
    void diameterChanged(float diameter);

protected:
    void updateJoltShape() override;

private:
    float m_diameter = 100.0f;
};

#endif // SPHERESHAPE_P_H
