#ifndef ACTIONS_H
#define ACTIONS_H

#include <QObject>
#include <QList>

class QsciApp;
class QAction;
class QActionGroup;
class QString;
class QMenu;
class QToolBar;
class QLineEdit;
class QEAction;

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
	QToolBar *viewToolBar;
	QToolBar *findToolBar;
	QLineEdit *findEntry;

	QList<QEAction*> fileItems;

	QList<QEAction*> editItems;
	QEAction *findAct;
	QEAction *findPrevAct;
	QEAction *findNextAct;
	QList<QEAction*> editItemsMisc;

	QList<QEAction*> viewItems;

	QList<QEAction*> settingsItems;
	QActionGroup *eolSetting;

	QAction *aboutAct;
	QAction *aboutQtAct;
};

#endif

