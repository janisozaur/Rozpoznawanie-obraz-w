#include "LeftRightProfile.h"

#include <QDebug>

#define EXPECTED_ARGS 1

LeftRightProfile::LeftRightProfile(QStringList args)
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

QVector<float> LeftRightProfile::features(const quint8 *data, const QSize size) const
{
	QVector<float> result;
	const int height = size.height();
	const int width = size.width();
    int maxLeft = width - 1;
    int maxRight = 0;
	for (int i = 0; i < height; i++) {
		bool found = false;
		for (int j = 0; j < width; j++) {
			if (data[i * width + j] > mThreshold) {
				result.append(j);
                if (j < maxLeft) {
					maxLeft = j;
				}
				found = true;
				break;
			}
		}
		if (!found) {
			result.append(width - 1);
		}
	}
    for (int i = 0; i < height; i++) {
		result[i] -= maxLeft;
	}
    for (int i = 0; i < height; i++) {
		bool found = false;
		for (int j = width - 1; j >= 0; j--) {
			if (data[i * width + j] > mThreshold) {
				result.append(j);
                if (j > maxRight) {
					maxRight = j;
				}
				found = true;
				break;
			}
		}
		if (!found) {
			result.append(0);
		}
	}
    for (int i = 0; i < height; i++) {
        result[i + height] += (width - maxRight);
	}
	return result;
}

QString LeftRightProfile::name() const
{
	return QString("Left Right Profile");
}
