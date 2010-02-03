#ifndef QEACTION_H
#define QEACTION_H

#include <QAction>

class QEAction : public QAction
{
	Q_OBJECT

public:
	QEAction(QObject *parent) : QAction (parent) {};
	QEAction(const QString & text, QObject *parent) : QAction (text, parent) {};
	QEAction(const QIcon & icon, const QString & text, QObject *parent) : QAction (icon, text, parent) {};

	bool showInMenu;
	bool showInToolbar;

	static QEAction* newAction(QString shortstr, QObject *parent,
			QString longstr = "",
			QKeySequence keyseq=0, 
			bool isCheckable = false,
			bool isChecked = false,
			bool inMenus = true,
			bool inToolbars = true);

	static QEAction* newAction(QIcon icon, QString shortstr, QObject *parent,
			QString longstr = "",
			QKeySequence keyseq=0, 
			bool isCheckable = false,
			bool isChecked = false,
			bool inMenus = true,
			bool inToolbars = true);


};

#endif
