#ifndef ACTIONS_H
#define ACTIONS_H

#include <QObject>

class QsciApp;
class QAction;
class QMenu;
class QToolBar;
class QLineEdit;

class Actions : public QObject
{
	Q_OBJECT

	enum Dir {
		DirNone = 0,
		DirForwards,
		DirBackwards
	};
public:
	Actions(QsciApp*);
	~Actions();

public slots:
	void find();
	void findText(bool);
	void findNext();
	void findPrev();
	void clearSearchDir();

private:
	inline QsciApp *application();
	void setupActions();
	void setupMenus();
	void setupToolBars();

	enum Dir lastSearchDir;
	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *viewMenu;
	QMenu *settingsMenu;
	QMenu *helpMenu;
	QToolBar *fileToolBar;
	QToolBar *editToolBar;
	QToolBar *findToolBar;
	QLineEdit *findEntry;

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
	QAction *suggestCompletion;

	QAction *lineNumAct;
	QAction *whiteSpaceAct;
	QAction *wrapTextAct;
	QAction *hlCurrentAct;
	QAction *foldAllAct;

	QAction *foldAct;
	QAction *autoCompAct;
	QAction *braceMatchAct;

	QAction *aboutAct;
	QAction *aboutQtAct;
};

#endif

