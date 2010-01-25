#ifndef ASCIIDOCLEXER_H
#define ASCIIDOCLEXER_H

#include <QObject>
#include <qscilexercustom.h>

class QsciStyle;

class AsciiDocLexer : public QsciLexerCustom {
	Q_OBJECT

public:
	enum {
		Default = 0,
		SideBarBlock,
		BoldText,
		ItalicText,
		FixedText,
		SuperText,
		SubText,
		URL,
		Bullet,
		Header,
		ParaHeader,
		Keyword,
		Comment,
		ListingBlock,

		MaxStyle
	};

	AsciiDocLexer(QObject *parent = 0);
	~AsciiDocLexer();
	const char *language() const;
	QString description(int) const;
	void styleText(int start, int end);
	bool pushStyle(int style);
	bool popStyle();
	int getStyle();
	bool hasStyle(int);
	void resetStyle();
	
private:
	void styleLine(QString);
	QMap<int,QString> styleNames;
#define declareStyle(style,color, paper, font) styleNames[style] = tr(#style); \
		setColor(color, style); \
		setPaper(paper, style); \
		setFont(font, style);
	QList<int> styleStack;
	QList<QString> keywordList;

};

#endif
