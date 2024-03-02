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

#ifndef READTYPES_H
#define READTYPES_H

class DataPointer;
class QString;

QString codepageStringCombination(const DataPointer& start, int length);
QString fixedString(const DataPointer& start, int length);

int readBigEndianInt1(const DataPointer& start);
int readBigEndianInt2(const DataPointer& start);
int readBigEndianInt3(const DataPointer& start);
int readBigEndianInt4(const DataPointer& start);
int readBCD2(const DataPointer& start);


QString bcdbyte(unsigned char start);
QString hexByte(unsigned char start);

// inline int LEInt32(const DataPointer& start) {
// 	return (start[3] << 24) + (start[2] << 16) + (start[1] << 8) + start[0];
// }

#endif
