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
		Comment,
		Header,
		ParaHeader,
		BoldText,
		ItalicText,
		FixedText,
		URL,
		ListingBlock,
		SideBarBlock,
		MaxStyle
	};

	AsciiDocLexer(QObject *parent = 0);
	~AsciiDocLexer();
	const char *language() const;
	QString description(int) const;
	void styleText(int start, int end);

private:
	int styleLine(QString, int);
	QMap<int,QString> styleNames;
#define declareStyle(style,color, paper, font) styleNames[style] = tr(#style); \
		setColor(color, style); \
		setPaper(paper, style); \
		setFont(font, style);

};

#endif
