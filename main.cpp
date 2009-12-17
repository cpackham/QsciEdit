#include <iostream>
#include <QtGui>
#include "qsciapp.h"

int main(int argc, char *argv[])
{
	std::cout << __FUNCTION__<< std::endl;

	QApplication app(argc, argv);
	QsciApp *qsciapp = new QsciApp;
	qsciapp->show();

	return app.exec();
}
