#include "config.h"
#ifdef HAVE_CRYPTO

#include "LargeNumber.h"

#include "../DataPointer.h"
#include <QString>

LargeNumber::LargeNumber(const DataPointer& start, int length) {
	mpz_init(t);
	mpz_import(t, length, 1, 1, 0, 0, start.toUnsignedPointer());
}

LargeNumber::LargeNumber() {
	mpz_init(t);
}

LargeNumber::~LargeNumber() {
	mpz_clear(t);
}

LargeNumber::LargeNumber(const LargeNumber& o) {
	mpz_init_set(t, o.t);
}

LargeNumber& LargeNumber::operator=(const LargeNumber& o) {
	mpz_init_set(t, o.t);
	return *this;
}

QString LargeNumber::toString() const {
	//This is madness
	char *base10String = mpz_get_str(NULL, 10, t);
	QString res = QString(base10String);
	void (*mpfree) (void *, size_t);
	mp_get_memory_functions (NULL, NULL, &mpfree);
	mpfree(base10String, strlen(base10String) + 1);

	//split in groups of three (avoid space artifact after last triple)
	for (int j = res.length() - 3; j > 0; j -= 3) {
		res.insert(j, ' ');
	}
	return res;
}

#endif
