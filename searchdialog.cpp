#include <QtGui>
#include "searchdialog.h"

SearchOptions::SearchOptions(QObject *parent):
	QObject(parent)
{
	regex = false;
	caseSensitive  = false;
	wholeWord = false;
	wrap = false;
	backwards  = false;
}

SearchDialog::SearchDialog(QWidget *parent)
	: QDialog(parent)
{
	QHBoxLayout *input = new QHBoxLayout();
	options = new SearchOptions();
	label = new QLabel(tr("Find Text:"));
	entry = new QLineEdit();
	label->setBuddy(entry);
	input->addWidget(label);
	input->addWidget(entry);

	QGridLayout *optionLayout = new QGridLayout();
	regexCheck = new QCheckBox("&Regular Expression");
	caseCheck = new QCheckBox("&Match Case");
	caseCheck->setChecked(true);
	wholeCheck = new QCheckBox("W&hole Word");
	wrapCheck = new QCheckBox("&Wrap");
	backCheck = new QCheckBox("Search &Backwards");
	optionLayout->addWidget(regexCheck, 0, 0);
	optionLayout->addWidget(caseCheck, 0, 1);
	optionLayout->addWidget(wholeCheck, 1, 0);
	optionLayout->addWidget(wrapCheck, 1, 1);
	optionLayout->addWidget(backCheck, 2, 0);

	QHBoxLayout *buttons = new QHBoxLayout();
	findButton = new QPushButton(tr("&Find"));
	findButton->setDefault(true);
	cancelButton = new QPushButton(tr("&Close"));
	buttons->addWidget(findButton);
	buttons->addWidget(cancelButton);

	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->addLayout(input);
	mainLayout->addLayout(optionLayout);
	mainLayout->addLayout(buttons);
	setLayout(mainLayout);

	connect(findButton, SIGNAL(pressed()), this, SLOT(findButtonPressed()));
	connect(cancelButton, SIGNAL(pressed()), this, SLOT(close()));
	connect(regexCheck, SIGNAL(toggled(bool)), options, SLOT(setRegex(bool)));
	connect(caseCheck, SIGNAL(toggled(bool)), options, SLOT(setCaseSensitive(bool)));
	connect(wholeCheck, SIGNAL(toggled(bool)), options, SLOT(setWholeWord(bool)));
	connect(wrapCheck, SIGNAL(toggled(bool)), options, SLOT(setWrap(bool)));
	connect(backCheck, SIGNAL(toggled(bool)), options, SLOT(setBackwards(bool)));
}

void SearchDialog::setSearchText(const QString text)
{
	entry->setText(text);
}

void SearchDialog::findButtonPressed()
{
	QString text;
	text = entry->text();

	if (!text.isEmpty()){
		emit findText(text, options);
	}
}
