#include "InMemoryUrlSchemeHandler.h"

#include <QWebEngineUrlRequestJob>

const QByteArray InMemoryUrlSchemeHandler::contentType = QByteArray("application/xhtml+xml");

void InMemoryUrlSchemeHandler::setContent(QByteArray &content) {
	buffer.setData(QByteArray(content));
}

void InMemoryUrlSchemeHandler::requestStarted(QWebEngineUrlRequestJob *request) {
	request->reply(contentType, &buffer);
}
