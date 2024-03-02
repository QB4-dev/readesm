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

#ifndef REPORTER_H
#define REPORTER_H

#include <QCoreApplication>
#include <QByteArray>
#include <QString>
#include <QTextStream>

class DataType;
class Block;

class Reporter {
	Q_DECLARE_TR_FUNCTIONS(Reporter)
public:
	virtual void tagValuePair(const QString& tag, const QString& value) = 0;
	virtual void tagValuePair(const QString& tag, int value);
	virtual void writeBlock(const Block& value, const QString& tag = QString());
	///return the QByteArray that contains the report, ready for being written to a file.
	virtual QByteArray toQByteArray() const = 0;
	virtual void setTitle(const QString& newTitle);
	virtual bool allowSvg() const = 0;
	void flush();
	Reporter();
	template <typename Arraytype>
	void writeArray(const Arraytype& ray, const QString& title = QString(), bool defaultShown = true) {
		arrayStart(ray.numberOfBlocks(), title, defaultShown);
		++nestLevel;
		for (int j = 0; j < ray.numberOfBlocks(); ++j) {
			this->writeBlock(ray[j]);
		}
		--nestLevel;
		arrayEnd(ray.numberOfBlocks());
	}
protected:
	int nestLevel;
	QByteArray collected;
	mutable QTextStream collector;
	QString title;
	virtual void subBlock(const Block& value, const QString& tag) = 0;
	virtual void arrayStart(int count, const QString& title, bool defaultShown) {}
	virtual void arrayEnd(int count) {}
};

#endif
