#include <QtGui>
#include <qsciscintilla.h>
#include "actions.h"
#include "globals.h"
#include "qticonloader.h"
#include "qsciapp.h"
#include "editorsettings.h"
#include "qeaction.h"

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
	QEAction *act;

	// File actions
	act = QEAction::newAction(QtIconLoader::icon("document-new"),
			tr("&New"),
			application(),
			tr("Create a new file"),
			QKeySequence::New);
	connect(act, SIGNAL(triggered()), application(), SLOT(newFile()));
	fileItems << act;

	act = QEAction::newAction(QtIconLoader::icon("document-open"),
			tr("&Open"),
			application(),
			tr("Open an existing file"),
			QKeySequence::Open);
	connect(act, SIGNAL(triggered()), application(), SLOT(open()));
	fileItems << act;

	act = QEAction::newAction(QtIconLoader::icon("view-refresh"),
			tr("&Reload"), 
			application(), 
			tr("Reload the current file"), 
			QKeySequence::Refresh);
	act->showInToolbar = false;
	connect(act, SIGNAL(triggered()), application(), SLOT(reload()));
	fileItems << act;

	act = QEAction::newAction(QtIconLoader::icon("document-save"),
			tr("&Save"),
			application(),
			tr("Save the current file"),
			QKeySequence::Save);
	connect(act, SIGNAL(triggered()), application(), SLOT(save()));
	fileItems << act;

	act = QEAction::newAction(QtIconLoader::icon("document-save-as"),
			tr("Save &As"),
			application(),
			tr("Save the current file with a new name"),
			QKeySequence::SaveAs);
	connect(act, SIGNAL(triggered()), application(), SLOT(saveAs()));
	fileItems << act;

	act = new QEAction(application());
	act->setSeparator(true);
	fileItems << act;

	act = QEAction::newAction(QtIconLoader::icon("application-exit"),
			tr("&Quit"),
			application(),
			tr("Exit %1").arg(APPLICATION_NAME));
	act->showInToolbar = false;
	connect(act, SIGNAL(triggered()), application(), SLOT(close()));
	fileItems << act;

	// Edit actions
	act = QEAction::newAction(QtIconLoader::icon("edit-undo"),
			tr("&Undo"),
			application(),
			tr("Undo last edit"),
			QKeySequence::Undo);
	connect(act, SIGNAL(triggered()), application(), SLOT(undo()));
	editItems << act;

	act = QEAction::newAction(QtIconLoader::icon("edit-redo"),
			tr("&Redo"),
			application(),
			tr("Redo last edit"),
			QKeySequence::Redo);
	connect(act, SIGNAL(triggered()), application(), SLOT(redo()));
	editItems << act;
			
	act = new QEAction(application());
	act->setSeparator(true);
	editItems << act;

	act = QEAction::newAction(QtIconLoader::icon("edit-cut"),
			tr("Cu&t"),
			application(),
			tr("Cut the selected text"),
			QKeySequence::Cut);
	connect(act, SIGNAL(triggered()), application(), SLOT(cut()));
	editItems << act;
	act->setEnabled(false);
	connect(application()->editor(), SIGNAL(copyAvailable(bool)),
		act, SLOT(setEnabled(bool)));

	act = QEAction::newAction(QtIconLoader::icon("edit-copy"),
			tr("&Copy"),
			application(),
			tr("Copy the selected text to the clipboard"),
			QKeySequence::Copy);
	connect(act, SIGNAL(triggered()), application(), SLOT(copy()));
	editItems << act;
	act->setEnabled(false);
	connect(application()->editor(), SIGNAL(copyAvailable(bool)),
		act, SLOT(setEnabled(bool)));

	act = QEAction::newAction(QtIconLoader::icon("edit-paste"),
			tr("&Paste"),
			application(),
			tr("Paste text from the clipboard"),
			QKeySequence::Paste);
	connect(act, SIGNAL(triggered()), application(), SLOT(paste()));
	editItems << act;

	act = new QEAction(application());
	act->setSeparator(true);
	editItemsMisc << act;

	findAct = QEAction::newAction(QtIconLoader::icon("edit-find"), 
			tr("&Find..."), 
			application(),
			tr("Simple text search"),
			QKeySequence::Find);
	connect(findAct, SIGNAL(triggered()), this, SLOT(find()));
	editItemsMisc << findAct;

	findPrevAct = QEAction::newAction(QtIconLoader::icon("go-previous"), 
			tr("Find &Prev"),
			application(),
			tr("Repeat the last search backwards"),
			QKeySequence::FindPrevious);
	connect(findPrevAct, SIGNAL(triggered()), this, SLOT(findPrev()));
	editItemsMisc << findPrevAct;

	findNextAct = QEAction::newAction(QtIconLoader::icon("go-next"),
			tr("Find &Next"),
			application(),
			tr("Repeat the last search forwards"),
			QKeySequence::FindNext);
	connect(findNextAct, SIGNAL(triggered()), this, SLOT(findNext()));
	editItemsMisc << findNextAct;

	act = new QEAction(application());
	act->setSeparator(true);
	editItemsMisc << act;

	act = QEAction::newAction(QtIconLoader::icon("edit-find"), 
			tr("&Search..."), 
			application(),
			tr("Advanced text search"),
			tr("Ctrl+Shift+F"));
	connect(act, SIGNAL(triggered()), application(), SLOT(search()));
	act->showInToolbar = false;
	editItemsMisc << act;

	act = QEAction::newAction(tr("&Goto Line"), 
			application(),
			tr("Jump to a line number"),
			tr("Ctrl+G"));
	connect(act, SIGNAL(triggered()), application(), SLOT(askForLine()));
	editItemsMisc << act;

	act = QEAction::newAction(tr("Goto matching brace"), 
			application(),
			tr("Jump to the matching brace"),
			tr("Ctrl+E"));
	connect(act, SIGNAL(triggered()), application()->editor(), SLOT(moveToMatchingBrace()));
	editItemsMisc << act;

	act = QEAction::newAction(tr("Select matching brace"), 
			application(),
			tr("Select to matching brace"),
			tr("Ctrl+Shift+E"));
	connect(act, SIGNAL(triggered()), application()->editor(), SLOT(selectToMatchingBrace()));
	editItemsMisc << act;

	act = QEAction::newAction(tr("Line Comment"),
			application(),
			tr("Comment out lines"), 
			tr("Ctrl+D"));
	connect(act, SIGNAL(triggered()), application(), SLOT(lineComment()));
	editItemsMisc << act;

	act = QEAction::newAction(tr("Block Comment"),
			application(),
			tr("Comment out the selected block of text"), 
			tr("Ctrl+Shift+D"));
	connect(act, SIGNAL(triggered()), application(), SLOT(blockComment()));
	editItemsMisc << act;

	act = QEAction::newAction(tr("Suggest Completion"),
			application(),
			tr("Suggest a completion based on the current text"),
			tr("Ctrl+Space"));
	connect(act, SIGNAL(triggered()), application()->editor(), SLOT(autoCompleteFromAll()));
	editItemsMisc << act;

	// View
	act = QEAction::newAction(QtIconLoader::icon("zoom-original"),
			tr("Reset Zoom"),
			application(),
			tr("Reset Zoom"),
			tr("Ctrl+1"));

	QSignalMapper *signalMapper = new QSignalMapper(this);
	signalMapper->setMapping(act, 0);
	connect(act, SIGNAL(triggered()), signalMapper, SLOT(map()));
	connect(signalMapper, SIGNAL(mapped(int)), application()->editor(), SLOT(zoomTo(int)));

	viewItems << act;

	act = QEAction::newAction(QtIconLoader::icon("zoom-in"),
			tr("Zoom In"),
			application(),
			tr("Zoom in"),
			tr("Ctrl++"));
	connect(act, SIGNAL(triggered()), application()->editor(), SLOT(zoomIn()));
	viewItems << act;

	act = QEAction::newAction(QtIconLoader::icon("zoom-out"),
			tr("Zoom Out"),
			application(),
			tr("Zoom Out"),
			tr("Ctrl+-"));
	connect(act, SIGNAL(triggered()), application()->editor(), SLOT(zoomOut()));
	viewItems << act;

	QEAction *foldAllAct = QEAction::newAction(tr("Toggle All Code Folds"),
				application(),
				tr("Folds or unfolds all lines"));
	foldAllAct->showInToolbar = false;
	connect(foldAllAct, SIGNAL(triggered()), 
		application()->editor(), SLOT(foldAll()));
	foldAllAct->setEnabled(application()->editorSettings->displayCodeFolding());
	viewItems << foldAllAct;
	
	// Settings
	act = QEAction::newAction(tr("Line Numbers"),
			application(),
			tr("Display line numbers in the margin"),
			0, true,
			application()->editorSettings->displayLineNumbers());
	connect(act, SIGNAL(triggered(bool)), application(), SLOT(setLineNumbers(bool)));
	settingsItems << act;
		
	act = QEAction::newAction(tr("White space"),
			application(),
			tr("Make white space visible"),
			0, true,
			application()->editorSettings->displayWhitespace());
	connect(act, SIGNAL(triggered(bool)), application(), SLOT(setWhiteSpaceVis(bool)));
	settingsItems << act;

	act = QEAction::newAction(tr("Wrap Text"),
			application(),
			tr("Wrap text"),
			0, true,
			application()->editorSettings->displayWrapText());
	connect(act, SIGNAL(triggered(bool)), application(), SLOT(setWrapText(bool)));
	settingsItems << act;

	act = QEAction::newAction(tr("Highlight Current Line"),
			application(),
			tr("Highlight Current Line"),
			0, true,
			application()->editorSettings->highlightCurrentLine());
	connect(act, SIGNAL(triggered(bool)), application(), SLOT(setHighlightCurrentLine(bool)));
	settingsItems << act;

	act = QEAction::newAction(tr("Indicate Edge Column"),
			application(),
			tr("Display a line marking the edge column"),
			0, true,
			application()->editorSettings->displayEdge());
	connect(act, SIGNAL(triggered(bool)), application(), SLOT(setDisplayEdge(bool)));
	settingsItems << act;

	act = new QEAction(application());
	act->setSeparator(true);
	settingsItems << act;

	act = QEAction::newAction(tr("Code Folding"),
			application(),
			tr("Enable/disable code folding"),
			0, true,
			application()->editorSettings->displayCodeFolding());
	connect(act, SIGNAL(triggered(bool)), application(), SLOT(setFolding(bool)));
	settingsItems << act;
	connect(act, SIGNAL(triggered(bool)), foldAllAct, SLOT(setEnabled(bool)));

	act = QEAction::newAction(tr("Auto completion"),
			application(),
			tr("Suggest completions for the current text"),
			0, true,
			application()->editorSettings->displayAutoComplete());
	connect(act, SIGNAL(triggered(bool)), application(), SLOT(setAutoCompletion(bool)));
	settingsItems << act;

	act = QEAction::newAction(tr("Brace Matching"),
			application(),
			tr("Highlight matching pairs of braces"),
			0, true,
			application()->editorSettings->displayBraceMatch());
	connect(act, SIGNAL(triggered(bool)), application(), SLOT(setBraceMatching(bool)));
	settingsItems << act;

	act = QEAction::newAction(tr("Auto Indent"),
			application(),
			tr("Automatically indent"),
			0, true,
			application()->editorSettings->autoIndent());
	connect(act, SIGNAL(triggered(bool)), application(), SLOT(setAutoIndent(bool)));
	settingsItems << act;
	
	act = new QEAction(application());
	act->setSeparator(true);
	settingsItems << act;
	
	eolSetting = new QActionGroup(application());
	act = QEAction::newAction(tr("Windows"),
			application(),
			tr("Windows EOL mode (CRLF)"),
			0, true,
			application()->editor()->eolMode() == QsciScintilla::EolWindows);
	connect(act, SIGNAL(triggered(bool)), application(), SLOT(setEolModeWindows(bool)));
	settingsItems << act;
	eolSetting->addAction(act);

	act = QEAction::newAction(tr("Unix"),
			application(),
			tr("Unix EOL mode (LF)"),
			0, true,
			application()->editor()->eolMode() == QsciScintilla::EolUnix);
	connect(act, SIGNAL(triggered(bool)), application(), SLOT(setEolModeUnix(bool)));
	settingsItems << act;
	eolSetting->addAction(act);

	act = QEAction::newAction(("Mac"),
			application(),
			tr("Mac EOL mode (CR)"),
			0, true,
			application()->editor()->eolMode() == QsciScintilla::EolMac);
	connect(act, SIGNAL(triggered(bool)), application(), SLOT(setEolModeMac(bool)));
	settingsItems << act;
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
	QList<QEAction*>::iterator iter;

	// File menu
	fileMenu = application()->menuBar()->addMenu(tr("&File"));
	for (iter = fileItems.begin(); iter != fileItems.end(); iter++)
		if ((*iter)->showInMenu)
			fileMenu->addAction(*iter);

	// Edit menu
	editMenu = application()->menuBar()->addMenu(tr("&Edit"));
	for (iter = editItems.begin(); iter != editItems.end(); iter++)
		if ((*iter)->showInMenu)
			editMenu->addAction(*iter);

	for (iter = editItemsMisc.begin(); iter != editItemsMisc.end(); iter++)
		if ((*iter)->showInMenu)
			editMenu->addAction(*iter);

	// View menu
	viewMenu = application()->menuBar()->addMenu(tr("&View"));
	for (iter = viewItems.begin(); iter != viewItems.end(); iter++)
		if ((*iter)->showInMenu)
			viewMenu->addAction(*iter);

	// Settings menu
	settingsMenu = application()->menuBar()->addMenu(tr("&Settings"));
	for (iter = settingsItems.begin(); iter != settingsItems.end(); iter++)
		if ((*iter)->showInMenu)
			settingsMenu->addAction(*iter);

	// Help menu
	application()->menuBar()->addSeparator();
	helpMenu = application()->menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(aboutQtAct);
}

void Actions::setupToolBars()
{
	QList<QEAction*>::iterator iter;

	// File toolbar
	fileToolBar = application()->addToolBar(tr("File"));
	for (iter= fileItems.begin(); iter != fileItems.end(); iter++)
		if ((*iter)->showInToolbar)
			fileToolBar->addAction(*iter);

	// Edit toolbar
	editToolBar = application()->addToolBar(tr("Edit"));
	for (iter = editItems.begin(); iter != editItems.end(); iter++)
		if ((*iter)->showInToolbar)
			editToolBar->addAction(*iter);
	
	// View toolbar
	viewToolBar = application()->addToolBar(tr("View"));
	for (iter = viewItems.begin(); iter != viewItems.end(); iter++)
		if ((*iter)->showInToolbar)
			viewToolBar->addAction(*iter);

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

