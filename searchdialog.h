#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QObject>
#include <QDialog>

class QDialog;
class QLabel;
class QLineEdit;
class QPushButton;
class QCheckBox;

class SearchOptions : public QObject
{
	Q_OBJECT;
public:
	SearchOptions(QObject *parent=0);
	bool regex;
	bool caseSensitive;
	bool wholeWord;
	bool wrap;
	bool backwards;
	bool start;

public slots:
	void setRegex(bool b) { regex = b; start = true; };
	void setCaseSensitive(bool b) { caseSensitive = b; start = true; };
	void setWholeWord(bool b) { wholeWord = b; start = true; };
	void setWrap(bool b) { wrap = b; start = true; };
	void setBackwards(bool b) { backwards = b; start = true; };
};


class SearchDialog : public QDialog
{
	Q_OBJECT;
public:
	SearchDialog(QWidget *parent=0);
	void setSearchText(const QString text);

signals:
	void searchText(const QString text, SearchOptions *opts);
	void replaceWithText(const QString text);

private slots:
	void findButtonPressed();
	void replaceButtonPressed();
	void textChanged(const QString);
private:
	SearchOptions *options;
	QLabel *label;
	QLineEdit *entry;
	QLabel *rlabel;
	QLineEdit *rentry;
	QCheckBox *regexCheck;
	QCheckBox *caseCheck;
	QCheckBox *wholeCheck;
	QCheckBox *wrapCheck;
	QCheckBox *backCheck;
	QCheckBox *advCheck;
	QPushButton *findButton;
	QPushButton *replaceButton;
	QPushButton *cancelButton;
};

#endif
