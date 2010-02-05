#include <QtGui>
#include "finddialog.h"

FindDialog::FindDialog(QWidget *parent)
	: QDialog(parent)
{
	QHBoxLayout *input = new QHBoxLayout();
	label = new QLabel(tr("Find Text:"));
	entry = new QLineEdit();
	label->setBuddy(entry);
	input->addWidget(label);
	input->addWidget(entry);

	QGridLayout *options = new QGridLayout();
	regexCheck = new QCheckBox("&Regular Expression");
	caseCheck = new QCheckBox("&Match Case");
	caseCheck->setChecked(true);
	wholeCheck = new QCheckBox("W&hole Word");
	wrapCheck = new QCheckBox("&Wrap");
	backCheck = new QCheckBox("Search &Backwards");
	options->addWidget(regexCheck, 0, 0);
	options->addWidget(caseCheck, 0, 1);
	options->addWidget(wholeCheck, 1, 0);
	options->addWidget(wrapCheck, 1, 1);
	options->addWidget(backCheck, 2, 0);

	QHBoxLayout *buttons = new QHBoxLayout();
	findButton = new QPushButton(tr("&Find"));
	findButton->setDefault(true);
	cancelButton = new QPushButton(tr("&Close"));
	buttons->addWidget(findButton);
	buttons->addWidget(cancelButton);

	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->addLayout(input);
	mainLayout->addLayout(options);
	mainLayout->addLayout(buttons);
	setLayout(mainLayout);

	connect(findButton, SIGNAL(pressed()), this, SLOT(findButtonPressed()));
	connect(cancelButton, SIGNAL(pressed()), this, SLOT(close()));
}

void FindDialog::setSearchText(const QString text)
{
	entry->setText(text);
}

void FindDialog::findButtonPressed()
{
	QString text;
	text = entry->text();
	if (!text.isEmpty()){
		emit findText(text, 
			regexCheck->isChecked(),
			caseCheck->isChecked(),
			wholeCheck->isChecked(),
			wrapCheck->isChecked(),
			backCheck->isChecked());
	}
}
