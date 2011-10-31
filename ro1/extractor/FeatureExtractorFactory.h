#ifndef FEATUREEXTRACTORFACTORY_H
#define FEATUREEXTRACTORFACTORY_H

#include <QStringList>

class FeatureExtractorInterface;

class FeatureExtractorFactory
{
public:
	static FeatureExtractorInterface *getExtractor(QStringList args);
};

#endif // FEATUREEXTRACTORFACTORY_H
