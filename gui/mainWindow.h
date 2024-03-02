#ifndef mainWindow_H
#define mainWindow_H

#include <QMainWindow>
#include <QSharedPointer>
#include <QByteArray>

class EsmFile;
class QString;
class QWebView;

class mainWindow : public QMainWindow
{
	Q_OBJECT
	private:
		QSharedPointer<EsmFile> esm;
		QByteArray htmlContent;
	public:
		QWebView* view;
		mainWindow();
	public slots:
		void openFile();
		void openFile(const QString& filename);
		void helpContents();
		void print();
		void saveRaw();
		void saveHtml();
		void helpAbout();
};

#endif
