#ifndef VUDETAILEDSPEEDDATA_H
#define VUDETAILEDSPEEDDATA_H

#include "RawVuDetailedSpeedData.h"

class VuDetailedSpeedData : public RawVuDetailedSpeedData {
public:
	explicit VuDetailedSpeedData(const DataPointer& start);
	void printOn(Reporter& report) const;
};

#endif
