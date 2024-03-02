#ifndef TIMEREAL_H
#define TIMEREAL_H

#include "../readTypes.h"
#include "../DataPointer.h"

#include <QCoreApplication>
#include <QDateTime>

class TimeReal : public QDateTime {
	Q_DECLARE_TR_FUNCTIONS(TimeReal)
	bool valid_;
	public:
	explicit TimeReal(const DataPointer& start);
	QString toString() const;
	bool isValid() const;
};

#endif
