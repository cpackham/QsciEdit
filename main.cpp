#include <iostream>
#include <QtGui>
#include "qsciapp.h"

int main(int argc, char *argv[])
{
	std::cout << __FUNCTION__<< std::endl;
	QString fileName = "";

	// Let Qt have first dibs on the command line arguments
	QApplication app(argc, argv);

	if (argc > 1)
		fileName = argv[1];

	QsciApp *qsciapp = new QsciApp(fileName);
	qsciapp->show();

	return app.exec();
}
