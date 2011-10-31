#include "Zones.h"

#define EXPECTED_ARGS 3

Zones::Zones(QStringList args)
{
    if (args.length() < EXPECTED_ARGS) {
        qFatal("Extractor %s expects %d argument: threshold, x zones count, y zones count.\n", name().toStdString().c_str(), EXPECTED_ARGS);
    }
    bool ok;
    mThreshold = qBound(0, args.at(0).toInt(&ok), 255);
    if (!ok) {
        qFatal("Extractor argument %s is not a number.\n", args.at(0).toStdString().c_str());
    }
    mXZones = qBound(0, args.at(1).toInt(&ok), 255);
    if (!ok) {
        qFatal("Extractor argument %s is not a number.\n", args.at(1).toStdString().c_str());
    }
    mYZones = qBound(0, args.at(2).toInt(&ok), 255);
    if (!ok) {
        qFatal("Extractor argument %s is not a number.\n", args.at(2).toStdString().c_str());
    }
}

QVector<float> Zones::features(const quint8 *data, const QSize size) const
{
    QVector<float> result;
    const int height = size.height();
    const int width = size.width();
    const int xZonePixels = (width + mXZones - 1) / mXZones;
    const int yZonePixels = (height + mYZones - 1) / mYZones;
    result.reserve(mXZones * mYZones);
    for (int i = 0; i < mXZones; i++) {
        for (int j = 0; j < mYZones; j++) {
            float sum = 0;
            for (int y = yZonePixels * j; y < qMin(yZonePixels * (j + 1), height); y++) {
                for (int x = xZonePixels * i; x < qMin(xZonePixels * (i + 1), width); x++) {
                    if (data[y * width + x] > mThreshold) {
                        sum += 1;
                    }
                }
            }
            result.append(sum);
        }
    }
    return result;
}

QString Zones::name() const
{
    return QString("Zones");
}
