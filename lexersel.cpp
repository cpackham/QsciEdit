#include <iostream>
#include <QtGui>
#include <qsciscintilla.h>
#include <qscilexer.h>
#include <qscilexercpp.h>
#include <qscilexerjava.h>
#include <qscilexerjavascript.h>
#include <qscilexerbash.h>
#include <qscilexermakefile.h>
#include <qscilexercmake.h>
#include <qscilexerpython.h>
#include <qscilexerperl.h>
#include <qscilexercss.h>
#include <qscilexerhtml.h>
#include <qscilexerdiff.h>
#include <qscilexertcl.h>
#include "asciidoclexer.h"
#include "lexersel.h"

QList<LexerData> initFileData()
{
	QList<LexerData> list;
	list 
 		<< LexerData("*.c;*.h;*.cpp;*.cc", LexerCPP)
  		<< LexerData("*.java", LexerJava)
  		<< LexerData("*.js", LexerJavaScript)
  		<< LexerData("*.sh;*.bash;", LexerBash)
  		<< LexerData("*.mk;Makefile*;", LexerMakefile)
  		<< LexerData("*.pro", LexerCMake)
  		<< LexerData("*.py", LexerPython)
  		<< LexerData("*.patch;*.diff;patch.txt;", LexerDiff)
  		<< LexerData("*.tcl", LexerTCL)
  		<< LexerData("*.pl;*.pm", LexerPerl)
  		<< LexerData("*.html;*.xhtml", LexerHTML)
  		<< LexerData("*.css", LexerCSS)
  		<< LexerData("*.txt;", LexerAsciiDoc);
 
	return list;
}

QList<LexerData> LexerSelector::lexerInfo(initFileData());

QsciLexer* LexerSelector::getLexerForFile(const QString &fileName,
		QString *lineCommentString, QString *blockCommentStartString,
		QString *blockCommentMiddleString, QString *blockCommentEndString)
{
	LexerID id;
	QList<LexerData>::iterator l_iter;
	for (l_iter = lexerInfo.begin(); l_iter != lexerInfo.end(); l_iter++) {

		LexerData lexerData = *l_iter;
		QStringList list = lexerData.pattern.split(";");

		QStringList::iterator iter;
		for( iter = list.begin(); iter != list.end(); iter++){

			QRegExp rx(*iter);
			rx.setPatternSyntax(QRegExp::Wildcard);

			if (rx.exactMatch(fileName)) {
				qDebug() << __FUNCTION__  << fileName << " matches" << *iter;
				id = lexerData.id;
				return getLexerById(id,lineCommentString,
					blockCommentStartString, 
					blockCommentMiddleString, 
					blockCommentEndString);
			}
		}
	}

	return NULL;
}

QsciLexer* LexerSelector::getLexerById(int id,
		QString *lineCommentString, QString *blockCommentStartString,
		QString *blockCommentMiddleString, QString *blockCommentEndString)
{
	QsciLexer *lexer = NULL;

	switch (id) {
	case LexerCPP:
		lexer = new QsciLexerCPP;
		break;
	case LexerJava:
		lexer = new QsciLexerJava;
		break;
	case LexerJavaScript:
		lexer = new QsciLexerJavaScript;
		break;
	case LexerBash:
		lexer = new QsciLexerBash;
		break;
	case LexerMakefile:
		lexer = new QsciLexerMakefile;
		break;
	case LexerCMake:
		lexer = new QsciLexerCMake;
		break;
	case LexerPython:
		lexer = new QsciLexerPython;
		break;
	case LexerDiff:
		lexer = new QsciLexerDiff;
		break;
	case LexerTCL:
		lexer = new QsciLexerTCL;
		break;
	case LexerPerl:
		lexer = new QsciLexerPerl;
		break;
	case LexerHTML:
		lexer = new QsciLexerHTML;
		break;
	case LexerCSS:
		lexer = new QsciLexerCSS;
		break;
	case LexerAsciiDoc:
		lexer = new AsciiDocLexer;
		break;
	default :
		break;
	}

	switch (id) {
	case LexerCPP:
	case LexerJava:
	case LexerJavaScript:
	case LexerCSS:
		*lineCommentString = "// ";
		*blockCommentStartString = "/* ";
		*blockCommentMiddleString = "";
		*blockCommentEndString = " */";
		break;
	case LexerBash:
	case LexerMakefile:
	case LexerPython:
	case LexerPerl:
	case LexerCMake:
	case LexerTCL:
		*lineCommentString = "# ";
		*blockCommentStartString = "# ";
		*blockCommentMiddleString = "# ";
		*blockCommentEndString = "";
		break;
		break;
	case LexerHTML:
		*lineCommentString = "";
		*blockCommentStartString = "<!-- ";
		*blockCommentMiddleString = "";
		*blockCommentEndString = " -->";
		break;
	case LexerAsciiDoc:
		*lineCommentString = "// ";
		*blockCommentStartString = "// ";
		*blockCommentMiddleString = "// ";
		*blockCommentEndString = "";
		break;
	default :
		break;
	}

	return lexer;
}

