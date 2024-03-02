#ifndef DATATYPE_H
#define DATATYPE_H

#include "../Block.h"

class DataPointer;
class Reporter;

class DataType : public Block {
public:
	explicit DataType(const DataPointer& nstart);

	friend Reporter& operator<<(Reporter& report, const DataType& d);
	virtual void printOn(Reporter& report) const  = 0;
	virtual int size() const = 0;
	virtual bool operator==(const DataType& other) const;
	virtual bool operator!=(const DataType& other) const;
	virtual bool isDefaultValue() const;
};

#endif
