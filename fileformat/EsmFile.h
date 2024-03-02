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

#ifndef ESMFILE_H
#define ESMFILE_H

#include <QByteArray>
#include <QCoreApplication>
#include <QSharedPointer>
#include <QVector>

class QString;
class Reporter;
class TopLevelBlock;

class EsmFile {
	Q_DECLARE_TR_FUNCTIONS(ESMFile)

public:
	QByteArray fileData;
	QVector< QSharedPointer<TopLevelBlock> > blocks;

	explicit EsmFile(const QString& filename);

	QString errorLog() const;

	QString suggestTitle() const;
	QString suggestFileName() const;

	friend Reporter& operator<<(Reporter& report, const EsmFile& e);

protected:
	void printOn(Reporter& report) const;
	QString errors;

};

#endif
