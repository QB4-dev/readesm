#ifndef CARDACTIVITYDAILYRECORD_H
#define CARDACTIVITYDAILYRECORD_H

#include "Subblocks.h"
#include "ActivityChangeInfo.h"
#include "RawCardActivityDailyRecord.h"


class CardActivityDailyRecord : public RawCardActivityDailyRecord {
	Q_DECLARE_TR_FUNCTIONS(CardActivityDailyRecord)
public:
	Subblocks<ActivityChangeInfo> activityChangeInfos;

	explicit CardActivityDailyRecord(const DataPointer& start);
	int size() const;
	void printOn(Reporter& o) const;
	QString title() const;
};

#endif
