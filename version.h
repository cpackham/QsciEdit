#ifndef VERSION_H
#define VERSION_H

class QString;
class Version {
public:
	Version();
	QString getVersionString();
	QString getBuildDate();
	QString versionText(QString input);

private:
	QString versionString;
	QString buildDate;
};

#endif

