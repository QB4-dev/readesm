#ifndef VUACTIVITIES_H
#define VUACTIVITIES_H

#include "RawVuActivities.h"

class VuActivities : public RawVuActivities {
	Q_DECLARE_TR_FUNCTIONS(VuActivities)
public:
	explicit VuActivities(const DataPointer& filewalker);
	QString title() const;
	void printOn(Reporter& report) const;
};


#endif
