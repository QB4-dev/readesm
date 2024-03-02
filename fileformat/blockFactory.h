#ifndef BLOCKFACTORY_H
#define BLOCKFACTORY_H

#include <QSharedPointer>

class DataPointer;
class TopLevelBlock;

QSharedPointer<TopLevelBlock> blockFactory(const DataPointer& start);

#endif
