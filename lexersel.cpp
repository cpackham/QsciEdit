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

QsciLexer* LexerSelector::getLexerForFile(const QString &fileName,
		QString *lineCommentString, QString *blockCommentStartString,
		QString *blockCommentMiddleString, QString *blockCommentEndString)
{
	QFileInfo info = QFileInfo(fileName);
	QString basename = info.baseName();
	QString suffix = info.suffix();

	if (suffix == "c" || suffix == "cpp" ||
	    suffix == "cc" || suffix == "h") {
		*lineCommentString = "// ";
		*blockCommentStartString = "/* ";
		*blockCommentMiddleString = "";
		*blockCommentEndString = " */";
		return new QsciLexerCPP;
	} else if (suffix == "sh" || suffix == "bash") {
		*lineCommentString = "# ";
		*blockCommentStartString = "# ";
		*blockCommentMiddleString = "# ";
		*blockCommentEndString = "";
		return new QsciLexerBash;
	} else if (suffix == "mk" || basename == "Makefile") {
		*lineCommentString = "# ";
		*blockCommentStartString = "# ";
		*blockCommentMiddleString = "# ";
		*blockCommentEndString = "";
		return new QsciLexerMakefile;
	} else if (suffix == "pro") {
		*lineCommentString = "# ";
		*blockCommentStartString = "# ";
		*blockCommentMiddleString = "# ";
		*blockCommentEndString = "";
		return new QsciLexerCMake;
	} else if (suffix == "py") {
		*lineCommentString = "# ";
		*blockCommentStartString = "# ";
		*blockCommentMiddleString = "# ";
		*blockCommentEndString = "";
		return new QsciLexerPython;
	} else if (suffix == "patch" || suffix == "diff") {
		return new QsciLexerDiff;
	} else if (suffix == "tcl") {
		*lineCommentString = "# ";
		*blockCommentStartString = "# ";
		*blockCommentMiddleString = "# ";
		*blockCommentEndString = "";
		return new QsciLexerTCL;
	} else if (suffix == "pl" || suffix == "pm") {
		*lineCommentString = "# ";
		*blockCommentStartString = "# ";
		*blockCommentMiddleString = "# ";
		*blockCommentEndString = "";
		return new QsciLexerPerl;
	} else if (suffix == "html" || suffix == "htm") {
		*lineCommentString = "";
		*blockCommentStartString = "<!-- ";
		*blockCommentMiddleString = "";
		*blockCommentEndString = " -->";
		return new QsciLexerHTML;
	} else if (suffix == "css") {
		*lineCommentString = "// ";
		*blockCommentStartString = "/* ";
		*blockCommentMiddleString = "";
		*blockCommentEndString = " */";
		return new QsciLexerCSS;
	}

	return NULL;
}

