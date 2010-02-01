#ifndef LEXERTEMPLATE_H
#define LEXERTEMPLATE_H

#include <QObject>
#include <qscilexercustom.h>

class QsciStyle;

class MyLexer : public QsciLexerCustom {
	Q_OBJECT

public:
	enum {
		Default = 0,
		Comment,

		MaxStyle
	};

	MyLexer(QObject *parent = 0);
	~MyLexer();
	const char *language() const;
	QString description(int) const;
	void styleText(int start, int end);
	bool pushStyle(int style);
	bool popStyle();
	int getStyle();
	bool hasStyle(int);
	void resetStyle();
	
private:
	QMap<int,QString> styleNames;
#define declareStyle(style,color, paper, font) styleNames[style] = tr(#style); \
		setColor(color, style); \
		setPaper(paper, style); \
		setFont(font, style);
	QList<int> styleStack;

};

#endif
