#include <QtGui>
#include <qsciscintilla.h>
#include <qscilexer.h>
#include "qsciapp.h"
#include "qticonloader.h"
#include "lexersel.h"
#include "finddialog.h"

const QString APPLICATION_NAME = "QsciEdit";
const QString COMPANY_NAME = "cpp";
const int LINE_NUM_MARGIN = 1;

QsciApp::QsciApp(const QString fileName)
{
	textEdit = new QsciScintilla;
	findDialog = NULL;

	// Order here is important
	setCurrentFile("");
	if (!fileName.isEmpty())
		loadFile(fileName);
	loadSettings();
	createActions();
	createMenus();
	createToolBars();
	createStatusBar();
	setAcceptDrops(true);
	setCentralWidget(textEdit);

	// The QsciScintilla drag & drop behaviour is to put the URL in as text
	// in the current document. We want it to open the file url so disable
	// the QsciScintilla behaviour.
	textEdit->setAcceptDrops(false);

	connect(textEdit, SIGNAL(modificationChanged(bool)),
		this, SLOT(documentModified(bool)));
}

void QsciApp::closeEvent(QCloseEvent *event)
{
	if (saveIfModified()) {
		saveSettings();
		event->accept();
	} else {
		event->ignore();
	}
}

void QsciApp::dragEnterEvent(QDragEnterEvent *event)
{
	event->acceptProposedAction();
}

void QsciApp::dragMoveEvent(QDragMoveEvent *event)
{
	event->acceptProposedAction();
}

void QsciApp::dragLeaveEvent(QDragLeaveEvent *event)
{
	event->accept();
}

void QsciApp::dropEvent(QDropEvent *event)
{
	const QMimeData *mimeData = event->mimeData();
	if (mimeData->hasUrls()) {
		QList<QUrl> urlList = mimeData->urls();

		// Until multi document support is added we could either reject
		// multiple URLS or only handle the first URL in a list. We've
		// opted for the latter.
		QString url = urlList.at(0).path();
		if (!url.isEmpty() && saveIfModified()) {
			loadFile(url);
		}
		event->acceptProposedAction();
	}
}

void QsciApp::documentModified(bool modified)
{
	setWindowModified(modified);
}

void QsciApp::setLineNumbers(bool enable)
{
	textEdit->setMarginLineNumbers(LINE_NUM_MARGIN, enable);
	if (enable) {
		textEdit->setMarginWidth(LINE_NUM_MARGIN, 50);
	} else {
		textEdit->setMarginWidth(LINE_NUM_MARGIN, 0);
	}
}
void QsciApp::setWhiteSpaceVis(bool enable)
{
	if (enable) {
		textEdit->setWhitespaceVisibility(QsciScintilla::WsVisible);
	} else {
		textEdit->setWhitespaceVisibility(QsciScintilla::WsInvisible);
	}
}

void QsciApp::setFolding(bool enable)
{
	if (enable) {
		textEdit->setFolding(QsciScintilla::PlainFoldStyle);
	} else {
		textEdit->setFolding(QsciScintilla::NoFoldStyle);
	}
}

void QsciApp::setAutoCompletion(bool enable)
{
	if (enable) {
		textEdit->setAutoCompletionSource(QsciScintilla::AcsAll);
		textEdit->setAutoCompletionThreshold(3);
	} else {
		textEdit->setAutoCompletionSource(QsciScintilla::AcsNone);
	}
}

void  QsciApp::setBraceMatching(bool enable)
{
	if (enable) {
		textEdit->setBraceMatching(QsciScintilla::SloppyBraceMatch);
		textEdit->setMatchedBraceForegroundColor(QColor("white"));
		textEdit->setMatchedBraceBackgroundColor(QColor("dodgerblue"));
		textEdit->setUnmatchedBraceForegroundColor(QColor("black"));
		textEdit->setUnmatchedBraceBackgroundColor(QColor("red"));
	} else {
		textEdit->setBraceMatching(QsciScintilla::NoBraceMatch);
	}
}

void QsciApp::askForLine()
{
	int line, index;
	textEdit->getCursorPosition(&line, &index);
	line++;
	QInputDialog dlg(this);
	dlg.setWindowTitle(tr("Goto"));
	dlg.setLabelText(tr("Line Number"));
	dlg.setInputMode(QInputDialog::IntInput);
	dlg.setIntMinimum(1);
	dlg.setIntMaximum(textEdit->lines());
	dlg.setIntValue(line);
	if (dlg.exec() == QDialog::Accepted) {
		statusBar()->showMessage(tr("goto %1").arg(dlg.intValue()));
		gotoLine(dlg.intValue());
	}
}

void QsciApp::createActions()
{
#define new_action(act, shortstr, longstr, keyseq, slot, icon) \
	act = new QAction(icon, shortstr, this); \
	act->setShortcuts(keyseq); \
	act->setStatusTip(longstr); \
	connect(act, SIGNAL(triggered()), this, SLOT(slot))

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

	new_action(pasteAct, tr("&Paste"),
			tr("Paste text from the clipboard"),
			QKeySequence::Paste,paste(),
			QtIconLoader::icon("edit-paste"));

	gotoLineAct = new QAction(tr("&Goto Line"), this);
	gotoLineAct->setStatusTip(tr("Jump to a line number"));
	gotoLineAct->setShortcut(tr("Ctrl+G"));

	connect(gotoLineAct, SIGNAL(triggered()), this, SLOT(askForLine()));

	findAct = new QAction(tr("&Find..."), this);
	findAct->setStatusTip(tr("Search for text"));
	findAct->setShortcuts(QKeySequence::Find);
	connect(findAct, SIGNAL(triggered()), this, SLOT(find()));

	findNextAct = new QAction(tr("Find &Next"), this);
	findNextAct->setStatusTip(tr("Repeat the last search"));
	findNextAct->setShortcuts(QKeySequence::FindNext);
	connect(findNextAct, SIGNAL(triggered()), this, SLOT(findNext()));
	findNextAct->setEnabled(false);

	connect(textEdit, SIGNAL(copyAvailable(bool)),
		cutAct, SLOT(setEnabled(bool)));
	connect(textEdit, SIGNAL(copyAvailable(bool)),
		copyAct, SLOT(setEnabled(bool)));

#define checkable_act(act, shortstr, longstr, slot, enable) \
	act = new QAction(shortstr, this); \
	act->setStatusTip(longstr); \
	act->setCheckable(true); \
	act->setChecked(enable); \
	connect(act, SIGNAL(triggered(bool)), this, SLOT(slot))

	// View
	checkable_act(lineNumAct, tr("Line Numbers"),
		tr("Display line numbers in the margin"),
		setLineNumbers(bool),
		textEdit->marginLineNumbers(LINE_NUM_MARGIN));
	checkable_act(whiteSpaceAct, tr("White space"),
		tr("Make white space visible"),
		setWhiteSpaceVis(bool),
		textEdit->whitespaceVisibility() != QsciScintilla::WsInvisible);
	
	// Settings
	checkable_act(foldAct, tr("Folding"), 
		tr("Enable/disable code folding"),
		setFolding(bool),
		textEdit->folding() != QsciScintilla::NoFoldStyle);
	checkable_act(autoCompAct, tr("Auto completion"),
		tr("Enable/disable auto completion"),
		setAutoCompletion(bool),
		textEdit->autoCompletionSource() != QsciScintilla::AcsNone);
	checkable_act(braceMatchAct, tr("Brace Matching"),
		tr("Enable/disable brace matching"),
		setBraceMatching(bool), 
		textEdit->braceMatching() != QsciScintilla::NoBraceMatch);
	
	// Help actions
	aboutAct = new QAction(QtIconLoader::icon("help-about"), "&About", this);
	aboutAct->setStatusTip(tr("Information about %1").arg(APPLICATION_NAME)); 
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	aboutQtAct = new QAction(QtIconLoader::icon("help-about"), "About Qt", this);
	aboutQtAct->setStatusTip(tr("Information about Qt")); 
	connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void QsciApp::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newAct);
	fileMenu->addAction(openAct);
	fileMenu->addAction(saveAct);
	fileMenu->addAction(saveAsAct);

	editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(undoAct);
	editMenu->addAction(redoAct);
	editMenu->addSeparator();
	editMenu->addAction(cutAct);
	editMenu->addAction(copyAct);
	editMenu->addAction(pasteAct);
	editMenu->addSeparator();
	editMenu->addAction(findAct);
	editMenu->addAction(findNextAct);
	editMenu->addAction(gotoLineAct);

	viewMenu = menuBar()->addMenu(tr("&View"));
	viewMenu->addAction(lineNumAct);
	viewMenu->addAction(whiteSpaceAct);

	settingsMenu = menuBar()->addMenu(tr("&Settings"));
	settingsMenu->addAction(foldAct);
	settingsMenu->addAction(autoCompAct);
	settingsMenu->addAction(braceMatchAct);

	menuBar()->addSeparator();
	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(aboutQtAct);
}

void QsciApp::createToolBars()
{
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(newAct);
	fileToolBar->addAction(openAct);
	fileToolBar->addAction(saveAct);
	fileToolBar->addAction(saveAsAct);
	fileToolBar->addSeparator();

	editToolBar = addToolBar(tr("Edit"));
	editToolBar->addAction(undoAct);
	editToolBar->addAction(redoAct);
	editToolBar->addSeparator();
	editToolBar->addAction(cutAct);
	editToolBar->addAction(copyAct);
	editToolBar->addAction(pasteAct);
}

void QsciApp::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}

void QsciApp::newFile()
{
	if (saveIfModified()) {
		textEdit->setText("");
		setCurrentFile("");
	}
}

void QsciApp::open()
{
	if (saveIfModified()) {
		QString fileName = QFileDialog::getOpenFileName(this);
		if (!fileName.isEmpty()) {
			loadFile(fileName);
		}
	}
}

bool QsciApp::save()
{
	if (curFile.isEmpty()) {
		return saveAs();
	} else {
		return saveFile(curFile);
	}
}

bool QsciApp::saveAs()
{
	QString fileName = QFileDialog::getSaveFileName(this);
	if (fileName.isEmpty())
		return false;

	return saveFile(fileName);
}

void QsciApp::undo() { textEdit->undo(); };
void QsciApp::redo() { textEdit->redo(); };
void QsciApp::cut() { textEdit->cut(); };
void QsciApp::copy() { textEdit->copy(); };
void QsciApp::paste() { textEdit->paste(); };

void QsciApp::about()
{
	QFile file(":/about.html");
	if(file.open(QFile::ReadOnly | QFile::Text)) {
		QTextStream in(&file);
		QMessageBox::about(this, tr("About %1").arg(APPLICATION_NAME), in.readAll());
	}
}

void QsciApp::gotoLine(int line)
{
	textEdit->setCursorPosition(line-1, 0);
	textEdit->ensureCursorVisible ();
	textEdit->ensureLineVisible (line+9);
}

void QsciApp::find()
{
	if (!findDialog) {
		findDialog = new FindDialog(this);
		connect(findDialog,
			SIGNAL(findText(const QString, bool, bool, bool, bool, bool)), this,
			SLOT(findText(const QString, bool, bool, bool, bool, bool)));
	}
	if (textEdit->hasSelectedText()) {
		findDialog->setSearchText(textEdit->selectedText());
	}
	findDialog->raise();
	findDialog->show();
	findDialog->activateWindow();
}

void QsciApp::findNext()
{
	bool found = textEdit->findNext();
	if (!found)
		statusBar()->showMessage(tr("No match"));
	else
		statusBar()->showMessage("");
}

void QsciApp::findText(const QString text, bool regex, bool caseSensitive,
		bool wholeWord, bool wrap, bool backwards)
{
	bool found = textEdit->findFirst(text, regex, caseSensitive, 
			wholeWord, wrap, !backwards);
	if (!found)
		statusBar()->showMessage(tr("No match"));
	else
		statusBar()->showMessage("");

	findNextAct->setEnabled(true);
}

bool QsciApp::saveIfModified()
{
	if (textEdit->isModified()) {
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, APPLICATION_NAME,
				tr("This document has unsaved changes.\n"
				   "Do you want to save the changes now?"),
				QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		if (ret == QMessageBox::Save)
			return save();
		else if (ret == QMessageBox::Cancel)
			return false;
	}
	return true;
}

void QsciApp::loadFile(const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("Application"),
			tr("Cannot read file %1:\n%2.")
			.arg(fileName)
			.arg(file.errorString()));
		return;
	}

	QTextStream in(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	textEdit->setText(in.readAll());
	QApplication::restoreOverrideCursor();

	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File %1 loaded").arg(fileName));
}

bool QsciApp::saveFile(const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::WriteOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("Application"),
			tr("Cannot write file %1:\n%2.")
			.arg(fileName)
			.arg(file.errorString()));
		return false;
	}

	QTextStream out(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	out << textEdit->text();
	QApplication::restoreOverrideCursor();

	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File %1 saved").arg(fileName));
	return true;
}

void QsciApp::setCurrentFile(const QString &fileName)
{
	curFile = fileName;
	textEdit->setModified(false);
	setWindowModified(false);

	QString shownName;
	if (curFile.isEmpty())
		shownName = "untitled";
	else
		shownName = QFileInfo(curFile).fileName();

	setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(APPLICATION_NAME));
	textEdit->setLexer(LexerSelector::getLexerForFile(fileName));
	if (textEdit->lexer()) {
		QFont font = QFont("Monospaced, Courier", 10);
		textEdit->lexer()->setDefaultFont(font);
		textEdit->lexer()->setFont(font);
	}
}

void QsciApp::loadSettings()
{
	QSettings settings(COMPANY_NAME, APPLICATION_NAME);
	QPoint pos = settings.value("pos", QPoint(0,0)).toPoint();
	QSize size = settings.value("size", QSize(600,700)).toSize();
	QString file = settings.value("file", "").toString();
	settings.beginGroup("editor settings");
	bool linenumbers = settings.value("line numbers",false).toBool();
	bool whitespace = settings.value("whitespace", false).toBool();
	bool folding = settings.value("folding", false).toBool();
	bool autocomplete = settings.value("auto completion", false).toBool();
	bool bracematch = settings.value("brace matching", false).toBool();
	settings.endGroup();

	move (pos);
	resize(size);
	if (curFile.isEmpty() && !file.isEmpty())
		loadFile(file);
	setLineNumbers(linenumbers);
	setWhiteSpaceVis(whitespace);
	setFolding(folding);
	setAutoCompletion(autocomplete);
	setBraceMatching(bracematch);
}

void QsciApp::saveSettings()
{
	QSettings settings(COMPANY_NAME, APPLICATION_NAME);
	settings.setValue("pos", pos());
	settings.setValue("size", size());
	settings.setValue("file", curFile);
	settings.beginGroup("editor settings");
	settings.setValue("line numbers", textEdit->marginLineNumbers(LINE_NUM_MARGIN));
	settings.setValue("whitespace", textEdit->whitespaceVisibility() != QsciScintilla::WsInvisible);
	settings.setValue("folding", textEdit->folding() != QsciScintilla::NoFoldStyle);
	settings.setValue("brace matching", textEdit->braceMatching() != QsciScintilla::NoBraceMatch);
	settings.setValue("auto completion", textEdit->autoCompletionSource() != QsciScintilla::AcsNone);
	settings.endGroup();
}
