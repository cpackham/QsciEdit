#include <QDebug>
#include <QColor>
#include <QFont>
#include <qsciscintilla.h>
#include <qscistyle.h>
#include "lexertemplate.h"

MyLexer::MyLexer(QObject *parent)
	: QsciLexerCustom(parent)
{
	styleStack = QList<int>();
	resetStyle ();
	
	declareStyle(Default,
		QColor(0x0, 0x0, 0x0),
		QColor("white"),
		QFont("Liberation Mono", 10));
	declareStyle(Comment,
		QColor(0x0, 0x80, 0x0),
		QColor("white"),
		QFont("Liberation Mono", 10));
}

MyLexer::~MyLexer()
{
	qDebug() << __FUNCTION__;
}

const char* MyLexer::language() const
{
	return "MyLexer";
}

QString MyLexer::description(int style) const
{
	if (styleNames.contains(style)) {
		return styleNames[style];
	} else {
		return QString("");
	}
}

void MyLexer::styleText(int start, int end)
{
	QString source;
	int len;
	
	if (!editor())
		return;
	
	resetStyle ();

	char *chars = new char[(end - start) + 1];
	editor()->SendScintilla(QsciScintilla::SCI_GETTEXTRANGE, start, end, chars);
	source = QString(chars);
	delete [] chars;
	
	len = end - start;

// 	qDebug() << "source =" << source;
	
	startStyling(start, 0x1f);
	setStyling(len, getStyle());
}

bool MyLexer::pushStyle(int style)
{
	bool pushed = false;
	
	if (style >= getStyle()) {
		styleStack << style;
		pushed = true;
// 		qDebug() << __FUNCTION__ << description(style);
	}
	return pushed;
}

bool MyLexer::popStyle()
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

int MyLexer::getStyle()
{
	int style;
	style = styleStack.at(styleStack.size()-1) ;
	return style;
}

bool MyLexer::hasStyle(int style)
{
	return styleStack.contains(style);
}

void MyLexer::resetStyle()
{
	styleStack.clear();
	styleStack << Default;
}

