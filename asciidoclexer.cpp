#include <QDebug>
#include <QColor>
#include <QFont>
#include <qsciscintilla.h>
#include <qscistyle.h>
#include "asciidoclexer.h"

AsciiDocLexer::AsciiDocLexer(QObject *parent)
	: QsciLexerCustom(parent)
{
	qDebug() << __FUNCTION__;
	declareStyle(Default,
		QColor(0x0, 0x0, 0x0),
		QColor("white"),
		QFont("Monospaced, Courier", 10));
	declareStyle(Comment,
		QColor(0x0, 0x80, 0x0),
		QColor("white"),
		QFont("Monospaced, Courier", 10));
	declareStyle(Header,
		QColor(0x80, 0x0, 0x0),
		QColor(0xff, 0xf0, 0xf0),
		QFont("Monospaced, Courier", 10));
	setEolFill(true, Header);
	declareStyle(ParaHeader,
		QColor(0x80, 0x0, 0x0),
		QColor("white"),
		QFont("Monospaced, Courier", 10));
	declareStyle(BoldText,
		QColor(0x0, 0x0, 0xe0),
		QColor(0xe0, 0xe0, 0xff),
		QFont("Monospaced, Courier", 10));
	declareStyle(ItalicText,
		QColor(0x80, 0x0, 0xe0),
		QColor("white"),
		QFont("Monospaced, Courier", 10));
	declareStyle(FixedText,
		QColor(0x40, 0x0, 0xe0),
		QColor("white"),
		QFont("Monospaced, Courier", 10));
	declareStyle(URL,
		QColor(0x0, 0x0, 0x80),
		QColor("white"),
		QFont("Monospaced, Courier", 10));
	declareStyle(ListingBlock,
		QColor(0x0, 0x0, 0x40),
		QColor(0xe0, 0xe0, 0xff),
		QFont("Monospaced, Courier", 10));
	setEolFill(true, ListingBlock);
	declareStyle(SideBarBlock,
		QColor(0xe0, 0x80, 0x0),
		QColor("white"),
		QFont("Monospaced, Courier", 10));
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

	qDebug() << __FUNCTION__ 
		<< "start =" << start
		<< " end =" << end;

	if (!editor())
		return;

	char *chars = new char[(end - start) + 1];
	editor()->SendScintilla(QsciScintilla::SCI_GETTEXTRANGE, start, end, chars);
	source = QString(chars);
	delete [] chars;

	qDebug() << "source =" << source;
	
	startStyling(start, 0x1f);
	QStringList list = source.split("\n");
	int defaultstyle = Default;

	QStringList::iterator iter, next;
	for (iter = list.begin(); iter != list.end(); iter = next) {
		next = iter+1;
		QString line = *iter;

		// The next line could make the current one a header.
		if (next != list.end()) {
			QString nextLine = *next;
			if ( nextLine.startsWith("===") || 
				nextLine.startsWith("~~") || 
				(nextLine.startsWith("--") && nextLine != "----")) {
				defaultstyle = Header;
		       }
		}

		int len = line.size();
		int style = defaultstyle;
		qDebug() << "line =" << line;

		if (line.startsWith("//")) {
			style = Comment;
		} else if (line.startsWith("==") || 
			   line.startsWith("~~") || 
			   (line.startsWith("--") && line != "----")) {
			style = Header;
			defaultstyle = Default;
		} else if (line.startsWith (".")) {
			style = ParaHeader;
		} else if (line == "****"){
			style = SideBarBlock;
			defaultstyle = defaultstyle == SideBarBlock ? Default : SideBarBlock;
		} else if (line == "----"){
			style = ListingBlock;
			defaultstyle = defaultstyle == ListingBlock ? Default : ListingBlock;
		} else if (!line.isEmpty()){
			defaultstyle = styleLine(line, defaultstyle);
			continue;
		}
		qDebug() << "Styling " << len << "bytes " << description(style);
		setStyling(len, style);
		// newline character was consumed in split so...
	 	setStyling(1, style);
	}
}

int AsciiDocLexer::styleLine(QString line, int defaultstyle)
{
	QStringList list = line.split(" ");
	QStringList::iterator iter;

	for (iter = list.begin(); iter != list.end(); iter++) {
		QString word = *iter;
		int len = word.size();
		int style = defaultstyle;

		qDebug() << "word =" << word;
		if (word.startsWith("http://")) { 
			style = URL;
		} else if (word.startsWith("_")) {
			style = ItalicText;
			defaultstyle = style;
		} else if (word.startsWith("*") && word != "*") {
			style = BoldText;
			defaultstyle = style;
		} else if (word.startsWith("+")) {
			style = FixedText;
			defaultstyle = style;
		}

		if (word.endsWith("_") || word.endsWith("_.")) {
			defaultstyle = Default;
		} else if (word.endsWith("*") && word != "*" ) {
			defaultstyle = Default;
		} else if (word.endsWith("+")) {
			defaultstyle = Default;
		}

		qDebug() << "Styling " << len << "bytes " << description(style);
		setStyling(len, (style));
		setStyling(1, (defaultstyle));
	}

	return defaultstyle;
}

