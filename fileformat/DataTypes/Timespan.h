#ifndef TIMESPAN_H
#define TIMESPAN_H

#include "TimeReal.h"

#include <QCoreApplication>

class Timespan {
	Q_DECLARE_TR_FUNCTIONS(Timespan)
	public:
	TimeReal begin;
	TimeReal end;

	explicit Timespan(const DataPointer& start);

	QString toString() const;
};

#endif
