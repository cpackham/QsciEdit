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
		QColor(0xe0, 0x0, 0x0),
		QColor("white"),
		QFont("Monospaced, Courier", 10));
	declareStyle(ParaHeader,
		QColor(0x80, 0x0, 0x0),
		QColor(0xff, 0x80, 0x80),
		QFont("Monospaced, Courier", 10));
	declareStyle(BoldText,
		QColor(0xe0, 0x0, 0x0),
		QColor("white"),
		QFont("Monospaced, Courier", 10));
	declareStyle(ItalicText,
		QColor(0xe0, 0x0, 0x0),
		QColor("white"),
		QFont("Monospaced, Courier", 10));
	declareStyle(FixedText,
		QColor(0xe0, 0x0, 0x0),
		QColor("white"),
		QFont("Monospaced, Courier", 10));
	declareStyle(URL,
		QColor(0x0, 0x0, 0x80),
		QColor("white"),
		QFont("Monospaced, Courier", 10));
	declareStyle(ListingBlock,
		QColor(0x0, 0x0, 0x40),
		QColor("white"),
		QFont("Monospaced, Courier", 10));
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
	int i;

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
	int default_style = Default;
	for (i = 0; i < list.size(); i++) {
		QString line = list.at(i);
		int len = line.size();
		int style = default_style;
		qDebug() << "line =" << line;

		if (line.startsWith("//")) {
			style = Comment;
		} else if (line.startsWith("==") || 
			   line.startsWith("~~") || 
			   (line.startsWith("--") && line != "----")) {
			style = Header;
		} else if (line.startsWith (".")) {
			style = ParaHeader;
		} else if (line == "****"){
			style = SideBarBlock;
			default_style = default_style == SideBarBlock ? Default : SideBarBlock;
		} else if (line == "----"){
			style = ListingBlock;
			default_style = default_style == ListingBlock ? Default : ListingBlock;
		} else if (!line.isEmpty()){
			default_style = styleLine(line, default_style);
			continue;
		}
		qDebug() << "Styling " << len << "bytes " << description(style);
		setStyling(len, style);
		// newline character was consumed in split so...
	 	setStyling(1, Default);
	}
}

int AsciiDocLexer::styleLine(QString line, int defaultstyle)
{
	int i;
	QStringList list = line.split(" ");

	for (i = 0; i < list.size(); i++) {
		QString word = list.at(i);
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
		} else if (word.endsWith("*")) {
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

