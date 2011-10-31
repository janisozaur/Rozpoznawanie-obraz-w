#include "Projection.h"

#define EXPECTED_ARGS 1

Projection::Projection(QStringList args)
{
    if (args.length() < EXPECTED_ARGS) {
        qFatal("Extractor %s expects %d arguments.\n", name().toStdString().c_str(), EXPECTED_ARGS);
    }
    bool ok;
    mThreshold = qBound(0, args.at(0).toInt(&ok), 255);
    if (!ok) {
        qFatal("Extractor argument %s is not a number.\n", args.at(0).toStdString().c_str());
    }
}

QVector<float> Projection::features(const quint8 *data, const QSize size) const
{
    QVector<float> result;
    const int height = size.height();
    const int width = size.width();
    result.reserve(width + height);
    for (int i = 0; i < height; i++) {
        float sum = 0;
        for (int j = 0; j < width; j++) {
            if (data[i * width + j] > mThreshold) {
                sum += 1;
            }
        }
        result.append(sum);
    }
    for (int i = 0; i < width; i++) {
        float sum = 0;
        for (int j = 0; j < height; j++) {
            if (data[i + j * width] > mThreshold) {
                sum += 1;
            }
        }
        result.append(sum);
    }
    return result;
}

QString Projection::name() const
{
    return QString("Projection");
}
