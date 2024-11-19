#include "heightfieldshape_p.h"

#include "physicsutils_p.h"

#include <QFileInfo>
#include <QImage>
#include <QVector>
#include <QHash>
#include <QQmlContext>
#include <QQmlFile>
#include <QDebug>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/HeightFieldShape.h>

#include "physicssystem_p.h"

class HeightField
{
public:
    HeightField(const QString &qmlSource);
    HeightField(QQuickImage *image);
    ~HeightField();

    void ref() { ++m_refCount; }
    int deref() { return --m_refCount; }
    void writeSamples(const QImage &heightMap);
    QVector<float> *heightFieldSamples();
    int sampleCount() const;

    int rows() const;
    int columns() const;

private:
    QString m_sourcePath;
    QQuickImage *m_image = nullptr;
    QVector<float> *m_samples = nullptr;
    int m_sampleCount = 0;
    int m_rows = 0;
    int m_columns = 0;
    int m_refCount = 0;
};

class HeightFieldManager
{
public:
    static HeightField *getHeightField(const QUrl &source, const QObject *contextObject);
    static HeightField *getHeightField(QQuickImage *source);
    static void releaseHeightField(HeightField *heightField);

private:
    static QHash<QString, HeightField *> heightFieldHash;
    static QHash<QQuickImage *, HeightField *> heightFieldImageHash;
};

QHash<QString, HeightField *> HeightFieldManager::heightFieldHash;
QHash<QQuickImage *, HeightField *> HeightFieldManager::heightFieldImageHash;

HeightField *HeightFieldManager::getHeightField(const QUrl &source, const QObject *contextObject)
{
    const QQmlContext *context = qmlContext(contextObject);

    const auto resolvedUrl = context ? context->resolvedUrl(source) : source;
    const auto qmlSource = QQmlFile::urlToLocalFileOrQrc(resolvedUrl);

    auto *heightField = heightFieldHash.value(qmlSource);
    if (!heightField) {
        heightField = new HeightField(qmlSource);
        heightFieldHash[qmlSource] = heightField;
    }
    heightField->ref();
    return heightField;
}

HeightField *HeightFieldManager::getHeightField(QQuickImage *source)
{
    auto *heightField = heightFieldImageHash.value(source);
    if (!heightField) {
        heightField = new HeightField(source);
        heightFieldImageHash[source] = heightField;
    }
    heightField->ref();
    return heightField;
}

void HeightFieldManager::releaseHeightField(HeightField *heightField)
{
    if (heightField != nullptr && heightField->deref() == 0) {
        qCDebug(lcQuick3dJoltPhysics()) << "deleting height field" << heightField;
        erase_if(heightFieldHash,
                 [heightField](std::pair<const QString &, HeightField *&> h) {
                     return h.second == heightField;
                 });
        erase_if(heightFieldImageHash,
                 [heightField](std::pair<QQuickImage *, HeightField *&> h) {
                     return h.second == heightField;
                 });
        delete heightField;
    }
}

HeightField::HeightField(const QString &qmlSource)
    : m_sourcePath(qmlSource)
{
}

HeightField::HeightField(QQuickImage *image)
    : m_image(image)
{
}

HeightField::~HeightField()
{
    delete m_samples;
    m_samples = nullptr;
}

void HeightField::writeSamples(const QImage &heightMap)
{
    m_rows = heightMap.height();
    m_columns = heightMap.width();

    int n = 1;
    int size = qMin(m_rows, m_columns);

    while (n * 2 <= size)
        n *= 2;

    m_samples = new QVector<float>();
    auto &samples = *m_samples;
    samples.resize(n * n);
    m_sampleCount = n;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            const auto pixel = heightMap.pixelColor(j, i);
            if (pixel.isValid()) {
                float f = pixel.valueF() - 0.5f;
                samples[i * n + j] = f;
            } else {
                samples[i * n + j] = JPH::HeightFieldShapeConstants::cNoCollisionValue;
            }
        }
    }
}

QVector<float> *HeightField::heightFieldSamples()
{
    if (m_samples)
        return m_samples;

    if (m_image == nullptr && m_sourcePath.isEmpty())
        return nullptr;

    const bool readFromFile = m_image == nullptr;

    if (readFromFile)
        writeSamples(QImage(m_sourcePath));
    else
        writeSamples(m_image->image());

    return m_samples;
}

int HeightField::sampleCount() const
{
    return m_sampleCount;
}

int HeightField::rows() const
{
    return m_rows;
}

int HeightField::columns() const
{
    return m_columns;
}

HeightFieldShape::HeightFieldShape(QQuick3DNode *parent) : AbstractShape(parent)
{
}

HeightFieldShape::~HeightFieldShape()
{
    if (m_heightField)
        HeightFieldManager::releaseHeightField(m_heightField);
}

const QUrl &HeightFieldShape::source() const
{
    return m_heightMapSource;
}

void HeightFieldShape::setSource(const QUrl &source)
{
    if (m_heightMapSource == source)
        return;

    m_heightMapSource = source;
    if (m_image == nullptr) {
        HeightFieldManager::releaseHeightField(m_heightField);
        m_heightField = nullptr;
    }

    emit sourceChanged(source);

    if (m_image == nullptr && !source.isEmpty()) {
        m_heightField = HeightFieldManager::getHeightField(m_heightMapSource, this);
        updateJoltShape();
        emit changed();
    }
}

const QVector3D &HeightFieldShape::extents() const
{
    return m_extents;
}

void HeightFieldShape::setExtents(const QVector3D &extents)
{
    m_extentsSetExplicitly = true;
    if (m_extents == extents)
        return;

    m_extents = extents;
    updateJoltShape();

    emit extentsChanged(extents);
    emit changed();
}

QQuickImage *HeightFieldShape::image() const
{
    return m_image;
}

void HeightFieldShape::setImage(QQuickImage *image)
{
    if (m_image == image)
        return;

    if (m_image)
        m_image->disconnect(this);

    m_image = image;

    if (m_image != nullptr) {
        connect(m_image, &QObject::destroyed, this, &HeightFieldShape::imageDestroyed);
        connect(m_image, &QQuickImage::paintedGeometryChanged, this,
                &HeightFieldShape::imageGeometryChanged);
    }

    HeightFieldManager::releaseHeightField(m_heightField);
    m_heightField = nullptr;

    if (m_image != nullptr)
        m_heightField = HeightFieldManager::getHeightField(m_image);
    else if (!m_heightMapSource.isEmpty())
        m_heightField = HeightFieldManager::getHeightField(m_heightMapSource, this);

    updateJoltShape();

    emit imageChanged(image);
    emit changed();
}

void HeightFieldShape::imageDestroyed(QObject *image)
{
    Q_ASSERT(m_image == image);

    setImage(nullptr);
}

void HeightFieldShape::imageGeometryChanged()
{
    Q_ASSERT(m_image);

    HeightFieldManager::releaseHeightField(m_heightField);
    m_heightField = HeightFieldManager::getHeightField(m_image);

    emit changed();
}

void HeightFieldShape::createJoltShape()
{
    m_joltShape = new JPH::HeightFieldShape();
    if (!m_heightField)
        return;

    const auto *samples = m_heightField->heightFieldSamples();
    float rows = m_heightField->rows();
    float cols = m_heightField->columns();
    updateExtents();
    if (samples && cols > 1 && rows > 1) {
        const auto sampleCount = m_heightField->sampleCount();
        const auto hfOffset = JPH::Vec3(-m_extents.x() / 2, 0, -m_extents.z() / 2);
        const auto hfScale = JPH::Vec3(m_extents.x() / (sampleCount - 1), m_extents.y(), m_extents.z() / (sampleCount - 1));

        JPH::HeightFieldShapeSettings settings(samples->data(), hfOffset, hfScale, sampleCount);
        settings.mBlockSize = 4;
        m_joltShape = settings.Create().Get();

        qCDebug(lcQuick3dJoltPhysics) << "created height field"
                                      << m_extents << m_heightField->columns()
                                      << m_heightField->rows();
    }
}

void HeightFieldShape::updateExtents()
{
    if (!m_heightField || m_extentsSetExplicitly)
        return;
    const auto numRows = m_heightField->rows();
    const auto numCols = m_heightField->columns();
    const auto &prevExt = m_extents;
    if (numRows == numCols) {
        m_extents = { 100.0f, 100.0f, 100.0f };
    } else if (numRows < numCols) {
        float f = float(numRows) / float(numCols);
        m_extents = { 100.0f, 100.0f, 100.0f * f };
    } else {
        float f = float(numCols) / float(numRows);
        m_extents = { 100.0f * f, 100.0f, 100.0f };
    }
    if (m_extents != prevExt)
        emit extentsChanged(m_extents);
}
