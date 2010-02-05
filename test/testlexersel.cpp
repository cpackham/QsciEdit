#include <QtTest/QtTest>
#include <qscilexer.h>
#include "lexersel.h"

class TestLexerSelector: public QObject
{
	Q_OBJECT

private slots:
	void getLexerForFile_data();
	void getLexerForFile();
	void getLexerForText_data();
	void getLexerForText();
};

void TestLexerSelector::getLexerForFile_data()
{
	QTest::addColumn<QString>("filename");
	QTest::addColumn<QString>("linecomment");

	QTest::newRow("C++ file 1") << "main.cpp" << "// ";
	QTest::newRow("C++ file 2") << "main.cc" << "// ";
	QTest::newRow("C file") << "main.c" << "// ";
	QTest::newRow("H file") << "main.h" << "// ";
	QTest::newRow("Java file") << "main.java" << "// ";
	QTest::newRow("Javascript file") << "main.js" << "// ";
	QTest::newRow("Bash file") << "main.bash" << "# ";
	QTest::newRow("sh file") << "main.sh" << "# ";
	QTest::newRow("Makefile 1") << "Makefile" << "# ";
	QTest::newRow("Makefile 2") << "main.mk" << "# ";
	QTest::newRow("Makefile 3") << "main.pro" << "# ";
	QTest::newRow("Python file") << "main.py" << "# ";
	QTest::newRow("Patch file 1") << "main.patch" << "";
	QTest::newRow("Patch file 2") << "main.diff" << "";
	QTest::newRow("Patch file 3") << "patch.txt" << "";
	QTest::newRow("TCL file 1") << "main.tcl" << "# ";
	QTest::newRow("Perl file 1") << "main.pl" << "# ";
	QTest::newRow("Perl file 2") << "main.pm" << "# ";
	QTest::newRow("HTML file 1") << "main.html" << "";
	QTest::newRow("HTML file 2") << "main.xhtml" << "";
	QTest::newRow("CSS file") << "main.css" << "// ";
	QTest::newRow("Asciidoc file") << "main.txt" << "// ";
	QTest::newRow("TeX file") << "main.tex" << "% ";
	QTest::newRow("MIB file") << "main.mib" << "-- ";

}

void TestLexerSelector::getLexerForFile()
{
	QString lineCommentString = "", blockCommentStartString = "",
		blockCommentMiddleString = "", blockCommentEndString = "";

	QsciLexer *lexer = NULL;

	QFETCH(QString, filename);
	QFETCH(QString, linecomment);

	lexer = LexerSelector::getLexerForFile(filename, &lineCommentString,
		&blockCommentStartString, 
		&blockCommentMiddleString, 
		&blockCommentEndString);

	QVERIFY(lexer != NULL);
	QCOMPARE(lineCommentString, linecomment);
}

void TestLexerSelector::getLexerForText_data()
{
	QTest::addColumn<QString>("text");
	QTest::addColumn<QString>("linecomment");

	QTest::newRow("Shell script 1") << "#!/bin/sh" << "# ";
	QTest::newRow("Shell script 2") << "#! /bin/sh" << "# ";
	QTest::newRow("BASH script 1") << "#!/bin/bash" << "# ";
	QTest::newRow("BASH script 2") << "#! /bin/bash" << "# ";
	QTest::newRow("Python script 1") << "#!/usr/bin/python" << "# ";
	QTest::newRow("Python script 2") << "#! /usr/bin/python" << "# ";
	QTest::newRow("TeX file 1") << "\\documentclass"  << "% ";
	QTest::newRow("TeX file 2") << "\\documentclass{article}"  << "% ";
	QTest::newRow("TeX file 3") << "\\documentclass[some options]{article}"  << "% ";
}

void TestLexerSelector::getLexerForText()
{
	QString lineCommentString = "", blockCommentStartString = "",
		blockCommentMiddleString = "", blockCommentEndString = "";

	QsciLexer *lexer = NULL;

	QFETCH(QString, text);
	QFETCH(QString, linecomment);

	lexer = LexerSelector::getLexerForText(text, &lineCommentString,
		&blockCommentStartString, 
		&blockCommentMiddleString, 
		&blockCommentEndString);

	QVERIFY(lexer != NULL);
	QCOMPARE(lineCommentString, linecomment);
}

QTEST_MAIN(TestLexerSelector)
#include "testlexersel.moc"