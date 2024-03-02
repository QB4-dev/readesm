#ifndef VUCARDIWRECORD_H
#define VUCARDIWRECORD_H

#include "RawVuCardIWRecord.h"

class VuCardIWRecord : public RawVuCardIWRecord {
	Q_DECLARE_TR_FUNCTIONS(VuCardIWRecord)
public:
	explicit VuCardIWRecord(const DataPointer& filewalker);
};


#endif
