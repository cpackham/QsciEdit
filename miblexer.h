#ifndef MIBLEXER_H
#define MIBLEXER_H

#include <QObject>
#include <qscilexercustom.h>

class QsciStyle;

class MibLexer : public QsciLexerCustom {
	Q_OBJECT

public:
	enum {
		Default = 0,
		String,
		Keyword,
		Type,
		Access,
		Comment,
		OID,

		MaxStyle
	};

	MibLexer(QObject *parent = 0);
	~MibLexer();
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
	QStringList keywords;
	QStringList accesses;
	QStringList types;

	void initKeywords();
	void initAccesses();
	void initTypes();
	void styleLine(QString);

};

#endif
