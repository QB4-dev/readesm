#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "InMemoryUrlSchemeHandler.h"

#include <QByteArray>
#include <QMainWindow>
#include <QSharedPointer>

class EsmFile;

class QString;
class QWebEngineView;

class mainWindow : public QMainWindow {
	Q_OBJECT
	private:
		InMemoryUrlSchemeHandler schemeHandler;
		QSharedPointer<EsmFile> esm;
		QByteArray htmlContent;
	public:
		QWebEngineView* view;
		mainWindow();
	public slots:
		void helpAbout();
		void helpContents();
		void openFile();
		void openFile(const QString& filename);
		void print();
		void saveHtml();
		void saveRaw();
};

#endif
