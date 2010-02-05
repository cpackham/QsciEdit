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
#include <qscilexertex.h>
#include "asciidoclexer.h"
#include "miblexer.h"
#include "lexersel.h"
#include "globals.h"

QList<LexerData> initFileData()
{
	QList<LexerData> list;
	list 
 		<< LexerData("*.c;*.h;*.cpp;*.cc", LexerCPP)
  		<< LexerData("*.java", LexerJava)
  		<< LexerData("*.js", LexerJavaScript)
  		<< LexerData("*.sh;*.bash", LexerBash)
  		<< LexerData("*.mk;Makefile*", LexerMakefile)
  		<< LexerData("*.pro", LexerCMake)
  		<< LexerData("*.py", LexerPython)
  		<< LexerData("*.patch;*.diff;patch.txt", LexerDiff)
  		<< LexerData("*.tcl", LexerTCL)
  		<< LexerData("*.pl;*.pm", LexerPerl)
  		<< LexerData("*.html;*.xhtml", LexerHTML)
  		<< LexerData("*.css", LexerCSS)
  		<< LexerData("*.txt", LexerAsciiDoc)
		<< LexerData("*.tex", LexerTex)
		<< LexerData("*.mib", LexerMib);
 
	return list;
}

QList<LexerData> LexerSelector::lexerInfo(initFileData());

QList<LexerData> initTextData()
{
	QList<LexerData> list;
	list 
  		<< LexerData("/bin/sh;/bin/bash", LexerBash)
  		<< LexerData("/bin/perl;/usr/bin/perl", LexerPerl)
  		<< LexerData("/usr/bin/python", LexerPerl) 
		<< LexerData("\\documentclass", LexerTex);
	return list;
	
}

QList<LexerData> LexerSelector::lexerTextInfo(initTextData());

QMap<QString,LexerID> initStringToId()
{
	QMap<QString,LexerID> map;
	#define addMap(id) map[#id] = id
	
	addMap(LexerNone);
	addMap(LexerCPP);
	addMap(LexerJava);
	addMap(LexerJavaScript);
	addMap(LexerBash);
	addMap(LexerMakefile);
	addMap(LexerCMake);
	addMap(LexerPython);
	addMap(LexerDiff);
	addMap(LexerTCL);
	addMap(LexerPerl);
	addMap(LexerHTML);
	addMap(LexerCSS);
	addMap(LexerAsciiDoc);
	addMap(LexerTex);
	addMap(LexerMib);
	
	#undef addMap
	return map;
}

QMap<QString,LexerID> LexerSelector::stringToIdMap(initStringToId());

QMap<LexerID,QString> initIdToString()
{
	QMap<LexerID,QString> map;
	#define addMap(id) map[id] = #id
	
	addMap(LexerNone);
	addMap(LexerCPP);
	addMap(LexerJava);
	addMap(LexerJavaScript);
	addMap(LexerBash);
	addMap(LexerMakefile);
	addMap(LexerCMake);
	addMap(LexerPython);
	addMap(LexerDiff);
	addMap(LexerTCL);
	addMap(LexerPerl);
	addMap(LexerHTML);
	addMap(LexerCSS);
	addMap(LexerAsciiDoc);
	addMap(LexerTex);
	addMap(LexerMib);
	
	#undef addMap
	return map;
}

QMap<LexerID,QString> LexerSelector::idToStringMap(initIdToString());

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
				// qDebug() << __FUNCTION__  << fileName << " matches" << *iter 
				//	 << " using lexer" << lexerData.id;
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

QsciLexer* LexerSelector::getLexerForText(const QString &text,
		QString *lineCommentString, QString *blockCommentStartString,
		QString *blockCommentMiddleString, QString *blockCommentEndString)
{
	LexerID id;
	QList<LexerData>::iterator l_iter;
	for (l_iter = lexerTextInfo.begin(); l_iter != lexerTextInfo.end(); l_iter++) {

		LexerData lexerData = *l_iter;
		QStringList list = lexerData.pattern.split(";");

		QStringList::iterator iter;
		for( iter = list.begin(); iter != list.end(); iter++){
			if (text.contains(*iter)) {
				// qDebug() << __FUNCTION__  << text << " matches" << *iter
				//	 << " using lexer" << lexerData.id;
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
	case LexerTex:
		lexer = new QsciLexerTeX;
		break;
	case LexerMib:
		lexer = new MibLexer;
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
	case LexerTex:
		*lineCommentString = "% ";
		*blockCommentStartString = "% ";
		*blockCommentMiddleString = "% ";
		*blockCommentEndString = "";
		break;
	case LexerMib:
		*lineCommentString = "-- ";
		*blockCommentStartString = "-- ";
		*blockCommentMiddleString = "-- ";
		*blockCommentEndString = "";		
		break;
	default :
		break;
	}

	return lexer;
}

void LexerSelector::saveLexerSettings()
{
	QSettings settings(COMPANY_NAME, APPLICATION_NAME+"-Lexers");
	QList<LexerData>::iterator iter;
	int i;

	settings.beginWriteArray("FileLexers");
	for (iter = lexerInfo.begin(), i=0;
	     iter != lexerInfo.end(); 
	     iter++, i++) {
		LexerData data(*iter);
		settings.setArrayIndex(i);
		settings.setValue("Pattern", data.pattern);
		settings.setValue("Id", lexerIdToString(data.id));
	}
	settings.endArray();

	settings.beginWriteArray("TextLexers");
	for (iter = lexerTextInfo.begin(), i=0; 
	     iter != lexerTextInfo.end(); 
	     iter++, i++) {
		LexerData data(*iter);
		settings.setArrayIndex(i);
		settings.setValue("Pattern", data.pattern);
		settings.setValue("Id", lexerIdToString(data.id));
	}
	settings.endArray();
}

void LexerSelector::loadLexerSettings()
{
	QSettings settings(COMPANY_NAME, APPLICATION_NAME+"-Lexers");
	QList<LexerData> list;
	int i, size;

	size = settings.beginReadArray("FileLexers");
	for (i = 0; i < size; i++) {
		settings.setArrayIndex(i);
		QString pattern = settings.value("Pattern").toString();
		LexerID id = lexerStringToId(settings.value("Id").toString());
		if (id > LexerNone) {
			LexerData data(pattern, id);
			list.append(data);
		}
	}
	settings.endArray();

	if (size && list.size()) {
		lexerInfo = list;
	}

	list.clear();
	size = settings.beginReadArray("TextLexers");
	for (i = 0; i < size; i++) {
		settings.setArrayIndex(i);
		QString pattern = settings.value("Pattern").toString();
		LexerID id = (LexerID)lexerStringToId(settings.value("Id").toString());
		if (id > LexerNone) {
			LexerData data(pattern, id);
			list.append(data);
		}
	}
	settings.endArray();
	
	if (size && list.size()) {
		lexerTextInfo = list;
	}
}

LexerID LexerSelector::lexerStringToId(QString str)
{
	return stringToIdMap[str];
}

QString LexerSelector::lexerIdToString(LexerID id)
{
	return idToStringMap[id];
}
