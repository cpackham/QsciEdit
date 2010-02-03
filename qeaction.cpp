#include <QtGui>
#include "qeaction.h"

QEAction* QEAction::newAction(QString shortstr,
			QObject *parent,
			QString longstr,
			QKeySequence keyseq, 
			bool isCheckable,
			bool isChecked,
			bool inMenus,
			bool inToolbars)
{
	QEAction *act;

	act = new QEAction(shortstr, parent);

	act->setStatusTip(longstr);
	act->setCheckable(isCheckable);
	act->setChecked(isChecked);

	if (keyseq)
		act->setShortcut(keyseq);

	act->showInMenu = inMenus;
	act->showInToolbar = inToolbars;

	return act;
}

QEAction* QEAction::newAction(QIcon icon, QString shortstr,
			QObject *parent,
			QString longstr,
			QKeySequence keyseq, 
			bool isCheckable,
			bool isChecked,
			bool inMenus,
			bool inToolbars)
{
	QEAction *act;

	act = newAction(shortstr, parent, longstr, keyseq, isCheckable,
			isChecked, inMenus, inToolbars);
	act->setIcon(icon);

	return act;
}
