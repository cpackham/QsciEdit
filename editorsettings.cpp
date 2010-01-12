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
	qsettings.endGroup();
}
