/* Copyright 2009 Andreas Gölzer

 This file is part of ReadESM.

 ReadESM is free software: you can redistribute it and/or modify it under the
 terms of the GNU General Public License as published by the Free Software
 Foundation, either version 3 of the License, or (at your option) any later
 version.

 ReadESM is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with
 ReadESM.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef TOPLEVELBLOCK_H
#define TOPLEVELBLOCK_H

#include "Block.h"
#include "Reporter/Reporter.h"

#include <QCoreApplication>

class EncryptedCertificate;
class RawData;
class QString;

class TopLevelBlock : public Block {
	Q_DECLARE_TR_FUNCTIONS(TopLevelBlock)

public:
	explicit TopLevelBlock(const DataPointer& start);
	virtual QString title() const;
	virtual void checkSignature(const EncryptedCertificate& cert);
	virtual QString signatureValidity() const;
	friend Reporter& operator<<(Reporter& report, const TopLevelBlock& b);
	enum SignatureState { UNKNOWN, NOTPRESENT, NOTPRESENTBUTSHOULDBE, UNCHECKED, VALID, INVALID };
protected:
	virtual RawData signedBytes() const = 0;
	virtual RawData signatureBytes() const;
	virtual void printOn(Reporter& o) const;
	SignatureState signatureState;
	bool validSignature;
	bool hasSignature;
};

#endif
