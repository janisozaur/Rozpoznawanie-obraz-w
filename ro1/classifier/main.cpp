#include "FeatureImporter.h"
#include "CpuClassifier.h"

#include <QtCore/QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QElapsedTimer>

#define PERCENTAGE_CONFUSION

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
	QStringList args = a.arguments();
	if (args.size() < 3) {
		QStringList usage;
		usage << args.at(0)
			  << "[train data]"
			  << "[test data]";
		qFatal("Too few arguments. Usage:\n%s\n", usage.join(" ").toStdString().c_str());
	}

	QFile trainFile(args.at(1));
	if (!trainFile.open(QIODevice::ReadOnly)) {
		qFatal("Failed to open train file %s.\n", trainFile.fileName().toStdString().c_str());
	}
	QFile testFile(args.at(2));
	if (!testFile.open(QIODevice::ReadOnly)) {
		qFatal("Failed to open test file %s.\n", testFile.fileName().toStdString().c_str());
	}

    QElapsedTimer loadTimer;
    loadTimer.start();
    FeatureImporter trainFeatures;
    FeatureImporter testFeatures;
#pragma omp sections
    {
#pragma omp section
        {
            trainFeatures.open(&trainFile);
        }
#pragma omp section
        {
            testFeatures.open(&testFile);
        }
    }
    int loadMsecs = loadTimer.elapsed();
    qDebug() << "loading took" << loadMsecs << "msecs";

	trainFile.close();
	testFile.close();

	QVector<QString> hash;
    QVector<qint8> trainClasses;
	for (int i = 0; i < trainFeatures.labels().size(); i++) {
        qint8 index = hash.indexOf(trainFeatures.labels().at(i));
		if (index == -1) {
			QString dbg("Appending label \"%1\" to hash at position %2. It has now value \"%3\"");
			hash.append(trainFeatures.labels().at(i));
			index = hash.size() - 1;
			//qDebug() << dbg.arg(trainFeatures.labels().at(i), QString::number(index), hash.at(index));
		}
		trainClasses.append(index);
	}

    ClassifierInterface *ci = new CpuClassifier();
    QVector<QVector<int> > classes;
    qDebug() << "starting classification";
    QList<int> k;

    bool ok = true;
    int i = 50;
    if (args.size() >= 4) {
        i = qMax(0, args.at(3).toInt(&ok));
    } else {
        ok = false;
    }
    if (!ok) {
        qDebug() << "no k given, assuming k = 50";
        i = 50;
    }
    qDebug() << "initial k:" << i;
    for (; i >= 1; i--) {
        k.append(i);
    }
    QElapsedTimer timer;
    timer.start();
	classes = ci->classify(trainFeatures.features(), testFeatures.features(),
						   trainClasses.constData(), NULL,
						   testFeatures.featuresPerItem(),
						   trainFeatures.itemCount(), testFeatures.itemCount(),
                           k);
    delete ci;
	int msecs = timer.elapsed();
	qDebug() << "calculations took" << msecs << "msecs";
    for (int w = 0; w < classes.size(); w++) {
        int correct = 0;
        QVector<QVector<qreal> > confusionMatrix;
        confusionMatrix.resize(hash.size());
        for (int i = 0; i < confusionMatrix.size(); i++) {
            confusionMatrix[i].resize(hash.size());
        }
        for (int i = 0; i < classes.at(w).size(); i++) {
            /*qDebug() << i;
            qDebug() << classes.at(i);
            qDebug() << hash.at(classes.at(i));
            qDebug() << testFeatures.labels().at(i);*/
            confusionMatrix[hash.indexOf(testFeatures.labels().at(i))][classes.at(w).at(i)]++;
            /*if (hash.at(classes.at(w).at(i)) == QString("5")) {
                qDebug() << "is 5, should be " << testFeatures.labels().at(i);
            }*/
            if (hash.at(classes.at(w).at(i)) == testFeatures.labels().at(i)) {
                correct++;
            }
        }
        QVector<QPair<QString, int> > sorter;
        for (int i = 0; i < hash.size(); i++) {
            sorter << qMakePair(hash.at(i), i);
        }
        qSort(sorter);
        QStringList l;
        for (int i = 0; i < hash.size(); i++) {
            l << sorter.at(i).first;
        }
        QVector<QVector<qreal> > tempConfusionMatrix;
        tempConfusionMatrix.resize(hash.size());
        for (int j = 0; j < confusionMatrix.size(); j++) {
            for (int i = 0; i < sorter.size(); i++) {
                tempConfusionMatrix[j] << confusionMatrix.at(j).at(sorter.at(i).second);
            }
        }
        confusionMatrix = tempConfusionMatrix;
        for (int j = 0; j < confusionMatrix.size(); j++) {
            tempConfusionMatrix[j] = confusionMatrix.at(sorter.at(j).second);
        }
        confusionMatrix = tempConfusionMatrix;
#ifdef PERCENTAGE_CONFUSION
        for (int i = 0; i < confusionMatrix.size(); i++) {
            qreal sum = 0;
            for (int j = 0; j < confusionMatrix.at(i).size(); j++) {
                sum += confusionMatrix.at(j).at(i);
            }
            for (int j = 0; j < confusionMatrix.at(i).size(); j++) {
                confusionMatrix[j][i] = confusionMatrix.at(j).at(i) / sum * 100.0;
            }
        }
#endif
        QTextStream stream(stdout);
        stream << "k: " << k.at(w) << endl;
        stream << "\t&\t" << l.join("\t&\t") << "\\\\" << endl;
        for (int i = 0; i < confusionMatrix.size(); i++) {
            QStringList list;
            list << sorter.at(i).first;
            for (int j = 0; j < confusionMatrix.size(); j++) {
                list << QString::number(confusionMatrix[i][j], 'g', 4);
            }
            const QString joined(list.join("\t&\t"));
            stream << joined << "\\\\" << endl;
        }
        stream << "correct: " << ((float)correct / (float)classes.at(w).size()) * 100 << "%" << endl;
    }
    msecs = timer.elapsed();
    qDebug() << "everything took" << msecs << "msecs";
	return 0;
}
