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

public slots:
	void setRegex(bool b) { regex = b; };
	void setCaseSensitive(bool b) { caseSensitive = b; };
	void setWholeWord(bool b) { wholeWord = b; };
	void setWrap(bool b) { wrap = b; };
	void setBackwards(bool b) { backwards = b; };
};


class SearchDialog : public QDialog
{
	Q_OBJECT;
public:
	SearchDialog(QWidget *parent=0);
	void setSearchText(const QString text);

signals:
	void searchText(const QString text, SearchOptions *opts);

private slots:
	void findButtonPressed();
private:
	SearchOptions *options;
	QLabel *label;
	QLineEdit *entry;
	QCheckBox *regexCheck;
	QCheckBox *caseCheck;
	QCheckBox *wholeCheck;
	QCheckBox *wrapCheck;
	QCheckBox *backCheck;
	QPushButton *findButton;
	QPushButton *cancelButton;
};

#endif
