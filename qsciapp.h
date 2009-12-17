#ifndef QSCIAPP_H
#define QSCIAPP_H

#include <QMainWindow>
class QsciScintilla;

class QsciApp : public QMainWindow
{
	Q_OBJECT

public:
	QsciApp();

private slots:
	void documentWasModified();

private:
	QsciScintilla *textEdit;
};
#endif

