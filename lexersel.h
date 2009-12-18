#ifndef LEXERSEL_H
#define LEXERSEL_H

class QsciLexer;

class LexerSelector {

public:
	static QsciLexer* getLexerForFile(const QString &fileName);
};

#endif
