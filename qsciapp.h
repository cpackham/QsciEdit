#ifndef QSCIAPP_H
#define QSCIAPP_H

#include <QMainWindow>
class QsciScintilla;

class QsciApp : public QMainWindow
{
	Q_OBJECT

public:
	QsciApp(const QString = 0);

public slots:
	void newFile();
	void open();
	bool save();
	bool saveAs();
	void cut();
	void copy();
	void paste();
	void undo();
	void redo();
	void about();

protected:
	void closeEvent(QCloseEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dragLeaveEvent(QDragLeaveEvent *event);
	void dropEvent(QDropEvent *event);

private slots:
	void documentModified(bool);
	void setLineNumbers(bool);
	void setFolding(bool);
	void setAutoCompletion(bool);
	void setBraceMatching(bool);

private:
	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	void loadFile(const QString &filename);
	bool saveFile(const QString &filename);
	void setCurrentFile(const QString &filename);
	bool saveIfModified();
	void loadSettings();
	void saveSettings();

	QsciScintilla *textEdit;
	QString curFile;
	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *viewMenu;
	QMenu *settingsMenu;
	QMenu *helpMenu;
	QToolBar *fileToolBar;
	QToolBar *editToolBar;

	QAction *newAct;
	QAction *openAct;
	QAction *saveAct;
	QAction *saveAsAct;
	QAction *exitAct;

	QAction *undoAct;
	QAction *redoAct;
	QAction *cutAct;
	QAction *copyAct;
	QAction *pasteAct;

	QAction *lineNumAct;

	QAction *foldAct;
	QAction *autoCompAct;
	QAction *braceMatchAct;

	QAction *aboutAct;
	QAction *aboutQtAct;

};
#endif

