#include <QtGui>
#include "qsciapp.h"

int main(int argc, char *argv[])
{
	QString fileName = "";
	unsigned int line = 1;

	// Let Qt have first dibs on the command line arguments
	QApplication app(argc, argv);

	// A line number can be specified 2 ways
	// Grep syntax <file>:<line>
	// Vim syntax  <file> +<line>
	if (argc > 2 ) {
		QString arg = argv[2];
		line = arg.toUInt();
	}
	if (argc > 1) {
		fileName = argv[1];
		QString arg = argv[1];
		QStringList list = arg.split(":"); // danger windows
		fileName = list.at(0);
		if (list.size() > 1) {
			line = list.at(1).toUInt();
		}
	}

	QsciApp *qsciapp = new QsciApp(fileName, line);
	qsciapp->show();

	return app.exec();
}
