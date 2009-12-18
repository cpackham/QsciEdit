#include <QtGui>
#include <qsciscintilla.h>
#include <qscilexer.h>
#include "qsciapp.h"
#include "qticonloader.h"
#include "lexersel.h"

QsciApp::QsciApp()
{
	textEdit = new QsciScintilla;

	createActions();
	createMenus();
	createToolBars();
	createStatusBar();

	connect(textEdit, SIGNAL(textChanged()),
		this, SLOT(documentWasModified()));

	setCentralWidget(textEdit);
	setCurrentFile("");
}

void QsciApp::closeEvent(QCloseEvent *event)
{
	if (saveIfModified()) {
		event->accept();
	} else {
		event->ignore();
	}
}

void QsciApp::documentWasModified()
{
	setWindowModified(textEdit->isModified());
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

	connect(textEdit, SIGNAL(copyAvailable(bool)),
		cutAct, SLOT(setEnabled(bool)));
	connect(textEdit, SIGNAL(copyAvailable(bool)),
		copyAct, SLOT(setEnabled(bool)));

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

	viewMenu = menuBar()->addMenu(tr("&View"));
	settingsMenu = menuBar()->addMenu(tr("&Settings"));
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

bool QsciApp::saveIfModified()
{
	if (textEdit->isModified()) {
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, tr("QsciApp"),
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

	setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("QsciApp")));
	textEdit->setLexer(LexerSelector::getLexerForFile(fileName));
	if (textEdit->lexer()) {
		QFont font = QFont("Monospaced, Courier", 10);
		textEdit->lexer()->setDefaultFont(font);
		textEdit->lexer()->setFont(font);
	}
}
