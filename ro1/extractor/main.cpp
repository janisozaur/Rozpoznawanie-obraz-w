#include "ArchiveExtractor.h"
#include "FeatureExtractorInterface.h"
#include "FeatureExtractorFactory.h"

#include <QtCore/QCoreApplication>
#include <QStringList>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	QStringList arguments = QCoreApplication::instance()->arguments();
	if (arguments.count() < 6) {
		QStringList usage;
		usage << arguments.at(0);
		usage << "[input images file]";
		usage << "[input labels file]";
		usage << "[output file]";
		usage << "[limit (use 0 to parse all)]";
		usage << "[extractor]";
		usage << "[extractor params]";
		qDebug() << usage.join(" ");
		qFatal("Too few arguments");
	}

	bool ok;
	quint32 count = arguments.at(4).toInt(&ok);
	if (!ok) {
		qFatal("Argument 4 (%s) is not a number.\n", arguments.at(4).toStdString().c_str());
	}

	QString imagesFileName = arguments.at(1);
	QFile imagesArchive(imagesFileName);
	if (!imagesArchive.open(QIODevice::ReadOnly)) {
		qFatal("Failed to open file %s.\n", imagesFileName.toStdString().c_str());
	}
	QString labelsFileName = arguments.at(2);
	QFile labelsArchive(labelsFileName);
	if (!labelsArchive.open(QIODevice::ReadOnly)) {
		qFatal("Failed to open file %s.\n", labelsFileName.toStdString().c_str());
	}

	ArchiveExtractor ie(&imagesArchive);
	ArchiveExtractor le(&labelsArchive);
	if (ie.itemCount() != le.itemCount() || ie.itemCount() == 0 || le.itemCount() == 0) {
		qFatal("There are no items in the archive or archive sizes mismatch.\n");
	}
	count = qBound((quint32)0, count, ie.itemCount());
	if (count == 0) {
		count = ie.itemCount();
	}
	QFile f(arguments.at(3));
	if (!f.open(QIODevice::WriteOnly)) {
		qFatal("Failed to open file %s for writing.\n", f.fileName().toStdString().c_str());
	}
	QTextStream output(&f);
	FeatureExtractorInterface *fei = FeatureExtractorFactory::getExtractor(arguments.mid(5));
	if (fei == NULL) {
		qFatal("Failed to create \"%s\" extractor.\n", arguments.at(5).toStdString().c_str());
	}
	output << fei->name() << endl;
	output << count << endl;
	output << fei->features(ie.extract(0), ie.itemSize()).size() << endl;
	for (unsigned int i = 0; i < count; i++) {
		//ie.display(1, threshold);
		QVector<float> features = fei->features(ie.extract(i), ie.itemSize());
		output << *le.extract(i) << " ";
		for (int j = 0; j < features.size(); j++) {
			output << features.at(j) << " ";
		}
		output << endl;
		//qDebug() << "written" << features.size() << "features for item" << i << "to" << f.fileName();
	}
	delete fei;
	f.close();

	imagesArchive.close();
	labelsArchive.close();

	return 0;
}
