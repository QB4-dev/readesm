#ifndef CARDDRIVERACTIVITY_H
#define CARDDRIVERACTIVITY_H

#include "RawCardDriverActivity.h"
#include "../DataTypes/CardActivityDailyRecord.h"
#include "../DataTypes/Subblocks.h"

class CardDriverActivity : public RawCardDriverActivity {
	Q_DECLARE_TR_FUNCTIONS(CardDriverActivity)
	QByteArray readCyclicData();
public:
	QByteArray activityDataUncycled;
	Subblocks<CardActivityDailyRecord> cardActivityDailyRecords;

	explicit CardDriverActivity(const DataPointer& start);
	void printOn(Reporter& report) const;
};

#endif
