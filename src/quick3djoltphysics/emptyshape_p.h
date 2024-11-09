#ifndef EMPTYSHAPE_P_H
#define EMPTYSHAPE_P_H

#include "abstractshape_p.h"

class Q_QUICK3DJOLTPHYSICS_EXPORT EmptyShape : public AbstractShape
{
    Q_OBJECT
    QML_NAMED_ELEMENT(EmptyShape)
public:
    EmptyShape(QQuick3DNode *parent = nullptr);
    ~EmptyShape() override;

protected:
    void createJoltShape() override;
};

#endif // EMPTYSHAPE_P_H
