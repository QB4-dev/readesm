#include "EsmFile.h"

#include "config.h"

#include "blockFactory.h"
#include "CardBlocks/CardCertificate.h"
#include "CardBlocks/Identification.h"
#include "CardBlocks/MemberStateCertificate.h"
#include "DataTypes/EncryptedCertificate.h"
#include "Reporter/Reporter.h"
#include "VuBlocks/VuOverview.h"
#include "VuBlocks/VuActivities.h"
#include "loadFile.h"

#include <QtCore/QFile>
#include <QtCore/QObject>

template <typename base, typename derived>
QSharedPointer<derived> findTypeInVector(QVector< QSharedPointer<base> > array){
	QSharedPointer<derived> pointer;
	for(int j = 0; j < array.size(); ++j){
		pointer = qSharedPointerDynamicCast<derived>(array[j]);
		if(!pointer.isNull()) return pointer;
	}
	return pointer;
}

template <typename base, typename derived>
QVector< QSharedPointer<derived> >  findManyInVector(QVector< QSharedPointer<base> > array){
	QVector< QSharedPointer<derived> > rv;
	QSharedPointer<derived> pointer;
	for(int j = 0; j < array.size(); ++j){
		pointer = qSharedPointerDynamicCast<derived>(array[j]);
		if(!pointer.isNull()) rv.append(pointer);
	}
	return rv;
}

Reporter& operator<<(Reporter& report, const EsmFile& e) {
	e.printOn(report);
// 	report.title = e.name();
// 	report.bigblockstart(QObject::tr("Statistics"));
// 	report.single(QObject::tr("Statistics for") + " " + e.title, true);
// 	report.single(formatRange(e.first, e.last));
// 	report(QObject::tr("Recorded days"), e.daycount);
// 	report(QObject::tr("Overall driven distance"), QString("%1 km").arg(e.drivenkm));
// 	report(QObject::tr("Overall driving time"), formatMinutes(e.drivenminutes));
// 	if(e.daycount)
// 	{
// 		report(QObject::tr("Average distance per day"), QString("%1 km").arg(e.drivenkm / e.daycount));
// 		report(QObject::tr("Average time driven per day"), formatMinutes(e.drivenminutes
// 				/ e.daycount) + " (" + stringify(100* e .drivenminutes
// 				/ (e.daycount * 24* 60 ))+ "%)");
// 	}
// 	if(e.drivenminutes)
// 		report(QObject::tr("Average speed when driving"), stringify(e.drivenkm * 60 / e.drivenminutes) + " km/h");
// 	report.bigblockend();
// 
// 	for(esmfile::subiter i = e.blocks.begin(); i < e.blocks.end(); ++i) report << **i;
// 
// #ifndef HAVE_NO_CRYPTO
// 	report.bigblockstart(QObject::tr("Key chain"));
// 	report << *e.CAcert;
// 	report << *e.devicecert;
// 	report.bigblockend();
// #endif
	return report;

}
void EsmFile::printOn(Reporter& report) const{
	for(int j = 0; j < blocks.size(); ++j) {
		report.topLevelBlock(*blocks[j]);
	}
	report.flush();
}

EsmFile::EsmFile(const QString& filename) : fileData(loadFile(filename)) {
	DataPointer fileWalker(fileData);
	while(fileWalker.bytesLeft() > 0) {
		QSharedPointer<TopLevelBlock> p(blockFactory(fileWalker));
		blocks.append(p);
		fileWalker += p->size();
	}
#ifdef HAVE_CRYPTO
	PlainCertificate ercaKey(DataPointer(loadFile(":/EC_PK.bin")));
	EncryptedCertificate* caCertificate = NULL;
	EncryptedCertificate* deviceCertificate = NULL;
	QSharedPointer<VuOverview> ov = findTypeInVector<TopLevelBlock, VuOverview>(blocks);
	if(ov){
		caCertificate = &ov->memberStateCertificate;
		deviceCertificate = &ov->vuCertificate;
	} else {
		QSharedPointer<MemberStateCertificate> mc = findTypeInVector<TopLevelBlock, MemberStateCertificate>(blocks);
		if(mc) caCertificate = &mc->certificate;
		QSharedPointer<CardCertificate> dc = findTypeInVector<TopLevelBlock, CardCertificate>(blocks);
		if(dc) deviceCertificate = &dc->certificate;
	}
	if(caCertificate) caCertificate->attemptVerificationFrom(ercaKey);
	if(deviceCertificate) deviceCertificate->attemptVerificationFrom(*caCertificate);
	if(deviceCertificate && deviceCertificate->isVerified()){
		for(int j = 0; j < blocks.size(); ++j) {
			blocks[j]->checkSignature(*deviceCertificate);
		}
	}
#endif
}



QString EsmFile::suggestTitle() const {
	QSharedPointer<VuOverview> ov = findTypeInVector<TopLevelBlock, VuOverview>(blocks);
	QSharedPointer<Identification> id = findTypeInVector<TopLevelBlock, Identification>(blocks);
	//QSharedPointer<CardDriverActivity> act = findTypeInVector<Block, VuOverview>(blocks);
	QString rv("Esm data");
	if(!ov.isNull()) rv = ov->vehicleRegistrationIdentification.vehicleRegistrationNumber;
	if(!id.isNull()) rv = id->cardHolderName.toString();
	return rv;
}

QString EsmFile::suggestFileName() const {
	return suggestTitle();
}