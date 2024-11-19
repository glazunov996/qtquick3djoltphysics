#ifndef HEIGHTFIELDSHAPE_P_H
#define HEIGHTFIELDSHAPE_P_H

#include "abstractshape_p.h"

#include <QUrl>
#include <QtQuick/private/qquickimage_p.h>

class HeightField;

class Q_QUICK3DJOLTPHYSICS_EXPORT HeightFieldShape : public AbstractShape
{
    Q_OBJECT
    Q_PROPERTY(QVector3D extents READ extents WRITE setExtents NOTIFY extentsChanged)
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QQuickImage *image READ image WRITE setImage NOTIFY imageChanged)
    QML_NAMED_ELEMENT(HeightFieldShape)
public:
    HeightFieldShape(QQuick3DNode *parent = nullptr);
    ~HeightFieldShape() override;

    const QUrl &source() const;
    void setSource(const QUrl &source);

    const QVector3D &extents() const;
    void setExtents(const QVector3D &extents);

    QQuickImage *image() const;
    void setImage(QQuickImage *image);

signals:
    void sourceChanged(const QUrl &source);
    void extentsChanged(const QVector3D &extents);
    void imageChanged(QQuickImage *image);

private slots:
    void imageDestroyed(QObject *image);
    void imageGeometryChanged();

protected:
    void createJoltShape() override;

private:
    void updateExtents();

    HeightField *m_heightField = nullptr;

    QUrl m_heightMapSource;
    QVector3D m_extents = { 100.0f, 100.0f, 100.0f };
    bool m_extentsSetExplicitly = false;
    QQuickImage *m_image = nullptr;
};

#endif // HEIGHTFIELDSHAPE_P_H
