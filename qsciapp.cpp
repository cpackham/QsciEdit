#include <cstdio>
#include <QtGui>
#include <qsciscintilla.h>
#include <qscilexer.h>
#include "qsciapp.h"
#include "qticonloader.h"
#include "lexersel.h"
#include "searchdialog.h"
#include "actions.h"
#include "globals.h"
#include "editorsettings.h"
#include "version.h"

QsciApp::QsciApp(const QString fileArg, unsigned int lineArg)
{
	QString fileName = "";
	int line = 1;
	int index = 0;

	setWindowTitle(tr("[*] - %1").arg(APPLICATION_NAME));
	textEdit = new QsciScintilla;
	textEdit->setFont(QFont("Liberation Mono", 10));
	searchDialog = NULL;
	lineCommentString = "";
	blockCommentStartString = "";
	blockCommentMiddleString = "";
	blockCommentEndString = "";
	curFileInfo = NULL;
	editorSettings = new EditorSettings;
	commandLineFile.fileName = fileArg;
	commandLineFile.line = lineArg;
	commandLineFile.index = 0;

	// Order here is important
	createStatusBar();
	

	if (!fileName.isEmpty()) {
		fileProvided = true;
		if (fileName == "-")
			loadStdIn();
		else
			loadFile(fileName);
		gotoLine(line);
	}

	loadSettings();
	applySettings();
	
	actions = new Actions(this);
	setAcceptDrops(true);
	setCentralWidget(textEdit);
	setWindowIcon(QIcon(":/qsciedit.png"));

	// The QsciScintilla drag & drop behaviour is to put the URL in as text
	// in the current document. We want it to open the file url so disable
	// the QsciScintilla behaviour.
	textEdit->setAcceptDrops(false);

	connect(textEdit, SIGNAL(modificationChanged(bool)),
		this, SLOT(documentModified(bool)));

	connect(textEdit, SIGNAL(cursorPositionChanged(int,int)),
		this, SLOT(positionChanged(int,int)));

	connect(this, SIGNAL(applicationFocusIn()),
		this, SLOT(checkModifiedOnDisk()));


	// applicationFocusIn is emitted by the event filter in this class. So
	// make sure to install the event filter.
	if (qApp)
		qApp->installEventFilter(this);

	// Load up the file
	if (!commandLineFile.fileName.isEmpty()) {
		fileName = commandLineFile.fileName;
		line = commandLineFile.line;
	} else if (!settingsFile.fileName.isEmpty()) {
		fileName = settingsFile.fileName;
		line = settingsFile.line;
		index = settingsFile.index;
	}

	if (fileName.isEmpty()) {
		setCurrentFile(fileName);
	} else if (fileName == "-") {
		loadStdIn();
	} else {
		loadFile(fileName);
	}
	
	gotoLine(line, index);
}

QString QsciApp::getLineCommentString()
{
	return lineCommentString;
}

QString QsciApp::getBlockCommentStartString()
{
	return blockCommentStartString;
}

QString QsciApp::getBlockCommentMiddleString()
{
	return blockCommentMiddleString;
}

QString QsciApp::getBlockCommentEndString()
{
	return blockCommentEndString;
}

void QsciApp::closeEvent(QCloseEvent *event)
{
	if (saveIfModified()) {
		saveSettings();
		if (searchDialog)
			delete searchDialog;
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

bool QsciApp::eventFilter(QObject *object, QEvent *event)
{
	if (qApp == object) {
		if (event->type() == QEvent::ApplicationActivate) {
			checkModifiedOnDisk();
			emit applicationFocusIn();
		}
	}
	return QMainWindow::eventFilter(object, event);
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
	editorSettings->setLineNumbers(enable);
}
void QsciApp::setWhiteSpaceVis(bool enable)
{
	if (enable) {
		textEdit->setWhitespaceVisibility(QsciScintilla::WsVisible);
	} else {
		textEdit->setWhitespaceVisibility(QsciScintilla::WsInvisible);
	}
	editorSettings->setWhitespace(enable);
}

void QsciApp::setFolding(bool enable)
{
	if (enable) {
		textEdit->setFolding(QsciScintilla::PlainFoldStyle);
	} else {
		textEdit->setFolding(QsciScintilla::NoFoldStyle);
	}
	editorSettings->setCodeFolding(enable);
}

void QsciApp::setAutoCompletion(bool enable)
{
	setAutoCompletion(enable, 3);
}

void QsciApp::setAutoCompletion(bool enable, int thresh)
{
	if (enable) {
		textEdit->setAutoCompletionSource(QsciScintilla::AcsAll);
		textEdit->setAutoCompletionThreshold(thresh);
	} else {
		textEdit->setAutoCompletionSource(QsciScintilla::AcsNone);
	}
	editorSettings->setAutoComplete(enable);
	editorSettings->setAutoCompleteThreshold(thresh);
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
	editorSettings->setBraceMatch(enable);
}

void QsciApp::setHighlightCurrentLine(bool enable)
{
	if (enable) {
		textEdit->setCaretLineVisible(true);
	} else {
		textEdit->setCaretLineVisible(false);
	}
	editorSettings->setHighlightCurrentLine(enable);
}

void QsciApp::setAutoIndent(bool enable)
{
	textEdit->setAutoIndent(enable);
	editorSettings->setAutoIndent(enable);
}

void QsciApp::setDisplayEdge(bool enable, int col)
{
	if (enable) {
		textEdit->setEdgeMode(QsciScintilla::EdgeLine);
		textEdit->setEdgeColumn(col);
	} else {
		textEdit->setEdgeMode(QsciScintilla::EdgeNone);
	}
	editorSettings->setDisplayEdge(enable);
	editorSettings->setEdgeColumn(col);
}

void QsciApp::setEolModeWindows(bool enable)
{
	if (enable) {
		setEolMode("Windows");
	}	
}

void QsciApp::setEolModeUnix(bool enable)
{
	if (enable) {
		setEolMode("Unix");
	}		
}

void QsciApp::setEolModeMac(bool enable)
{
	if (enable) {
		setEolMode("Mac");
	}	
}

void QsciApp::setEolMode(const QString mode)
{
	QMap<QString,QsciScintilla::EolMode> map;
	map["Windows"] = QsciScintilla::EolWindows;
	map["CRLF"] = QsciScintilla::EolWindows;
	map["Unix"] = QsciScintilla::EolUnix;
	map["LF"] = QsciScintilla::EolUnix;
	map["Mac"] = QsciScintilla::EolMac;
	map["CR"] = QsciScintilla::EolMac;
	
	if (map.contains(mode)) {
		textEdit->setEolMode(map[mode]);
		editorSettings->setEolMode(mode);
	}
}

void QsciApp::setIndentationsUseTabs(bool enable, int width)
{
	if (enable) {
		textEdit->setIndentationsUseTabs(enable);
		textEdit->setIndentationWidth(textEdit->tabWidth());
	} else {
		textEdit->setIndentationsUseTabs(enable);
		textEdit->setIndentationWidth(width);
	}
	editorSettings->setIndentUseTabs(enable);
	editorSettings->setIndentWidth(width);
}

void QsciApp::positionChanged(int line, int col)
{
 	markLine(line);
}

void QsciApp::search()
{
	if (!searchDialog) {
		searchDialog = new SearchDialog();
		connect (searchDialog, SIGNAL(searchText(const QString, SearchOptions*)),
			this, SLOT(searchText(const QString, SearchOptions*)));
		connect (searchDialog, SIGNAL(replaceWithText(const QString)),
			this, SLOT(replaceWithText(const QString)));
	}
	if (textEdit->hasSelectedText()) {
		searchDialog->setSearchText(textEdit->selectedText());
	}
	
	searchDialog->show();
}

void QsciApp::searchText(const QString text, SearchOptions *opts)
{
	bool found;

	if (opts->start) {
		found = textEdit->findFirst(text, opts->regex, opts->caseSensitive,
			opts->wholeWord, opts->wrap, !opts->backwards);
		opts->start = false;
	} else {
		found = textEdit->findNext();
	}

	if (!found) {
		statusBar()->showMessage(tr("No match"));
	} else {
		statusBar()->showMessage(tr(""));
	}
}

void QsciApp::replaceWithText(const QString text)
{
	textEdit->replace(text);
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

void QsciApp::lineComment()
{
	int i;
	int from, ifrom, to, ito;
	QString comment = getLineCommentString();
	if (comment.isEmpty())
		return;

	textEdit->beginUndoAction();
	textEdit->getCursorPosition(&from, &ifrom);
	to = from;
	if (textEdit->hasSelectedText()) {
		textEdit->getSelection (&from, &ifrom, &to, &ito);
	}
	for (i = from; i <= to; i++) {
		textEdit->insertAt(comment, i, 0);
	}
	textEdit->endUndoAction();
}

void QsciApp::blockComment()
{
	if (!textEdit->hasSelectedText())
		return;

	QString start = getBlockCommentStartString();
	if (start.isEmpty())
		return;

	QString middle = getBlockCommentMiddleString();
	QString end = getBlockCommentEndString();

	int from, ifrom, to, ito;
	textEdit->beginUndoAction();
	textEdit->getSelection (&from, &ifrom, &to, &ito);
	if (!end.isEmpty())
		textEdit->insertAt(end, to, ito);

	textEdit->insertAt(start, from, ifrom);

	if (!middle.isEmpty()) {
		for (int i=from+1; i < to; i++) {
			textEdit->insertAt(middle, i, 0);
		}	
	}

	textEdit->endUndoAction();
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

void QsciApp::doReload()
{
	int line, index;
	textEdit->getCursorPosition(&line, &index);
	loadFile(curFile);
	gotoLine(line+1, index);
}

void QsciApp::reload()
{
	if (!curFile.isEmpty()) {
		if(saveIfModified()) {
			doReload();
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
	QString data;

	if(file.open(QFile::ReadOnly | QFile::Text)) {
		QTextStream in(&file);
		data = in.readAll();
	}

	QMessageBox::about(this,
		tr("About %1").arg(APPLICATION_NAME),
		Version::versionText(data));
}

void QsciApp::gotoLine(int line, int index)
{
	if (line <= textEdit->lines()) {
		textEdit->setCursorPosition(line-1, index);
		textEdit->ensureCursorVisible ();
		textEdit->ensureLineVisible (line+9);
	}
}

void QsciApp::setWrapText(bool enabled)
{
	if (enabled) {
		textEdit->setWrapMode(QsciScintilla::WrapWord);
	} else {
		textEdit->setWrapMode(QsciScintilla::WrapNone);
	}
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
		setCurrentFile(fileName);
		return;
	}

	QTextStream in(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	textEdit->setText(in.readAll());
	QApplication::restoreOverrideCursor();

	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File %1 loaded").arg(fileName));
}

void QsciApp::loadStdIn()
{
	QTextStream in(stdin);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	textEdit->setText(in.readAll());
	QApplication::restoreOverrideCursor();

	setCurrentFile("");
	statusBar()->showMessage(tr("StdIn loaded"));
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

	if (curFileInfo) {
		delete curFileInfo;
		curFileInfo = NULL;
	}

	QString shownName;
	if (curFile.isEmpty()) {
		shownName = "untitled";
	} else {
		curFileInfo = new QFileInfo(curFile);
		shownName = curFileInfo->fileName();
	}

	setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(APPLICATION_NAME));

	QsciLexer *lexer = NULL;

	lexer = LexerSelector::getLexerForFile(fileName,
			&lineCommentString,
			&blockCommentStartString,
			&blockCommentMiddleString,
			&blockCommentEndString);

	// Try to determine from hash bang
	if (!lexer) {
		QString text = textEdit->text(0);
		lexer = LexerSelector::getLexerForText(text,
			&lineCommentString,
			&blockCommentStartString,
			&blockCommentMiddleString,
			&blockCommentEndString);
		
	}

	textEdit->setLexer(lexer);
	if (textEdit->lexer()) {
		QFont font = QFont("Liberation Mono", 10);
		textEdit->lexer()->setFont(font);
	}
}

void QsciApp::loadSettings()
{
	QSettings settings(COMPANY_NAME, APPLICATION_NAME);
	QPoint pos = settings.value("pos", QPoint(0,0)).toPoint();
	QSize size = settings.value("size", QSize(600,700)).toSize();
	QString file = settings.value("file", "").toString();
	int line = settings.value("CursorLine", 0).toInt();
	int index = settings.value("CursorIndex", 0).toInt();
	LexerSelector::loadLexerSettings();
	editorSettings->load();

	settingsFile.fileName = file;
	settingsFile.line     = line+1;
	settingsFile.index    = index;
	
	move (pos);
	resize(size);
}

void QsciApp::saveSettings()
{
	int line, index;
	QSettings settings(COMPANY_NAME, APPLICATION_NAME);

	textEdit->getCursorPosition(&line, &index);
	settings.setValue("pos", pos());
	settings.setValue("size", size());
	settings.setValue("file", curFileInfo ? curFileInfo->absoluteFilePath() : "");
	settings.setValue("CursorLine", line);
	settings.setValue("CursorIndex", index);
	editorSettings->save();
	LexerSelector::saveLexerSettings();
}

void QsciApp::applySettings()
{
	setLineNumbers(editorSettings->displayLineNumbers());
	setWhiteSpaceVis(editorSettings->displayWhitespace());
	setFolding(editorSettings->displayCodeFolding());
	setAutoCompletion(editorSettings->displayAutoComplete(),
			editorSettings->autoCompleteThreshold());
	setBraceMatching(editorSettings->displayBraceMatch());
	setWrapText(editorSettings->displayWrapText());
	setHighlightCurrentLine(editorSettings->highlightCurrentLine());
	setAutoIndent(editorSettings->autoIndent());
	setDisplayEdge(editorSettings->displayEdge(),
			editorSettings->edgeColumn());
	setEolMode(editorSettings->eolMode());
	setIndentationsUseTabs(editorSettings->indentUseTabs(),
			editorSettings->indentWidth());
}

void QsciApp::askReload()
{
	int  ret = QMessageBox::warning(this, "File Changed On Disk",
			tr("The file has changed on disk.\n"
			   "Do you want to reload the file now?"),
			QMessageBox::Yes | QMessageBox::No,
			QMessageBox::No);
	if (ret == QMessageBox::Yes) {
		doReload();
	} else {
		curFileInfo->refresh();
	}
}

void QsciApp::askReloadOrKeep()
{
	int  ret = QMessageBox::warning(this, "File Changed On Disk",
			tr("The file has changed on disk and there are unsaved changes.\n"
			   "Do you want to save the current changes, ignore the changes\n"
			   "on disk or discard the current changes?"),
			QMessageBox::Discard | QMessageBox::Save | QMessageBox::Ignore,
			QMessageBox::Ignore);

	switch (ret) {
	case QMessageBox::Discard:
		doReload();
		break;
	case QMessageBox::Save:
		save();
		break;
	case QMessageBox::Ignore:
		curFileInfo->refresh();
		break;
	}
}

void QsciApp::checkModifiedOnDisk()
{
	if (curFile.isEmpty()) {
		return;
	}

	QFileInfo info = QFileInfo(curFile);
	if (info.lastModified() != curFileInfo->lastModified()) {
		if (textEdit->isModified()) {
			askReloadOrKeep();
		} else {
			askReload();
		}
	}
}

void QsciApp::markLine(int line)
{
	static int mnr = -1;
	if (mnr < 0) {
		mnr = textEdit->markerDefine(QsciScintilla::RightTriangle, -1);
	} else {
		textEdit->markerDeleteAll(mnr);
	}

	textEdit->markerAdd(line, mnr);
}
