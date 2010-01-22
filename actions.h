#ifndef ACTIONS_H
#define ACTIONS_H

#include <QObject>
#include <QList>

class QsciApp;
class QAction;
class QString;
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

	QList<QAction*> fileItems;

	QList<QAction*> editItemsUR;
	QList<QAction*> editItemsCCP;
	QAction *findAct;
	QAction *findPrevAct;
	QAction *findNextAct;
	QList<QAction*> editItemsMisc;

	QList<QAction*> viewItems;
	QAction *foldAllAct;

	QList<QAction*> settingsItems;

	QAction *aboutAct;
	QAction *aboutQtAct;
};

#endif

