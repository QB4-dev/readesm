#ifndef CHECKSHA1_H
#define CHECKSHA1_H

class QByteArray;
class RawData;

bool checkSha1(const RawData& data, const QByteArray& hash);

#endif
