#ifndef CPUCLASSIFIER_H
#define CPUCLASSIFIER_H

#include "ClassifierInterface.h"

class CpuClassifier : public ClassifierInterface
{
public:
	CpuClassifier();
    QVector<QVector<int> > classify(const float *trainFeatures,
						  const float *testFeatures,
                          const qint8 *trainClasses,
                          const qint8 *testClasses,
						  const quint32 featuresPerItem,
						  const quint32 trainItemCount,
						  const quint32 testItemCount,
						  const QList<int> k);
};

#endif // CPUCLASSIFIER_H
