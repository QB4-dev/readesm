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

#ifndef VUBLOCK_H
#define VUBLOCK_H

#include "../TopLevelBlock.h"

class VuBlock : public TopLevelBlock {
	public:
	virtual QString title() const = 0;
	explicit VuBlock(const DataPointer& nstart);
	virtual int size() const = 0;
	virtual void printOn(Reporter& report) const = 0;
	virtual RawData signedBytes() const;
};

#endif
