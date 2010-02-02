#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

// hmm may be settings isn't the best name for this sucker

class QSettings;
class EditorSettings {
public:
	EditorSettings();

	void load();
	void save();
	bool displayLineNumbers()   { return linenumbers; };
	void setLineNumbers(bool b) { linenumbers = b; };
	bool displayWhitespace()    { return whitespace; }; 
	void setWhitespace( bool b) { whitespace = b; };
	bool displayWrapText()      { return wraptext ; };
	void setWrapText(bool b)    { wraptext = b; };
	bool displayCodeFolding()   { return folding;};
	void setCodeFolding(bool b) { folding = b ;};
	bool displayAutoComplete()  { return autocomplete; };
	void setAutoComplete(bool b){ autocomplete = b;};
	int  autoCompleteThreshold() { return autocompletethresh; };
	void setAutoCompleteThreshold(int i) { autocompletethresh  = i; };
	bool displayBraceMatch()    { return bracematch; };
	void setBraceMatch(bool b)  { bracematch = b; };
	bool highlightCurrentLine() { return highlightcurrent; };
	void setHighlightCurrentLine(bool b) { highlightcurrent = b; };
	bool autoIndent()           { return autoindent; };
	void setAutoIndent(bool b)  { autoindent = b; };
	bool displayEdge()          { return edgeindication; };
	void setDisplayEdge(bool b) { edgeindication = b; };
	int  edgeColumn()           { return edgecol; };
	void setEdgeColumn(int i)   { edgecol = i; };
	QString eolMode()	    { return eolmode; };
	void setEolMode(const QString s) { eolmode = s; };
	bool indentUseTabs()	    { return indenttabs; };
	void setIndentUseTabs(bool b) { indenttabs = b; };
	int  indentWidth()	    { return indentwidth; };
	void setIndentWidth(int i)  { indentwidth = i; };

private:
	bool linenumbers;
	bool whitespace;
	bool wraptext;
	bool folding;
	bool autocomplete;
	int  autocompletethresh;
	bool bracematch;
	bool highlightcurrent;
	bool autoindent;
	bool edgeindication;
	int  edgecol;
	QString eolmode;
	bool indenttabs;
	int  indentwidth;
};


#endif
