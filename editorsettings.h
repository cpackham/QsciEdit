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

private:
	bool linenumbers;
	bool whitespace;
	bool wraptext;
	bool folding;
	bool autocomplete;
	int  autocompletethresh;
	bool bracematch;
	bool highlightcurrent;
};


#endif
