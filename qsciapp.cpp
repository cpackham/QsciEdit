#include <QtGui>
#include <qsciscintilla.h>
#include <qscilexercpp.h>
#include "qsciapp.h"

QsciApp::QsciApp()
{
	textEdit = new QsciScintilla;
	QsciLexerCPP *lexer = new QsciLexerCPP;
	textEdit->setLexer(lexer);

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
	if (1) {
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
#define new_action(act, shortstr, longstr, keyseq, slot) \
	act = new QAction(shortstr, this); \
	act->setShortcuts(keyseq); \
	act->setStatusTip(longstr); \
	connect(act, SIGNAL(triggered()), this, SLOT(slot))

	new_action(newAct,tr("&New"),
			tr("Create a new file"), 
			QKeySequence::New,newFile());
	new_action(openAct, tr("&Open"),
			tr("Open an existing file"),
			QKeySequence::Open,open());
	new_action(saveAct,tr("&Save"),
			tr("Save the current file"),
			QKeySequence::Save,save());
	new_action(saveAsAct,tr("Save &As"),
			tr("Save the current file with a new name"),
			QKeySequence::SaveAs,saveAs());
}

void QsciApp::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newAct);
	fileMenu->addAction(openAct);
	fileMenu->addAction(saveAct);
	fileMenu->addAction(saveAsAct);

	editMenu = menuBar()->addMenu(tr("&Edit"));
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
}

void QsciApp::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}

void QsciApp::newFile()
{
	textEdit->setText("");
}

void QsciApp::open()
{
	QString fileName = QFileDialog::getOpenFileName(this);
	if (!fileName.isEmpty()) {
		loadFile(fileName);
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
}
