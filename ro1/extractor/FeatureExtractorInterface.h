#ifndef FEATUREEXTRACTORINTERFACE_H
#define FEATUREEXTRACTORINTERFACE_H

#include <QVector>
#include <QSize>

class FeatureExtractorInterface
{
public:
	FeatureExtractorInterface();
	virtual ~FeatureExtractorInterface();
	virtual QVector<float> features(const quint8 *data, const QSize size) const = 0;
	virtual QString name() const = 0;
};

#endif // FEATUREEXTRACTORINTERFACE_H
