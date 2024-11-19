#ifndef ROTATEDTRANSLATEDSHAPE_P_H
#define ROTATEDTRANSLATEDSHAPE_P_H

#include "abstractshape_p.h"

class Q_QUICK3DJOLTPHYSICS_EXPORT RotatedTranslatedShape : public AbstractShape
{
    Q_OBJECT
    Q_PROPERTY(AbstractShape *shape READ shape WRITE setShape NOTIFY shapeChanged)
    QML_NAMED_ELEMENT(RotatedTranslatedShape)
public:
    RotatedTranslatedShape(QQuick3DNode *parent = nullptr);
    ~RotatedTranslatedShape() override;

    AbstractShape *shape() const;
    void setShape(AbstractShape *shape);

protected:
    void createJoltShape() override;

signals:
    void shapeChanged(AbstractShape *shape);

private:
    AbstractShape *m_shape = nullptr;
    QMetaObject::Connection m_shapeSignalConnection;
};

#endif // ROTATEDTRANSLATEDSHAPE_P_H
