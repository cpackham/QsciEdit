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
#define new_action(act, shortstr, longstr, keyseq, slot, icon) \
	act = new QAction(icon, shortstr, application()); \
	act->setShortcuts(keyseq); \
	act->setStatusTip(longstr); \
	connect(act, SIGNAL(triggered()), application(), SLOT(slot))

	// File actions
	new_action(newAct,tr("&New"),
			tr("Create a new file"), 
			QKeySequence::New,newFile(),
			QtIconLoader::icon("document-new"));
	new_action(openAct, tr("&Open"),
			tr("Open an existing file"),
			QKeySequence::Open,open(),
			QtIconLoader::icon("document-open"));
	new_action(saveAct,tr("&Save"),
			tr("Save the current file"),
			QKeySequence::Save,save(),
			QtIconLoader::icon("document-save"));
	new_action(saveAsAct,tr("Save &As"),
			tr("Save the current file with a new name"),
			QKeySequence::SaveAs,saveAs(),
			QtIconLoader::icon("document-save-as"));

	// Edit actions
	new_action(undoAct, tr("&Undo"),
			tr("Undo last edit"),
			QKeySequence::Undo,undo(),
			QtIconLoader::icon("edit-undo"));
	new_action(redoAct, tr("&Redo"),
			tr("Redo last edit"),
			QKeySequence::Redo,redo(),
			QtIconLoader::icon("edit-redo"));
	new_action(cutAct, tr("Cu&t"),
			tr("Cut the selected text"),
			QKeySequence::Cut,cut(),
			QtIconLoader::icon("edit-cut"));
	cutAct->setEnabled(false);

	new_action(copyAct, tr("&Copy"),
			tr("Copy the selected text to the clipboard"),
			QKeySequence::Copy,copy(),
			QtIconLoader::icon("edit-copy"));
	copyAct->setEnabled(false);
	connect(application()->editor(), SIGNAL(copyAvailable(bool)),
		cutAct, SLOT(setEnabled(bool)));
	connect(application()->editor(), SIGNAL(copyAvailable(bool)),
		copyAct, SLOT(setEnabled(bool)));

	new_action(pasteAct, tr("&Paste"),
			tr("Paste text from the clipboard"),
			QKeySequence::Paste,paste(),
			QtIconLoader::icon("edit-paste"));

	gotoLineAct = new QAction(tr("&Goto Line"), application());
	gotoLineAct->setStatusTip(tr("Jump to a line number"));
	gotoLineAct->setShortcut(tr("Ctrl+G"));
	connect(gotoLineAct, SIGNAL(triggered()), application(), SLOT(askForLine()));

	gotoBraceAct = new QAction(tr("Goto matching brace"), application());
	gotoBraceAct->setStatusTip(tr("Jump to the matching brace"));
	gotoBraceAct->setShortcut(tr("Ctrl+E"));
	connect(gotoBraceAct, SIGNAL(triggered()), application()->editor(), SLOT(moveToMatchingBrace()));

	selectBraceAct = new QAction(tr("Select matching brace"), application());
	selectBraceAct->setStatusTip(tr("Select to matching brace"));
	selectBraceAct->setShortcut(tr("Ctrl+Shift+E"));
	connect(selectBraceAct, SIGNAL(triggered()), application()->editor(), SLOT(selectToMatchingBrace()));

	findAct = new QAction(QtIconLoader::icon("edit-find"), tr("&Find..."), application());
	findAct->setStatusTip(tr("Search for text"));
	findAct->setShortcuts(QKeySequence::Find);
	connect(findAct, SIGNAL(triggered()), this, SLOT(find()));

	findPrevAct = new QAction(QtIconLoader::icon("go-previous"), tr("Find &Prev"), application());
	findPrevAct->setStatusTip(tr("Repeat the last search"));
	findPrevAct->setShortcuts(QKeySequence::FindPrevious);
	connect(findPrevAct, SIGNAL(triggered()), this, SLOT(findPrev()));
	// findPrevAct->setEnabled(false);

	findNextAct = new QAction(QtIconLoader::icon("go-next"),tr("Find &Next"), application());
	findNextAct->setStatusTip(tr("Repeat the last search"));
	findNextAct->setShortcuts(QKeySequence::FindNext);
	connect(findNextAct, SIGNAL(triggered()), this, SLOT(findNext()));
	// findNextAct->setEnabled(false);

	lineCommentAct = new QAction(tr("Line Comment"), application());
	lineCommentAct->setStatusTip(tr("Comment out lines"));
	lineCommentAct->setShortcut(tr("Ctrl+D"));
	connect(lineCommentAct, SIGNAL(triggered()), application(), SLOT(lineComment()));

	blockCommentAct = new QAction(tr("Block Comment"), application());
	blockCommentAct->setStatusTip(tr("Comment out the selected block of text"));
	blockCommentAct->setShortcut(tr("Ctrl+Shift+D"));
	connect(blockCommentAct, SIGNAL(triggered()), application(), SLOT(blockComment()));

	suggestCompletion = new QAction(tr("Suggest Completion"), application());
	suggestCompletion->setStatusTip(tr("Suggest a completion based on the current text"));
	suggestCompletion->setShortcut(tr("Ctrl+Space"));
	connect(suggestCompletion, SIGNAL(triggered()), application()->editor(), SLOT(autoCompleteFromAll()));

#define checkable_act(act, shortstr, longstr, slot, enable) \
	act = new QAction(shortstr, application()); \
	act->setStatusTip(longstr); \
	act->setCheckable(true); \
	act->setChecked(enable); \
	connect(act, SIGNAL(triggered(bool)), application(), SLOT(slot))

	// View
	checkable_act(lineNumAct, tr("Line Numbers"),
		tr("Display line numbers in the margin"),
		setLineNumbers(bool),
		application()->editorSettings->displayLineNumbers());
	checkable_act(whiteSpaceAct, tr("White space"),
		tr("Make white space visible"),
		setWhiteSpaceVis(bool),
		application()->editorSettings->displayWhitespace());
	checkable_act(wrapTextAct, tr("Wrap Text"),
		tr("Wrap text"),
		setWrapText(bool),
		application()->editorSettings->displayWrapText());
	
	// Settings
	checkable_act(foldAct, tr("Folding"), 
		tr("Enable/disable code folding"),
		setFolding(bool),
		application()->editorSettings->displayCodeFolding());
	checkable_act(autoCompAct, tr("Auto completion"),
		tr("Suggest completions for the current text"),
		setAutoCompletion(bool),
		application()->editorSettings->displayAutoComplete());
	checkable_act(braceMatchAct, tr("Brace Matching"),
		tr("Highlight matching pairs of braces"),
		setBraceMatching(bool), 
		application()->editorSettings->displayBraceMatch());
	
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
	fileMenu = application()->menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newAct);
	fileMenu->addAction(openAct);
	fileMenu->addAction(saveAct);
	fileMenu->addAction(saveAsAct);

	editMenu = application()->menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(undoAct);
	editMenu->addAction(redoAct);
	editMenu->addSeparator();
	editMenu->addAction(cutAct);
	editMenu->addAction(copyAct);
	editMenu->addAction(pasteAct);
	editMenu->addSeparator();
	editMenu->addAction(findAct);
	editMenu->addAction(findNextAct);
	editMenu->addAction(findPrevAct);
	editMenu->addAction(gotoLineAct);
	editMenu->addAction(gotoBraceAct);
	editMenu->addAction(selectBraceAct);
	editMenu->addAction(lineCommentAct);
	editMenu->addAction(blockCommentAct);
	editMenu->addAction(suggestCompletion);

	viewMenu = application()->menuBar()->addMenu(tr("&View"));
	viewMenu->addAction(lineNumAct);
	viewMenu->addAction(whiteSpaceAct);
	viewMenu->addAction(wrapTextAct);

	settingsMenu = application()->menuBar()->addMenu(tr("&Settings"));
	settingsMenu->addAction(foldAct);
	settingsMenu->addAction(autoCompAct);
	settingsMenu->addAction(braceMatchAct);

	application()->menuBar()->addSeparator();
	helpMenu = application()->menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(aboutQtAct);
}

void Actions::setupToolBars()
{
	fileToolBar = application()->addToolBar(tr("File"));
	fileToolBar->addAction(newAct);
	fileToolBar->addAction(openAct);
	fileToolBar->addAction(saveAct);
	fileToolBar->addAction(saveAsAct);
	fileToolBar->addSeparator();

	editToolBar = application()->addToolBar(tr("Edit"));
	editToolBar->addAction(undoAct);
	editToolBar->addAction(redoAct);
	editToolBar->addSeparator();
	editToolBar->addAction(cutAct);
	editToolBar->addAction(copyAct);
	editToolBar->addAction(pasteAct);

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

