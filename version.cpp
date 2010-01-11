#include <QString>
#include <iostream>
#include "version.h"

Version::Version()
{
	versionString = "QsciEdit version @@VERSION@@";
	buildDate = "Built on @@DATE@@";
}

QString Version::getVersionString()
{
	return versionString;
}

QString Version::getBuildDate()
{
	return buildDate;
}

QString Version::versionText(QString input)
{
	input.replace ("GIT_VERSION_STRING", getVersionString());
	input.replace ("BUILD_DATE_STRING", getBuildDate());

	return input;
}
