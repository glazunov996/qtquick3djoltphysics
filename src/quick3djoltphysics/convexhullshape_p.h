#ifndef CONVEXHULLSHAPE_P_H
#define CONVEXHULLSHAPE_P_H

#include "meshshape_p.h"

class Q_QUICK3DJOLTPHYSICS_EXPORT ConvexHullShape : public MeshShape
{
    Q_OBJECT
    QML_NAMED_ELEMENT(ConvexHullShape)
public:
    ConvexHullShape(QQuick3DNode *parent = nullptr);
    ~ConvexHullShape() override;

protected:
    void updateJoltShape() override;
};

#endif // CONVEXHULLSHAPE_P_H
