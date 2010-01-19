#ifndef VERSION_H
#define VERSION_H

class QString;
class Version {
public:
	Version();
	QString getVersionString();
	QString getBuildDate();
	QString getBuildFlagsString();
	QString versionText(QString input);

private:
	QString versionString;
	QString buildDate;
	QString buildFlags;
};

#endif

