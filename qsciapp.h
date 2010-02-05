#ifndef QSCIAPP_H
#define QSCIAPP_H

#include <QMainWindow>
class QLineEdit;
class QFileInfo;
class QsciScintilla;
class SearchDialog;
class SearchOptions;
class Actions;
class EditorSettings;

class QsciApp : public QMainWindow
{
	Q_OBJECT

public:
	QsciApp(const QString = 0, unsigned int = 1);
	QString getLineCommentString();
	QString getBlockCommentStartString();
	QString getBlockCommentMiddleString();
	QString getBlockCommentEndString();
	QsciScintilla *editor() { return textEdit; };
	EditorSettings *editorSettings;

public slots:
	void newFile();
	void open();
	void reload();
	bool save();
	bool saveAs();
	void cut();
	void copy();
	void paste();
	void undo();
	void redo();
	void about();
	void gotoLine(int, int=0);

signals:
	void applicationFocusIn();

protected:
	void closeEvent(QCloseEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dragLeaveEvent(QDragLeaveEvent *event);
	void dropEvent(QDropEvent *event);
	bool eventFilter(QObject *object, QEvent *event);

private slots:
	void documentModified(bool);
	void setLineNumbers(bool);
	void setWhiteSpaceVis(bool);
	void setFolding(bool);
	void setAutoCompletion(bool);
	void setAutoCompletion(bool, int);
	void setBraceMatching(bool);
	void setHighlightCurrentLine(bool);
	void setAutoIndent(bool);
	void setDisplayEdge(bool, int col=80);
	void askForLine();
	void setWrapText(bool);
	void lineComment();
	void blockComment();
	void checkModifiedOnDisk();
	void setEolModeWindows(bool);
	void setEolModeUnix(bool);
	void setEolModeMac(bool);
	void positionChanged(int,int);
	void search();
	void searchText(const QString, SearchOptions*);
	void replaceWithText(const QString text);

private:
	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	void loadFile(const QString &filename);
	void loadStdIn();
	bool saveFile(const QString &filename);
	void setCurrentFile(const QString &filename);
	bool saveIfModified();
	void loadSettings();
	void saveSettings();
	void applySettings();
	void doReload();
	void askReload();
	void askReloadOrKeep();
	void setEolMode(const QString);
	void setIndentationsUseTabs(bool, int);
	void markLine(int);

	QsciScintilla *textEdit;
	Actions *actions;
	QString curFile;
	QFileInfo *curFileInfo;
	SearchDialog *searchDialog;
	struct FileArgs {
		QString fileName;
		int line;
		int index;
	};
	
	FileArgs commandLineFile; // File specified on the command line
	FileArgs settingsFile;    // File saved in the confguration

	QString lineCommentString;
	QString blockCommentStartString;
	QString blockCommentMiddleString;
	QString blockCommentEndString;
	bool fileProvided;

};
#endif

