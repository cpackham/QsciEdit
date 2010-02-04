#include <QDebug>
#include <QColor>
#include <QFont>
#include <qsciscintilla.h>
#include <qscistyle.h>
#include "miblexer.h"

MibLexer::MibLexer(QObject *parent)
	: QsciLexerCustom(parent)
{
	styleStack = QList<int>();
	resetStyle ();
	initKeywords();
	initAccesses();
	initTypes();
	
	declareStyle(Default,
		QColor(0x0, 0x0, 0x0),
		QColor("white"),
		QFont("Liberation Mono", 10));
	declareStyle(String,
		QColor(0x80, 0x0, 0x80),
		QColor("white"),
		QFont("Liberation Mono", 10));
	declareStyle(Keyword,
		QColor(0x0, 0x0, 0xff),
		QColor("white"),
		QFont("Liberation Mono", 10));
	declareStyle(Type,
		QColor(0xe0, 0x0, 0x0),
		QColor("white"),
		QFont("Liberation Mono", 10));
	declareStyle(Access,
		QColor(0xff, 0x0, 0x0),
		QColor("white"),
		QFont("Liberation Mono", 10));
	declareStyle(Comment,
		QColor(0x0, 0x80, 0x0),
		QColor("white"),
		QFont("Liberation Mono", 10));
	declareStyle(OID,
		QColor(0x0, 0x80, 0x80),
		QColor("white"),
		QFont("Liberation Mono", 10));
}

MibLexer::~MibLexer()
{
	qDebug() << __FUNCTION__;
}

const char* MibLexer::language() const
{
	return "MibLexer";
}

QString MibLexer::description(int style) const
{
	if (styleNames.contains(style)) {
		return styleNames[style];
	} else {
		return QString("");
	}
}

void MibLexer::styleText(int start, int end)
{
	QString source;
	
//  	qDebug() << __FUNCTION__ << start << end;
	
	if (!editor())
		return;
	
	resetStyle ();

	char *chars = new char[(end - start) + 1];
	editor()->SendScintilla(QsciScintilla::SCI_GETTEXTRANGE, start, end, chars);
	source = QString(chars);
	delete [] chars;
	
// 	qDebug() << "source =" << source;
	startStyling(start, 0x1f);
	QStringList list = source.split("\n");
	QStringList::iterator iter;
	for(iter = list.begin(); iter != list.end(); iter++) {
		styleLine(*iter);
	}

}

void MibLexer::styleLine(QString line)
{
	QStringList list = line.split(QRegExp("[\\s]"));
	QStringList::iterator iter;
	
	QString kwPat = "(" + keywords.join("|") + ")[,]*";
	QRegExp kwReg(kwPat);

	QString accessPat = "(" + accesses.join("|") + ")[,]*";
	QRegExp accessReg(accessPat);

	QString typePat = "(" + types.join("|") + ")[,]*";
	QRegExp typeReg(typePat);

	QRegExp oidReg("\\d+\\.(\\d+\\.)*\\d+");
	QRegExp commentReg("--.*");
	QRegExp stringReg("\"[^\"]*\"");
	QRegExp stringStart("\"[^\"]*");
	QRegExp stringEnd("[^\"]*\"");
	
	for(iter = list.begin(); iter != list.end(); iter++) {
		QString word = *iter;
		int len = word.size();
		bool pop = false;
		
		if (kwReg.exactMatch(word)) {
			pop = pushStyle(Keyword);
		} else if (accessReg.exactMatch(word)) {
			pop = pushStyle(Access);
		} else if (typeReg.exactMatch(word)) {
			pop = pushStyle(Type);
		} else if (oidReg.exactMatch(word)) {
			pop = pushStyle(OID);
		} else if (commentReg.exactMatch(word) && !hasStyle(Comment)) {
			pushStyle(Comment);
		} else if (stringReg.exactMatch(word)) {
			pop = pushStyle(String);
		} else if (stringStart.exactMatch(word) && !hasStyle(String)) {
			pushStyle(String);
		} else if (stringEnd.exactMatch(word) && getStyle() == String) {
			pop = true;
		}
		
		setStyling(len, getStyle());
		if (pop)
			popStyle();
		setStyling(1, getStyle());
	}
	
	if (getStyle() == Comment)
		popStyle();
	
}

bool MibLexer::pushStyle(int style)
{
	bool pushed = false;
	
	if (style >= getStyle()) {
		styleStack << style;
		pushed = true;
	}
	return pushed;
}

bool MibLexer::popStyle()
{
	int style;
	bool popped = false;
	if (styleStack.size() > 1) {
		style = styleStack.takeLast() ;
		popped = true;
	}
	return popped;
}

int MibLexer::getStyle()
{
	int style;
	style = styleStack.at(styleStack.size()-1) ;
	return style;
}

bool MibLexer::hasStyle(int style)
{
	return styleStack.contains(style);
}

void MibLexer::resetStyle()
{
	styleStack.clear();
	styleStack << Default;
}

void MibLexer::initKeywords()
{
	keywords
		<< "BEGIN"
		<< "CONTACT-INFO"
		<< "DEFINITIONS"
		<< "DESCRIPTION"
		<< "END"
		<< "FROM"
		<< "IDENTIFIER"
		<< "IMPORTS"
		<< "INDEX"
		<< "LAST-UPDATED"
		<< "MAX-ACCESS"
		<< "OBJECT"
		<< "OBJECTS"
		<< "ORGANIZATION"
		<< "REVISION"
		<< "SEQUENCE" 
		<< "STATUS"
		<< "SYNTAX";
}

void MibLexer::initAccesses()
{
	accesses
		<< "not-accessible"
		<< "read-create"
		<< "read-only"
		<< "read-write"
		<< "write-only";
}

void MibLexer::initTypes()
{
	types
		<< "DisplayString"
		<< "InetAddress"
		<< "InetAddressType"
		<< "Integer32"
		<< "InterfaceIndex"
		<< "INTEGER"
		<< "MODULE-IDENTITY"
		<< "NOTIFICATION-TYPE"
		<< "OBJECT-IDENTITY"
		<< "OBJECT-TYPE"
		<< "RowStatus";
}
