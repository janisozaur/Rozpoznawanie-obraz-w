#ifndef ARCHIVEEXTRACTOR_H
#define ARCHIVEEXTRACTOR_H

#include <QVector>
#include <QByteArray>
#include <QSize>

class QIODevice;
class QDataStream;

class ArchiveExtractor
{
private:
	QByteArray mData;
	QDataStream *mStream;
	quint8 mDepth;
	QVector<quint32> mDimensions;
	quint16 mByteSize;
public:
	ArchiveExtractor(QIODevice *data);
	~ArchiveExtractor();
	const quint8 *extractAll() const;
	const quint8 *extract(const unsigned int itemNumber) const;
	void display(const int itemNumber, const quint8 threshold) const;
	quint32 itemCount() const;
	quint32 itemByteSize() const;
	QSize itemSize() const;
	quint32 dimensionCount() const;
	quint32 sizeInDim(const quint32 dim) const;
};

#endif // ARCHIVEEXTRACTOR_H
