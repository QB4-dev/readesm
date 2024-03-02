#include "loadFile.h"

#include <QByteArray>
#include <QFile>
#include <QString>

QByteArray loadFile(const QString& filename) {
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly)) {
		//TODO: error handling;
	}
	QByteArray content = file.readAll();
	file.close();
	return content;
}
