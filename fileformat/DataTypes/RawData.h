#ifndef RAWDATA_H
#define RAWDATA_H

#include "DataType.h"
#include "../DataPointer.h"

class RawData : public DataType, public DataPointer {
	Q_DECLARE_TR_FUNCTIONS(RawData)
public:
	int length_;
	RawData(const DataPointer& start, int length);
	explicit RawData(const QByteArray& data_);
	QString toString() const;
	QString className() const;

	int size() const;
	QByteArray toQByteArray() const;
	void printOn(Reporter& report) const;
};

#endif
