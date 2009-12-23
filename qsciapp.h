#ifndef QSCIAPP_H
#define QSCIAPP_H

#include <QMainWindow>
class QLineEdit;
class QsciScintilla;
class FindDialog;

class QsciApp : public QMainWindow
{
	Q_OBJECT

	enum Dir {
		DirNone = 0,
		DirForwards,
		DirBackwards
	};

public:
	QsciApp(const QString = 0);
	QString getLineCommentString();
	QString getBlockCommentStartString();
	QString getBlockCommentMiddleString();
	QString getBlockCommentEndString();

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
	void find();
	void findText(bool);
	void findNext();
	void findPrev();
	void about();
	void gotoLine(int);

protected:
	void closeEvent(QCloseEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dragLeaveEvent(QDragLeaveEvent *event);
	void dropEvent(QDropEvent *event);

private slots:
	void documentModified(bool);
	void setLineNumbers(bool);
	void setWhiteSpaceVis(bool);
	void setFolding(bool);
	void setAutoCompletion(bool);
	void setBraceMatching(bool);
	void askForLine();
	void clearSearchDir();
	void setWrapText(bool);
	void lineComment();
	void blockComment();

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
	QToolBar *findToolBar;
	QLineEdit *findEntry;
	FindDialog *findDialog;

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
	QAction *gotoLineAct;
	QAction *gotoBraceAct;
	QAction *selectBraceAct;
	QAction *findAct;
	QAction *findPrevAct;
	QAction *findNextAct;
	QAction *lineCommentAct;
	QAction *blockCommentAct;

	QAction *lineNumAct;
	QAction *whiteSpaceAct;
	QAction *wrapTextAct;

	QAction *foldAct;
	QAction *autoCompAct;
	QAction *braceMatchAct;

	QAction *aboutAct;
	QAction *aboutQtAct;

	enum Dir lastSearchDir;
	QString lineCommentString;
	QString blockCommentStartString;
	QString blockCommentMiddleString;
	QString blockCommentEndString;

};
#endif

