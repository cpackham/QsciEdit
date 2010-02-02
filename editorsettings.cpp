#include <QtGui>
#include "globals.h"
#include "editorsettings.h"

EditorSettings::EditorSettings()
	: linenumbers(false),
	  whitespace(false),
	  wraptext(false),
	  folding(false),
	  autocomplete(false),
	  autocompletethresh(3)
{
	
}

void EditorSettings::load()
{
	QSettings qsettings(COMPANY_NAME, APPLICATION_NAME);
	qsettings.beginGroup("editor settings");
	setLineNumbers(qsettings.value("line numbers",false).toBool());
	setWhitespace(qsettings.value("whitespace", false).toBool());
	setWrapText(qsettings.value("wrap text", false).toBool());
	setCodeFolding(qsettings.value("folding", false).toBool());
	setBraceMatch(qsettings.value("brace matching", false).toBool());
	setAutoComplete(qsettings.value("auto completion", false).toBool());
	setAutoCompleteThreshold(qsettings.value("auto completion threshold", 3).toInt());
	setHighlightCurrentLine(qsettings.value("HighlightCurrentLine", false).toBool());
	setAutoIndent(qsettings.value("AutoIndent", false).toBool());
	setDisplayEdge(qsettings.value("DisplayEdge", false).toBool());
	setEdgeColumn(qsettings.value("EdgeColumn", 80).toInt());
	setEolMode(qsettings.value("EolMode", "").toString());
	setIndentUseTabs(qsettings.value("IndentUseTabs", true).toBool());
	setIndentWidth(qsettings.value("IndentWidth", 8).toInt());
	qsettings.endGroup();
}

void EditorSettings::save()
{
	QSettings qsettings(COMPANY_NAME, APPLICATION_NAME);
	qsettings.beginGroup("editor settings");
	qsettings.setValue("line numbers", displayLineNumbers());
	qsettings.setValue("whitespace", displayWhitespace());
	qsettings.setValue("wrap text", displayWrapText());
	qsettings.setValue("folding", displayCodeFolding());
	qsettings.setValue("brace matching", displayBraceMatch());
	qsettings.setValue("auto completion",displayAutoComplete());
	qsettings.setValue("auto completion threshold",autoCompleteThreshold());
	qsettings.setValue("HighlightCurrentLine", highlightCurrentLine());
	qsettings.setValue("AutoIndent", autoIndent());
	qsettings.setValue("DisplayEdge", displayEdge());
	qsettings.setValue("EdgeColumn", edgeColumn());
	qsettings.setValue("EolMode", eolMode());
	qsettings.setValue("IndentUseTabs", indentUseTabs());
	qsettings.setValue("IndentWidth", indentWidth());
	qsettings.endGroup();
}
