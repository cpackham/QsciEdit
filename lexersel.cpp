#include <iostream>
#include <QtGui>
#include <qsciscintilla.h>
#include <qscilexer.h>
#include <qscilexercpp.h>
#include <qscilexerbash.h>
#include <qscilexermakefile.h>
#include <qscilexercmake.h>
#include <qscilexerpython.h>
#include <qscilexerperl.h>
#include <qscilexercss.h>
#include <qscilexerhtml.h>
#include <qscilexerdiff.h>
#include <qscilexertcl.h>
#include "lexersel.h"

QsciLexer* LexerSelector::getLexerForFile(const QString &fileName)
{
	QFileInfo info = QFileInfo(fileName);
	QString basename = info.baseName();
	QString suffix = info.suffix();

	if (suffix == "c" || suffix == "cpp" || suffix == "cc" || suffix == "h")
		return new QsciLexerCPP;
	else if (suffix == "sh" || suffix == "bash")
		return new QsciLexerBash;
	else if (suffix == "mk" || basename == "Makefile")
		return new QsciLexerMakefile;
	else if (suffix == "pro")
		return new QsciLexerCMake;
	else if (suffix == "py")
		return new QsciLexerPython;
	else if (suffix == "patch" || suffix == "diff")
		return new QsciLexerDiff;
	else if (suffix == "tcl")
		return new QsciLexerTCL;
	else if (suffix == "pl" || suffix == "pm")
		return new QsciLexerPerl;
	else if (suffix == "html" || suffix == "htm")
		return new QsciLexerHTML;
	else if (suffix == "css")
		return new QsciLexerCSS;

	return NULL;
}
