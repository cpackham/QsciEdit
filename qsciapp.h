#ifndef QSCIAPP_H
#define QSCIAPP_H

#include <QMainWindow>
class QLineEdit;
class QsciScintilla;
class FindDialog;
class Actions;

class QsciApp : public QMainWindow
{
	Q_OBJECT

public:
	QsciApp(const QString = 0);
	QString getLineCommentString();
	QString getBlockCommentStartString();
	QString getBlockCommentMiddleString();
	QString getBlockCommentEndString();
	QsciScintilla *editor() { return textEdit; };

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
	void setWrapText(bool);
	void lineComment();
	void blockComment();

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

	QsciScintilla *textEdit;
	Actions *actions;
	QString curFile;
	FindDialog *findDialog;

	QString lineCommentString;
	QString blockCommentStartString;
	QString blockCommentMiddleString;
	QString blockCommentEndString;
	bool fileProvided;

};
#endif

