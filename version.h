#ifndef VERSION_H
#define VERSION_H

class QString;
class Version {
public:
	static QString getVersionString();
	static QString getBuildDate();
	static QString getBuildFlagsString();
	static QString versionText(QString input);
};

#endif

