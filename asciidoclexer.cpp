#include <QDebug>
#include <QColor>
#include <QFont>
#include <qsciscintilla.h>
#include <qscistyle.h>
#include "asciidoclexer.h"

AsciiDocLexer::AsciiDocLexer(QObject *parent)
	: QsciLexerCustom(parent)
{
	styleStack = QList<int>();
	resetStyle ();
	
	keywordList = QList<QString>();
	keywordList << "NOTE:" << "TIP:" << "IMPORTANT:"
		<< "WARNING:" << "CAUTION:";
	
	declareStyle(Default,
		QColor(0x0, 0x0, 0x0),
		QColor("white"),
		QFont("Liberation Mono", 10));
	declareStyle(Comment,
		QColor(0x0, 0x80, 0x0),
		QColor("white"),
		QFont("Liberation Mono", 10));
	declareStyle(Header,
		QColor(0x80, 0x0, 0x0),
		QColor(0xff, 0xf0, 0xf0),
		QFont("Liberation Mono", 10));
	setEolFill(true, Header);
	declareStyle(ParaHeader,
		QColor(0x80, 0x0, 0x0),
		QColor("white"),
		QFont("Liberation Mono", 10));
	declareStyle(BoldText,
		QColor(0x0, 0x0, 0xe0),
		QColor("white"),
		QFont("Liberation Mono", 10));
	declareStyle(ItalicText,
		QColor(0x80, 0x0, 0xe0),
		QColor("white"),
		QFont("Liberation Mono", 10));
	declareStyle(FixedText,
		QColor(0x40, 0x0, 0xe0),
		QColor("white"),
		QFont("Liberation Mono", 10));
	declareStyle(SuperText,
		QColor(0x40, 0xe0, 0xe0),
		QColor("white"),
		QFont("Liberation Mono", 10));
	declareStyle(SubText,
		QColor(0x0, 0x80, 0x80),
		QColor("white"),
		QFont("Liberation Mono", 10));
	declareStyle(URL,
		QColor(0x0, 0x0, 0x80),
		QColor("white"),
		QFont("Liberation Mono", 10));
	declareStyle(ListingBlock,
		QColor(0x0, 0x0, 0x40),
		QColor(0xe0, 0xe0, 0xff),
		QFont("Liberation Mono", 10));
	setEolFill(true, ListingBlock);
	declareStyle(SideBarBlock,
		QColor(0xe0, 0x80, 0x0),
		QColor("white"),
		QFont("Liberation Mono", 10));
	declareStyle(Keyword,
		QColor(0xe0, 0x0, 0x0),
		QColor("white"),
		QFont("Liberation Mono", 10));
	declareStyle(Bullet,
		QColor(0xe0, 0x0, 0x0),
		QColor("white"),
		QFont("Liberation Mono", 10));
}

AsciiDocLexer::~AsciiDocLexer()
{
	qDebug() << __FUNCTION__;
}

const char* AsciiDocLexer::language() const
{
	return "AsciiDoc";
}

QString AsciiDocLexer::description(int style) const
{
	if (styleNames.contains(style)) {
		return styleNames[style];
	} else {
		return QString("");
	}
}

void AsciiDocLexer::styleText(int start, int end)
{
	QString source;
	QRegExp paraHead("\\.[^ .].*");
	
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

	QStringList::iterator iter, next;
	for (iter = list.begin(); iter != list.end(); iter = next) {
		bool pop = false;
		next = iter+1;
		QString line = *iter;

		int len = line.size();
// 		qDebug() << "line =" << line;

		// The next line could make the current one a header.
		if (next != list.end()) {
			QString nextLine = *next;
			if ( nextLine.startsWith("===") || 
				nextLine.startsWith("~~") || 
				(nextLine.startsWith("--") && nextLine != "----")) {
					pop = pushStyle(Header);
					goto endloop;
		       }
		}

		if (line.startsWith("//")) {
			pushStyle(Comment);
		} else if (line.startsWith("==") || 
			   line.startsWith("~~") || 
			   (line.startsWith("--") && line != "----")) {
			pop = pushStyle(Header);
		} else if (paraHead.exactMatch(line)) {
			pop = pushStyle(ParaHeader);
		} else if (line == "****"){
			if (hasStyle(SideBarBlock))
				pop = true;
			else
				pushStyle(SideBarBlock);
		} else if (line == "----"){
			if (hasStyle(ListingBlock))
				pop = true;
			else
				pushStyle(ListingBlock);
				
		} else if (!line.isEmpty()){
			styleLine(line);
			continue;
		}
		
endloop:		
// 		qDebug() << "Styling " << len << "bytes " << description(getStyle());
		setStyling(len, getStyle());
		// newline character was consumed in split so...
	 	setStyling(1, getStyle());
		
		if (pop)
			popStyle();
	}
}

void AsciiDocLexer::styleLine(QString line)
{
	QStringList list = line.split(" ");
	QStringList::iterator iter;
	QRegExp startstyledtext("([*_+^~])[^ ].*");
	QRegExp endstyledtext(".+([*_+^~])[.,);:]?");
	QRegExp bullet("[*]+");
	QRegExp numbered("[.]+");

	for (iter = list.begin(); iter != list.end(); iter++) {
		QString word = *iter;
		int len = word.size();
		bool pop = false;

// 		qDebug() << "word =" << word;
		if (word.startsWith("http://")) { 
			pop = pushStyle(URL);
		} else if (keywordList.contains(word)) {
			pop = pushStyle(Keyword);
		} else if (bullet.exactMatch(word) || numbered.exactMatch(word)) {
			pop = pushStyle(Bullet);
		} else if (startstyledtext.exactMatch(word)) {
			QStringList list = startstyledtext.capturedTexts();
			QChar type = list[1][0];
			
			switch (type.toLatin1()) {
			case '_':
				pushStyle(ItalicText);
				break;
			case '*':
				pushStyle(BoldText);
				break;
			case '+':
				pushStyle(FixedText);
				break;
			case '^':
				pushStyle(SuperText);
				break;
			case '~':
				pushStyle(SubText);
				break;
			}
		}
		
// 		qDebug() << "Styling " << len << "bytes " << description(getStyle());
		setStyling(len, (getStyle()));

		if (endstyledtext.exactMatch(word)) {
			QStringList list = endstyledtext.capturedTexts();
			QChar type = list[1][0];

			switch (type.toLatin1()) {
			case '_':
				pop = hasStyle(ItalicText);
				break;
			case '*':
				pop = hasStyle(BoldText);
				break;
			case '+':
				pop = hasStyle(FixedText);
				break;
			case '^':
				pop = hasStyle(SuperText);
				break;
			case '~':
				pop = hasStyle(SubText);
				break;
			}
		}

		if (pop) 
			popStyle();

		setStyling(1, (getStyle()));
	}
}

bool AsciiDocLexer::pushStyle(int style)
{
	bool pushed = false;
	
	if (style >= getStyle()) {
		styleStack << style;
		pushed = true;
// 		qDebug() << __FUNCTION__ << description(style);
	}
	return pushed;
}

bool AsciiDocLexer::popStyle()
{
	int style;
	bool popped = false;
	if (styleStack.size() > 1) {
		style = styleStack.takeLast() ;
		popped = true;
// 		qDebug() << __FUNCTION__ << description(style);
	}
	return popped;
}

int AsciiDocLexer::getStyle()
{
	int style;
	style = styleStack.at(styleStack.size()-1) ;
	return style;
}

bool AsciiDocLexer::hasStyle(int style)
{
	return styleStack.contains(style);
}

void AsciiDocLexer::resetStyle()
{
	styleStack.clear();
	styleStack << Default;
}

