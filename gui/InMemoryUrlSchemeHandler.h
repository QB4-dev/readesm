#ifndef INMEMORYURLSCHEMEHANDLER_H
#define INMEMORYURLSCHEMEHANDLER_H

#include <QBuffer>
#include <QWebEngineUrlSchemeHandler>

class QWebEngineUrlRequestJob;

class InMemoryUrlSchemeHandler : public QWebEngineUrlSchemeHandler {
	private:
	static const QByteArray contentType;

	QBuffer buffer;

	public:
	void setContent(QByteArray &content);
	void requestStarted(QWebEngineUrlRequestJob *request);
};

#endif
