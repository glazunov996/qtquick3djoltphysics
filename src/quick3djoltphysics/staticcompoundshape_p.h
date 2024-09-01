#ifndef STATICCOMPOUNDSHAPE_P_H
#define STATICCOMPOUNDSHAPE_P_H

#include "abstractshape_p.h"

#include <QtQml/QQmlEngine>
#include <QtQml/QQmlListProperty>

#include <QVector>

class Q_QUICK3DJOLTPHYSICS_EXPORT StaticCompoundShape : public AbstractShape
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<AbstractShape> shapes READ shapes CONSTANT)
    QML_NAMED_ELEMENT(StaticCompoundShape)
public:
    StaticCompoundShape(QQuick3DNode *parent = nullptr);
    ~StaticCompoundShape() override;

    QQmlListProperty<AbstractShape> shapes();
    const QVector<AbstractShape *> &getShapesList() const;

protected:
    void updateJoltShape() override;

private:
    static void qmlAppendShape(QQmlListProperty<AbstractShape> *list, AbstractShape *shape);
    static AbstractShape *qmlShapeAt(QQmlListProperty<AbstractShape> *list, qsizetype index);
    static qsizetype qmlShapeCount(QQmlListProperty<AbstractShape> *list);
    static void qmlClearShapes(QQmlListProperty<AbstractShape> *list);

    QVector<AbstractShape *> m_shapes;
};

#endif // STATICCOMPOUNDSHAPE_P_H
