#ifndef PROJECTION_H
#define PROJECTION_H

#include "FeatureExtractorInterface.h"

#include <QStringList>

class Projection : public FeatureExtractorInterface
{
public:
    Projection(QStringList args);
    QVector<float> features(const quint8 *data, const QSize size) const;
    QString name() const;

private:
    quint8 mThreshold;
};

#endif // PROJECTION_H
