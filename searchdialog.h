#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QObject>
#include <QDialog>

class QDialog;
class QLabel;
class QLineEdit;
class QPushButton;
class QCheckBox;

class FindDialog : public QDialog
{
	Q_OBJECT;
public:
	FindDialog(QWidget *parent=0);
	void setSearchText(const QString text);

signals:
	void findText(const QString text, bool regex, bool caseSensitive,
		bool wholeWord, bool wrap, bool backwards);

private slots:
	void findButtonPressed();
private:
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
