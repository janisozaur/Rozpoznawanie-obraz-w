#ifndef CROSSING_H
#define CROSSING_H

#include "FeatureExtractorInterface.h"

#include <QStringList>

class Crossing : public FeatureExtractorInterface
{
public:
    Crossing(QStringList args);
    QVector<float> features(const quint8 *data, const QSize size) const;
    QString name() const;

private:
    quint8 mThreshold;
};

#endif // CROSSING_H
