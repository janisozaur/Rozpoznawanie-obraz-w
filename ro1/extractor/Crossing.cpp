#include "Crossing.h"

#define EXPECTED_ARGS 1

Crossing::Crossing(QStringList args)
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

QVector<float> Crossing::features(const quint8 *data, const QSize size) const
{
    QVector<float> result;
    const int height = size.height();
    const int width = size.width();
    for (int i = 0; i < height; i++) {
        bool belowThresh = true;
        int cnt = 0;
        for (int j = 0; j < width; j++) {
            if ((data[i * width + j] >  mThreshold &&  belowThresh) ||
                (data[i * width + j] <= mThreshold && !belowThresh))
            {
                belowThresh = !belowThresh;
                cnt++;
            }
        }
        result.append(cnt);
    }
    for (int i = 0; i < width; i++) {
        bool belowThresh = true;
        int cnt = 0;
        for (int j = 0; j < height; j++) {
            if ((data[i + j * width] >  mThreshold &&  belowThresh) ||
                (data[i + j * width] <= mThreshold && !belowThresh))
            {
                belowThresh = !belowThresh;
                cnt++;
            }
        }
        result.append(cnt);
    }
    return result;
}

QString Crossing::name() const
{
    return QString("Crossing");
}
