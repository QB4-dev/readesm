#ifndef CARDSLOTS_H
#define CARDSLOTS_H

#include "RawCardSlots.h"

class CardSlots : public RawCardSlots {
	Q_DECLARE_TR_FUNCTIONS(CardSlots)
public:
	explicit CardSlots(const DataPointer& filewalker);
	virtual void printOn(Reporter& report) const;
};

#endif
