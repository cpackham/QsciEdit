#ifndef QSCIAPP_H
#define QSCIAPP_H

#include <QMainWindow>
class QsciScintilla;

class QsciApp : public QMainWindow
{
	Q_OBJECT

public:
	QsciApp();

public slots:
	void newFile();
	void open();
	bool save();
	bool saveAs();

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void documentWasModified();

private:
	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	void loadFile(const QString &filename);
	bool saveFile(const QString &filename);
	void setCurrentFile(const QString &filename);

	QsciScintilla *textEdit;
	QString curFile;
	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *viewMenu;
	QMenu *settingsMenu;
	QToolBar *fileToolBar;

	QAction *newAct;
	QAction *openAct;
	QAction *saveAct;
	QAction *saveAsAct;
	QAction *exitAct;

};
#endif

