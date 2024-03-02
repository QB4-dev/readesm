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

#ifndef BLOCK_H
#define BLOCK_H

#include "DataPointer.h"
#include "Reporter/Reporter.h"

// Intentionally added here for auto-generated subclasses
#include "readTypes.h"

#include <QCoreApplication>

class QString;

class Block {
	Q_DECLARE_TR_FUNCTIONS(Block)
	public:
	explicit Block(const DataPointer& filewalker);
    virtual ~Block() {}

	///Size of data used in the file
	/** If the data element uses 3 bytes in the file, size() will return 3. */
	virtual int size() const = 0;

	///Name of block element in the specification (or something meaningful if there is no name in the law)
	/** This usually matches the name of the c++-class used to read and show the data */
	virtual QString className() const = 0;

	///Possible title for the block (empty if unset)
	virtual QString title() const;

	///Possible short form of the block (empty if unset)
	virtual QString toString() const;

	friend Reporter& operator<<(Reporter& report, const Block& b);

	protected:
	virtual void printOn(Reporter& o) const = 0;
	DataPointer start;
};

#endif
