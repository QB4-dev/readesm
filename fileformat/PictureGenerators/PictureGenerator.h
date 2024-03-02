#ifndef PICTUREGENERATOR_H
#define PICTUREGENERATOR_H

#include <QCoreApplication>
#include <QString>
#include <QTextStream>

class PictureGenerator {
public:
	///returns the string to be used in the document
	virtual QString toString() const;

	///reset to the initial state
	virtual void reset();

protected:
	PictureGenerator();
	QString collected;
	QTextStream collector;
};

#endif
