#include <QtGui>
#include "qsciapp.h"

QsciApp::QsciApp()
{
	textEdit = new QPlainTextEdit;
	setCentralWidget(textEdit);

	connect(textEdit->document(), SIGNAL(contentsChanged()),
		this, SLOT(documentWasModified()));

	setWindowTitle(tr("QsciApp [*]"));

}

void QsciApp::documentWasModified()
{
	setWindowModified(textEdit->document()->isModified());
}

