#ifndef LEFTRIGHTPROFILE_H
#define LEFTRIGHTPROFILE_H

#include "FeatureExtractorInterface.h"

#include <QStringList>

class LeftRightProfile : public FeatureExtractorInterface
{
public:
	LeftRightProfile(QStringList args);
	QVector<float> features(const quint8 *data, const QSize size) const;
	QString name() const;

private:
	quint8 mThreshold;
};

#endif // LEFTRIGHTPROFILE_H
