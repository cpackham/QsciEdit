#include <QtGui>
#include <qsciscintilla.h>
#include <qscilexercpp.h>
#include "qsciapp.h"

QsciApp::QsciApp()
{
	textEdit = new QsciScintilla;
	QsciLexerCPP *lexer = new QsciLexerCPP;
	textEdit->setLexer(lexer);

	connect(textEdit, SIGNAL(textChanged()),
		this, SLOT(documentWasModified()));

	setCentralWidget(textEdit);
	setWindowTitle(tr("QsciApp [*]"));

}

void QsciApp::documentWasModified()
{
	setWindowModified(textEdit->isModified());
}

