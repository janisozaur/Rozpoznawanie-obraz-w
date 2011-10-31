#include "FeatureExtractorFactory.h"
#include "LeftRightProfile.h"
#include "Projection.h"
#include "Crossing.h"
#include "Zones.h"

FeatureExtractorInterface *FeatureExtractorFactory::getExtractor(QStringList args)
{
	FeatureExtractorInterface *extractor = NULL;
	if (args.isEmpty()) {
		return extractor;
	} else if (args.at(0) == "LeftRightProfile") {
		extractor = new LeftRightProfile(args.mid(1));
    } else if (args.at(0) == "Projection") {
        extractor = new Projection(args.mid(1));
    } else if (args.at(0) == "Crossing") {
        extractor = new Crossing(args.mid(1));
    } else if (args.at(0) == "Zones") {
        extractor = new Zones(args.mid(1));
    }
	return extractor;
}
