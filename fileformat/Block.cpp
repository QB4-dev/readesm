#include "Block.h"

#include "config.h"

#include <QString>

Block::Block(const DataPointer& filewalker) :
	start(filewalker)
{
}

Reporter& operator<<(Reporter& report, const Block& b) {
	b.printOn(report);
	return report;
}

QString Block::title() const {
	return QString();
}

QString Block::toString() const {
	return QString();
}
