#include <QtGui>
#include "searchdialog.h"

SearchOptions::SearchOptions(QObject *parent):
	QObject(parent)
{
	regex = false;
	caseSensitive  = true;
	wholeWord = false;
	wrap = false;
	backwards  = false;
	start = true;
}

SearchDialog::SearchDialog(QWidget *parent)
	: QDialog(parent)
{
	options = new SearchOptions();
	setWindowTitle("Search/Replace");

	QHBoxLayout *input = new QHBoxLayout();
	label = new QLabel(tr("Find Text:"));
	entry = new QLineEdit();
	entry->setMinimumWidth(200);
	label->setBuddy(entry);
	input->addWidget(label);
	input->addWidget(entry);

	QHBoxLayout *replace = new QHBoxLayout();
	rlabel = new QLabel(tr("Replace With:"));
	rentry = new QLineEdit();
	rentry->setMinimumWidth(200);
	rlabel->setBuddy(rentry);
	replace->addWidget(rlabel);
	replace->addWidget(rentry);

	QVBoxLayout *optionLayout = new QVBoxLayout();
	regexCheck = new QCheckBox("&Match Regular Expression");
	caseCheck = new QCheckBox("&Match Case");
	wholeCheck = new QCheckBox("W&hole Word");
	wrapCheck = new QCheckBox("&Wrap Search");
	backCheck = new QCheckBox("Search &Backwards");
	advCheck = new QCheckBox("Advance on Replace");

	regexCheck->setChecked(options->regex);
	caseCheck->setChecked(options->caseSensitive);
	wholeCheck->setChecked(options->wholeWord);
	wrapCheck ->setChecked(options->wrap);
	backCheck->setChecked(options->backwards);
	advCheck->setChecked(true);

	optionLayout->addLayout(input);
	optionLayout->addLayout(replace);
	optionLayout->addWidget(regexCheck);
	optionLayout->addWidget(caseCheck);
	optionLayout->addWidget(wholeCheck);
	optionLayout->addWidget(wrapCheck);
	optionLayout->addWidget(backCheck);
	optionLayout->addWidget(advCheck);

	QDialogButtonBox *buttons = new QDialogButtonBox(Qt::Vertical);
	findButton = new QPushButton(tr("&Find"));
	findButton->setDefault(true);
	replaceButton = new QPushButton(tr("&Replace"));
	cancelButton = new QPushButton(tr("&Close"));
	buttons->addButton(findButton, QDialogButtonBox::ActionRole);
	buttons->addButton(replaceButton, QDialogButtonBox::ActionRole);
	buttons->addButton(cancelButton, QDialogButtonBox::ActionRole);

	QGridLayout *mainLayout = new QGridLayout();
	mainLayout->addLayout(optionLayout, 0, 0);
	mainLayout->addWidget(buttons, 0, 1);
	setLayout(mainLayout);

	connect(entry, SIGNAL(textChanged(const QString)), this, SLOT(textChanged(const QString)));
	connect(entry, SIGNAL(returnPressed()), this, SLOT(findButtonPressed()));
	connect(findButton, SIGNAL(pressed()), this, SLOT(findButtonPressed()));
	connect(replaceButton, SIGNAL(pressed()), this, SLOT(replaceButtonPressed()));
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

	if (!text.isEmpty()) {
		emit searchText(text, options);
	}
}

void SearchDialog::replaceButtonPressed()
{
	QString text;
	text = rentry->text();

	if (text.isEmpty())
		return;
	if (options->start)
		return;

	emit replaceWithText(text);

	if (advCheck->isChecked()) {
		findButtonPressed();
	}
}

void SearchDialog::textChanged(const QString text)
{
	options->start = true;
}
