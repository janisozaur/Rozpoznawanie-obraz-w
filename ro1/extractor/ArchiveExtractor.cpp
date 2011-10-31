#include "ArchiveExtractor.h"

#include <QDebug>

ArchiveExtractor::ArchiveExtractor(QIODevice *data) :
	mStream(new QDataStream(data))
{
	mStream->skipRawData(2);
	quint8 format;
	*mStream >> format;
	if (format != '\x08') {
		qFatal("Improper data format - only unsigned byte (0x08) supported.\n");
	}
	*mStream >> mDepth;
	for (int i = 0; i < mDepth; i++) {
		quint32 dim;
		*mStream >> dim;
		mDimensions << dim;
		qDebug() << "dimension: " << dim;
	}
	mByteSize = 1;
	for (int i = 1; i < mDimensions.count(); i++) {
		mByteSize *= mDimensions.at(i);
	}
	mData = data->readAll();
	qDebug() << "all ok";
}

ArchiveExtractor::~ArchiveExtractor()
{
	delete mStream;
}

quint32 ArchiveExtractor::itemCount() const
{
	return mDimensions.at(0);
}

quint32 ArchiveExtractor::dimensionCount() const
{
	return mDimensions.size();
}

quint32 ArchiveExtractor::sizeInDim(const quint32 dim) const
{
	return mDimensions.at(dim);
}

const quint8 *ArchiveExtractor::extractAll() const
{
	return (const quint8 *)mData.constData();
}

const quint8 *ArchiveExtractor::extract(const unsigned int itemNumber) const
{
	return (const quint8 *)(mData.constData() + itemNumber * mByteSize);
}

quint32 ArchiveExtractor::itemByteSize() const
{
	return mByteSize;
}

void ArchiveExtractor::display(const int itemNumber, const quint8 threshold) const
{
	const quint8 *buffer = extract(itemNumber);
	for (quint32 i = 0; i < mDimensions.at(1); i++) {
		QString row;
		for (quint32 j = 0; j < mDimensions.at(2); j++) {
			if ((const quint8 &)(buffer[i * mDimensions.at(2) + j]) > threshold) {
				row += "#";
			} else {
				row += " ";
			}
		}
		qDebug() << row;
	}
	qDebug() << QString(mDimensions.at(2), '*');
	delete [] buffer;
}

QSize ArchiveExtractor::itemSize() const
{
	return QSize(mDimensions.at(1), mDimensions.at(2));
}
