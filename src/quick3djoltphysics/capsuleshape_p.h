#ifndef CAPSULESHAPE_P_H
#define CAPSULESHAPE_P_H

#include "abstractshape_p.h"

class Q_QUICK3DJOLTPHYSICS_EXPORT CapsuleShape : public AbstractShape
{
    Q_OBJECT
    Q_PROPERTY(float diameter READ diameter WRITE setDiameter NOTIFY diameterChanged)
    Q_PROPERTY(float height READ height WRITE setHeight NOTIFY heightChanged)
    QML_NAMED_ELEMENT(CapsuleShape)
public:
    CapsuleShape(QQuick3DNode *parent = nullptr);
    ~CapsuleShape() override;

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

#endif // CAPSULESHAPE_P_H
