#ifndef CYLINDERSHAPE_P_H
#define CYLINDERSHAPE_P_H

#include "abstractshape_p.h"

class Q_QUICK3DJOLTPHYSICS_EXPORT CylinderShape : public AbstractShape
{
    Q_OBJECT
    Q_PROPERTY(float diameter READ diameter WRITE setDiameter NOTIFY diameterChanged)
    Q_PROPERTY(float height READ height WRITE setHeight NOTIFY heightChanged)
    QML_NAMED_ELEMENT(CylinderShape)
public:
    CylinderShape(QQuick3DNode *parent = nullptr);
    ~CylinderShape() override;

    float diameter() const;
    void setDiameter(float diameter);

    float height() const;
    void setHeight(float height);

signals:
    void diameterChanged(float diameter);
    void heightChanged(float height);

protected:
    void createJoltShape() override;

private:
    float m_diameter = 1.0f;
    float m_height = 1.0f;
};

#endif // CYLINDERSHAPE_P_H
