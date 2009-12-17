#ifndef QSCIAPP_H
#define QSCIAPP_H

#include <QMainWindow>
class QPlainTextEdit;

class QsciApp : public QMainWindow
{
	Q_OBJECT

public:
	QsciApp();

private slots:
	void documentWasModified();

private:
	QPlainTextEdit *textEdit;
};
#endif

