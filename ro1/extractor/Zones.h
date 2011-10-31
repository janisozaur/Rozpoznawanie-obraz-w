#ifndef ZONES_H
#define ZONES_H

#include "FeatureExtractorInterface.h"

#include <QStringList>

class Zones : public FeatureExtractorInterface
{
public:
    Zones(QStringList args);
    QVector<float> features(const quint8 *data, const QSize size) const;
    QString name() const;

private:
    quint8 mThreshold;
    quint8 mXZones;
    quint8 mYZones;
};

#endif // ZONES_H
