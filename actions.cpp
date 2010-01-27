#include <QtGui>
#include <qsciscintilla.h>
#include "actions.h"
#include "globals.h"
#include "qticonloader.h"
#include "qsciapp.h"
#include "editorsettings.h"

Actions::Actions(QsciApp* parent) : QObject(parent)
{
	setupActions();
	setupMenus();
	setupToolBars();
	clearSearchDir();
}

Actions::~Actions()
{
}

void Actions::setupActions()
{
	QAction *act;
#define newActionWithIcon(list, shortstr, longstr, keyseq, slot, icon) \
	act = new QAction(icon, shortstr, application()); \
	act->setShortcuts(keyseq); \
	act->setStatusTip(longstr); \
	connect(act, SIGNAL(triggered()), application(), SLOT(slot));\
	list << act

#define newAction(list, shortstr, longstr, key, slot) \
	act = new QAction(shortstr, application()); \
	act->setShortcut(key); \
	act->setStatusTip(longstr); \
	connect(act, SIGNAL(triggered()), application(), SLOT(slot));\
	list << act

#define checkableAction(list, shortstr, longstr, slot, enable) \
	act = new QAction(shortstr, application()); \
	act->setStatusTip(longstr); \
	act->setCheckable(true); \
	act->setChecked(enable); \
	connect(act, SIGNAL(triggered(bool)), application(), SLOT(slot)); \
	list << act


	// File actions
	newActionWithIcon(fileItems,tr("&New"),
			tr("Create a new file"), 
			QKeySequence::New,newFile(),
			QtIconLoader::icon("document-new"));
	newActionWithIcon(fileItems, tr("&Open"),
			tr("Open an existing file"),
			QKeySequence::Open,open(),
			QtIconLoader::icon("document-open"));
	newActionWithIcon(fileItems, tr("&Reload"),
			tr("Reload the current file"),
			QKeySequence::Refresh,reload(),
			QtIconLoader::icon("view-refresh"));
	newActionWithIcon(fileItems, tr("&Save"),
			tr("Save the current file"),
			QKeySequence::Save,save(),
			QtIconLoader::icon("document-save"));
	newActionWithIcon(fileItems, tr("Save &As"),
			tr("Save the current file with a new name"),
			QKeySequence::SaveAs,saveAs(),
			QtIconLoader::icon("document-save-as"));

	// Edit actions
	newActionWithIcon(editItems, tr("&Undo"),
			tr("Undo last edit"),
			QKeySequence::Undo,undo(),
			QtIconLoader::icon("edit-undo"));
	newActionWithIcon(editItems, tr("&Redo"),
			tr("Redo last edit"),
			QKeySequence::Redo,redo(),
			QtIconLoader::icon("edit-redo"));
			
	act = new QAction(application());
	act->setSeparator(true);
	editItems << act;

	newActionWithIcon(editItems, tr("Cu&t"),
			tr("Cut the selected text"),
			QKeySequence::Cut,cut(),
			QtIconLoader::icon("edit-cut"));
	act->setEnabled(false);
	connect(application()->editor(), SIGNAL(copyAvailable(bool)),
		act, SLOT(setEnabled(bool)));

	newActionWithIcon(editItems, tr("&Copy"),
			tr("Copy the selected text to the clipboard"),
			QKeySequence::Copy,copy(),
			QtIconLoader::icon("edit-copy"));
	act->setEnabled(false);
	connect(application()->editor(), SIGNAL(copyAvailable(bool)),
		act, SLOT(setEnabled(bool)));

	newActionWithIcon(editItems, tr("&Paste"),
			tr("Paste text from the clipboard"),
			QKeySequence::Paste,paste(),
			QtIconLoader::icon("edit-paste"));

	act = new QAction(application());
	act->setSeparator(true);
	editItemsMisc << act;

	findAct = new QAction(QtIconLoader::icon("edit-find"), tr("&Find..."), application());
	findAct->setStatusTip(tr("Search for text"));
	findAct->setShortcuts(QKeySequence::Find);
	connect(findAct, SIGNAL(triggered()), this, SLOT(find()));
	editItemsMisc << findAct;

	findPrevAct = new QAction(QtIconLoader::icon("go-previous"), tr("Find &Prev"), application());
	findPrevAct->setStatusTip(tr("Repeat the last search backwards"));
	findPrevAct->setShortcuts(QKeySequence::FindPrevious);
	connect(findPrevAct, SIGNAL(triggered()), this, SLOT(findPrev()));
	editItemsMisc << findPrevAct;

	findNextAct = new QAction(QtIconLoader::icon("go-next"),tr("Find &Next"), application());
	findNextAct->setStatusTip(tr("Repeat the last search forwards"));
	findNextAct->setShortcuts(QKeySequence::FindNext);
	connect(findNextAct, SIGNAL(triggered()), this, SLOT(findNext()));
	editItemsMisc << findNextAct;

	act = new QAction(application());
	act->setSeparator(true);
	editItemsMisc << act;

	newAction(editItemsMisc, tr("&Goto Line"), 
		tr("Jump to a line number"), tr("Ctrl+G"), askForLine());

	act = new QAction(tr("Goto matching brace"), application());
	act->setStatusTip(tr("Jump to the matching brace"));
	act->setShortcut(tr("Ctrl+E"));
	connect(act, SIGNAL(triggered()), application()->editor(), SLOT(moveToMatchingBrace()));
	editItemsMisc << act;

	act = new QAction(tr("Select matching brace"), application());
	act->setStatusTip(tr("Select to matching brace"));
	act->setShortcut(tr("Ctrl+Shift+E"));
	connect(act, SIGNAL(triggered()), application()->editor(), SLOT(selectToMatchingBrace()));
	editItemsMisc << act;

	newAction(editItemsMisc, tr("Line Comment"), 
		tr("Comment out lines"), tr("Ctrl+D"), lineComment());

	newAction(editItemsMisc, tr("Block Comment"),
		tr("Comment out the selected block of text"), 
		tr("Ctrl+Shift+D"), blockComment());

	act = new QAction(tr("Suggest Completion"), application());
	act->setStatusTip(tr("Suggest a completion based on the current text"));
	act->setShortcut(tr("Ctrl+Space"));
	connect(act, SIGNAL(triggered()), application()->editor(), SLOT(autoCompleteFromAll()));
	editItemsMisc << act;

	// View
	checkableAction(viewItems, tr("Line Numbers"),
		tr("Display line numbers in the margin"),
		setLineNumbers(bool),
		application()->editorSettings->displayLineNumbers());
	checkableAction(viewItems, tr("White space"),
		tr("Make white space visible"),
		setWhiteSpaceVis(bool),
		application()->editorSettings->displayWhitespace());
	checkableAction(viewItems, tr("Wrap Text"),
		tr("Wrap text"),
		setWrapText(bool),
		application()->editorSettings->displayWrapText());

	checkableAction(viewItems, tr("Highlight Current Line"),
		tr("Highlight Current Line"),
		setHighlightCurrentLine(bool),
		application()->editorSettings->highlightCurrentLine());

	checkableAction(viewItems, tr("Indicate Edge Column"),
		tr("Display a line marking the edge column"),
		setDisplayEdge(bool),
		application()->editorSettings->displayEdge());

	act = new QAction(application());
	act->setSeparator(true);
	viewItems << act;

	QAction *foldAllAct = new QAction(tr("Toggle All Code Folds"),
				application());
	foldAllAct->setStatusTip(tr("Folds or unfolds all lines"));
	connect(foldAllAct, SIGNAL(triggered()), 
		application()->editor(), SLOT(foldAll()));
	foldAllAct->setEnabled(application()->editorSettings->displayCodeFolding());
	viewItems << foldAllAct;
	
	// Settings
	checkableAction(settingsItems, tr("Code Folding"), 
		tr("Enable/disable code folding"),
		setFolding(bool),
		application()->editorSettings->displayCodeFolding());
	connect(act, SIGNAL(triggered(bool)), foldAllAct, SLOT(setEnabled(bool)));
	checkableAction(settingsItems, tr("Auto completion"),
		tr("Suggest completions for the current text"),
		setAutoCompletion(bool),
		application()->editorSettings->displayAutoComplete());
	checkableAction(settingsItems, tr("Brace Matching"),
		tr("Highlight matching pairs of braces"),
		setBraceMatching(bool), 
		application()->editorSettings->displayBraceMatch());
	checkableAction(settingsItems, tr("Auto Indent"),
		tr("Automatically indent"),
		setAutoIndent(bool), 
		application()->editorSettings->autoIndent());
	
	act = new QAction(application());
	act->setSeparator(true);
	settingsItems << act;
	
	eolSetting = new QActionGroup(application());
	checkableAction(settingsItems, tr("Windows"),
		tr("Windows EOL mode (CRLF)"),
		setEolModeWindows(bool), 
		application()->editor()->eolMode() == QsciScintilla::EolWindows);
	eolSetting->addAction(act);

	checkableAction(settingsItems, tr("Unix"),
		tr("Unix EOL mode (LF)"),
		setEolModeUnix(bool), 
		application()->editor()->eolMode() == QsciScintilla::EolUnix);
	eolSetting->addAction(act);

	checkableAction(settingsItems, tr("Mac"),
		tr("Mac EOL mode (CR)"),
		setEolModeMac(bool), 
		application()->editor()->eolMode() == QsciScintilla::EolMac);
	eolSetting->addAction(act);
	
	// Help actions
	aboutAct = new QAction(QtIconLoader::icon("help-about"), "&About", application());
	aboutAct->setStatusTip(tr("Information about %1").arg(APPLICATION_NAME)); 
	connect(aboutAct, SIGNAL(triggered()), application(), SLOT(about()));

	aboutQtAct = new QAction(QtIconLoader::icon("help-about"), "About Qt", application());
	aboutQtAct->setStatusTip(tr("Information about Qt")); 
	connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

QsciApp *Actions::application()
{
	return static_cast<QsciApp*>(parent());
}

void Actions::setupMenus()
{
	QList<QAction*>::iterator iter;

	// File menu
	fileMenu = application()->menuBar()->addMenu(tr("&File"));
	for (iter = fileItems.begin(); iter != fileItems.end(); iter++)
		fileMenu->addAction(*iter);

	// Edit menu
	editMenu = application()->menuBar()->addMenu(tr("&Edit"));
	for (iter = editItems.begin(); iter != editItems.end(); iter++)
		editMenu->addAction(*iter);

	for (iter = editItemsMisc.begin(); iter != editItemsMisc.end(); iter++)
		editMenu->addAction(*iter);

	// View menu
	viewMenu = application()->menuBar()->addMenu(tr("&View"));
	for (iter = viewItems.begin(); iter != viewItems.end(); iter++)
		viewMenu->addAction(*iter);

	// Settings menu
	settingsMenu = application()->menuBar()->addMenu(tr("&Settings"));
	for (iter = settingsItems.begin(); iter != settingsItems.end(); iter++)
		settingsMenu->addAction(*iter);

	// Help menu
	application()->menuBar()->addSeparator();
	helpMenu = application()->menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(aboutQtAct);
}

void Actions::setupToolBars()
{
	QList<QAction*>::iterator iter;

	// File toolbar
	fileToolBar = application()->addToolBar(tr("File"));
	for (iter = fileItems.begin(); iter != fileItems.end(); iter++)
		fileToolBar->addAction(*iter);

	// Edit toolbar
	editToolBar = application()->addToolBar(tr("Edit"));
	for (iter = editItems.begin(); iter != editItems.end(); iter++)
		editToolBar->addAction(*iter);
	
	// Find toolbar
	findToolBar = application()->addToolBar(tr("&Find"));
	findEntry = new QLineEdit();
	findEntry->setMaximumWidth(200);
	connect(findEntry, SIGNAL(textEdited(QString)), this, SLOT(clearSearchDir()));
	connect(findEntry, SIGNAL(returnPressed()), this, SLOT(findNext()));
	findToolBar->addWidget(findEntry);
	findToolBar->addAction(findPrevAct);
	findToolBar->addAction(findNextAct);
	findToolBar->hide();
	QAction *closeToolBarAct
	       = new QAction(QtIconLoader::icon("application-exit"),
			     tr("&Close"), this);
	closeToolBarAct->setStatusTip(tr("Close the find toolbar"));
	connect(closeToolBarAct, SIGNAL(triggered()), findToolBar, SLOT(hide()));
	findToolBar->addAction(closeToolBarAct);
}

void Actions::find()
{
	if (application()->editor()->hasSelectedText()) {
		findEntry->setText(application()->editor()->selectedText());
	}
	findToolBar->show();
	findEntry->setFocus();
}

void Actions::findText(bool forwards)
{
	QString text = findEntry->text();
	if (text.isEmpty())
		return;

	if (forwards)
		lastSearchDir = Actions::DirForwards;
	else
		lastSearchDir = Actions::DirBackwards;

	bool found = application()->editor()->findFirst(text, false, true, 
			false, false, forwards);
	if (!found)
		application()->statusBar()->showMessage(tr("No match"));
	else
		application()->statusBar()->showMessage("");
}

void Actions::findNext()
{
	if (lastSearchDir != Actions::DirForwards)
		return findText(true);

	bool found = application()->editor()->findNext();
	if (!found)
		application()->statusBar()->showMessage(tr("No match"));
	else
		application()->statusBar()->showMessage("");
}

void Actions::findPrev()
{
	if (lastSearchDir != Actions::DirBackwards)
		return findText(false);

	bool found = application()->editor()->findNext();
	if (!found)
		application()->statusBar()->showMessage(tr("No match"));
	else
		application()->statusBar()->showMessage("");
}

void Actions::clearSearchDir()
{
	lastSearchDir = Actions::DirNone;
}

