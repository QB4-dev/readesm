#ifndef FULLCARDNUMBER_H
#define FULLCARDNUMBER_H

#include "RawFullCardNumber.h"

#include <QMap>

class FullCardNumber : public RawFullCardNumber {
	Q_DECLARE_TR_FUNCTIONS(FullCardNumber)
public:
	explicit FullCardNumber(const DataPointer& filewalker);
	QString toString() const;
	virtual void printOn(Reporter& o) const;
	static QMap<QString, QString>& mapCardNumberToName();
};


#endif
