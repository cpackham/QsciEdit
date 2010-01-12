#include <cstdio>
#include <QtGui>
#include <qsciscintilla.h>
#include <qscilexer.h>
#include "qsciapp.h"
#include "qticonloader.h"
#include "lexersel.h"
#include "finddialog.h"
#include "actions.h"
#include "globals.h"
#include "editorsettings.h"
#include "version.h"

QsciApp::QsciApp(const QString fileName, unsigned int line)
{
	textEdit = new QsciScintilla;
	findDialog = NULL;
	lineCommentString = "";
	blockCommentStartString = "";
	blockCommentMiddleString = "";
	blockCommentEndString = "";
	fileProvided = false;
	editorSettings = new EditorSettings;

	// Order here is important
	createStatusBar();
	setCurrentFile(fileName);

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

	// The QsciScintilla drag & drop behaviour is to put the URL in as text
	// in the current document. We want it to open the file url so disable
	// the QsciScintilla behaviour.
	textEdit->setAcceptDrops(false);

	connect(textEdit, SIGNAL(modificationChanged(bool)),
		this, SLOT(documentModified(bool)));

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
	Version version;

	if(file.open(QFile::ReadOnly | QFile::Text)) {
		QTextStream in(&file);
		data = in.readAll();
	}

	QMessageBox::about(this,
		tr("About %1").arg(APPLICATION_NAME),
		version.versionText(data));
}

void QsciApp::gotoLine(int line)
{
	textEdit->setCursorPosition(line-1, 0);
	textEdit->ensureCursorVisible ();
	textEdit->ensureLineVisible (line+9);
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

	QString shownName;
	if (curFile.isEmpty())
		shownName = "untitled";
	else
		shownName = QFileInfo(curFile).fileName();

	setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(APPLICATION_NAME));
	textEdit->setLexer(LexerSelector::getLexerForFile(fileName,
			&lineCommentString,
			&blockCommentStartString,
			&blockCommentMiddleString,
			&blockCommentEndString));
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
	editorSettings->load();

	move (pos);
	resize(size);
	if (!fileProvided && !file.isEmpty())
		loadFile(file);
}

void QsciApp::saveSettings()
{
	QSettings settings(COMPANY_NAME, APPLICATION_NAME);
	settings.setValue("pos", pos());
	settings.setValue("size", size());
	settings.setValue("file", curFile);
	editorSettings->save();
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
}
