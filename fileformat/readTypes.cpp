#include "readTypes.h"

#include "DataPointer.h"

#include <QChar>
#include <QDebug>
#include <QString>
#include <QTextCodec>
#include <QtGlobal>

bool checkString(const DataPointer& start, int length) {
	for (int j = 0; j < length; ++j) {
		if (start[j] > 0x20 && start[j] != 0xFF && start[j] != '?') {
			return true;
		}
	}
	return false;
}

inline bool isISO8859CodePageNumber(unsigned char number) {
	return number <= 16 && number != 0 && number != 12;
}

QString codepageStringCombination(const DataPointer& start, int length) {
	if (!checkString(start + 1, length - 1)) {
		return QString();
	}

	QString rv;
	QTextCodec *codec = NULL;

	if (isISO8859CodePageNumber(start[0])) {
		QString codecName = QString("ISO-8859-%1").arg(start[0]);
		codec = QTextCodec::codecForName(codecName.toLatin1());
		if (codec == NULL) {
			qDebug() << "Failed to load codec" << codecName;
		}
	} else {
		qDebug() << start[0] << "is not a code page";
	}

	if (codec != NULL) {
		QTextDecoder *decoder = codec->makeDecoder();
		rv = decoder->toUnicode(start.toPointer(1), length - 1);

		delete decoder;
	} else {
		rv = QString::fromLatin1(start.toPointer(1), length - 1);
	}

	// Fix for bug #9 "Premature end of document".
	// Null characters violate the standard, so it should be OK to do this here.
	rv.replace(QChar(), QChar(QChar::SpecialCharacter::Space), Qt::CaseInsensitive);
	return rv.trimmed();
}

QString fixedString(const DataPointer& start, int length) {
	if (!checkString(start, length)) {
		return QString();
	}
	QString rv = QString::fromLatin1(start.toPointer(0), length).trimmed();
	if (isISO8859CodePageNumber(start[0])) {
		qDebug() << rv << "might be a code page-string combination, code page" << (int)start[0] << "parsing as such";
		return codepageStringCombination(start, length);
	}
	return rv;
}
/*
How about this? Would reduce the size of the API and eliminate duplicate code.
Also, one can easily switch to a longer data type.
int readBigEndianInt(const DataPointer& start, int length) {
	int res = start[0];
	int i;
	for (i = 1; i < length; i++) {
		res <<= 8;
		res += start[i];
	}
	return res;
}
*/

int readBigEndianInt1(const DataPointer& start) {
	return start[0];
}

int readBigEndianInt2(const DataPointer& start) {
	return (start[0] << 8) + start[1];
}

int readBigEndianInt3(const DataPointer& start) {
	return (start[0] << 16) + (start[1] << 8) + start[2];
}

int readBigEndianInt4(const DataPointer& start) {
	return (start[0] << 24) + (start[1] << 16) + (start[2] << 8) + start[3];
}

int readBCD2(const DataPointer& start) {
	return (start[0] >> 4) * 1000 + (start[0] & 0x0F) * 100 + (start[1] >> 4) * 10 + (start[1] & 0x0F);
}

QString bcdbyte(unsigned char start) {
	char resAsChars[2] = {(char) ((start >> 4) | 0x30), (char) ((start & 0x0F) | 0x30)};
	return QString::fromLatin1(resAsChars, 2);
}

QString hexByte(unsigned char start) {
	return QString("%1").arg(start, 2, 16, QChar('0'));
}
