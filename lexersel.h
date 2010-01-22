#ifndef LEXERSEL_H
#define LEXERSEL_H

class QsciLexer;

enum LexerID {
	LexerNone = 0,
	LexerCPP,
	LexerJava,
	LexerJavaScript,
	LexerBash,
	LexerMakefile,
	LexerCMake,
	LexerPython,
	LexerDiff,
	LexerTCL,
	LexerPerl,
	LexerHTML,
	LexerCSS,
	LexerAsciiDoc
};

class LexerData {
public:
	QString pattern;
	LexerID id;

	LexerData(QString s, LexerID i)
		:pattern(s),id(i)
	{
	};
};

class LexerSelector {

public:
	static QsciLexer* getLexerForFile(const QString &fileName,
		QString *lineCommentString,
		QString *blockCommentStartString,
		QString *blockCommentMiddleString,
		QString *blockCommentEndString);

	static QsciLexer* getLexerForText(const QString &text,
		QString *lineCommentString,
		QString *blockCommentStartString,
		QString *blockCommentMiddleString,
		QString *blockCommentEndString);

	static QsciLexer* getLexerById(int id,
		QString *lineCommentString,
		QString *blockCommentStartString,
		QString *blockCommentMiddleString,
		QString *blockCommentEndString);

	static void saveLexerSettings();
	static void loadLexerSettings();

private:
	static QList<LexerData> lexerInfo;
	static QList<LexerData> lexerTextInfo;
};

#endif
