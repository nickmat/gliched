#include "geEditor.h"
#include "geMainFrame.h"
#include <wx/stc/stc.h>
#include <wx/log.h> 
#include <unordered_set>

static const std::unordered_set<std::string> glich_keywords = {
    "let", "global", "constant", "function", "result", "this" "command", "object",
    "file", "write", "nl", "mark", "call", "set", "module",
    "if", "else", "elseif",
    "do", "in", "in:r", "while", "until", "exit",
    "true", "false", "null", "empty", "infinity", "inf", "nan",
    "and", "or", "not",
    "grammar", "format", "lexicon",
    "past", "future", "today"
};

enum {
    STYLE_DEFAULT = 0,
    STYLE_COMMENT = 1,
    STYLE_NUMBER = 2,
    STYLE_STRING = 3,
    STYLE_KEYWORD = 4,
    STYLE_IDENTIFIER = 5,
    STYLE_OPERATOR = 6
};

geEditor::geEditor(wxWindow* parent)
    : wxStyledTextCtrl( parent, wxID_ANY ), m_runPage( false )
{
    SetLexer(wxSTC_LEX_CONTAINER);
    StyleSetFont(wxSTC_STYLE_DEFAULT, wxFont(11, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    SetTabWidth(4);
    SetUseTabs(false);
    StyleClearAll();

    // Style colours
    StyleSetForeground( STYLE_DEFAULT, wxColour( 0, 0, 0 ) );
    StyleSetForeground( STYLE_COMMENT, wxColour( 0, 128, 0 ) );
    StyleSetForeground( STYLE_NUMBER, wxColour( 0, 0, 255 ) );
    StyleSetForeground( STYLE_STRING, wxColour( 163, 21, 21 ) );
    StyleSetForeground( STYLE_KEYWORD, wxColour( 0, 0, 192 ) );
    StyleSetBold( STYLE_KEYWORD, true );
    StyleSetForeground( STYLE_IDENTIFIER, wxColour( 0, 0, 0 ) );
    StyleSetForeground( STYLE_OPERATOR, wxColour( 128, 0, 64 ) );

    // Line numbers
    const int lineNumMargin = 0;
    int lineNumWidth = TextWidth(wxSTC_STYLE_LINENUMBER, "_99999");
    SetMarginType(lineNumMargin, wxSTC_MARGIN_NUMBER);
    SetMarginWidth(lineNumMargin, lineNumWidth);
    StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(80, 80, 80));
    StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour(240, 240, 240));

    // Folding margin
    const int foldMargin = 1;
    SetMarginType(foldMargin, wxSTC_MARGIN_SYMBOL);
    SetMarginMask(foldMargin, 0xFE000000);
    SetMarginWidth(foldMargin, 16);
    SetMarginSensitive(foldMargin, true);

    // Folding markers
    MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_ARROW, *wxBLACK, wxColour(200, 200, 200));         // closed: >
    MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_ARROWDOWN, *wxBLACK, wxColour(200, 200, 200)); // open: v
    MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_VLINE, *wxBLACK, wxColour(200, 200, 200));
    MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_LCORNER, *wxBLACK, wxColour(200, 200, 200));
    MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_ARROW, *wxBLACK, wxColour(200, 200, 200));
    MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN, *wxBLACK, wxColour(200, 200, 200));
    MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_TCORNER, *wxBLACK, wxColour(200, 200, 200));

    SetProperty("fold", "1");
    SetProperty("fold.compact", "1");

    SetIndentationGuides(wxSTC_IV_LOOKBOTH);
    SetCaretLineVisible(true);
    SetCaretLineBackground(wxColour(235, 245, 255));

    Bind(wxEVT_STC_UPDATEUI, &geEditor::OnUpdateUI, this);
    Bind(wxEVT_STC_STYLENEEDED, &geEditor::OnStyleNeeded, this);
    Bind( wxEVT_STC_MARGINCLICK, &geEditor::OnMarginClick, this );
    Bind(wxEVT_STC_CHARADDED, &geEditor::OnCharAdded, this);
    Bind( wxEVT_STC_CHANGE, &geEditor::OnContentChanged, this );
    m_autosaveTimer.Bind( wxEVT_TIMER, &geEditor::OnAutosaveTimer, this );
}

bool geEditor::LoadFile( const wxString& path )
{
    if( wxStyledTextCtrl::LoadFile( path ) ) {
        m_filename = path;
        m_tabName = wxFileNameFromPath( path );
        return true;
    }
    return false;
}

bool geEditor::SaveFile( const wxString& path )
{
    if( wxStyledTextCtrl::SaveFile( path ) ) {
        m_filename = path;
        m_tabName = wxFileNameFromPath( path );
        return true;
    }
    return false;
}

// Helper: Returns true if inside a /* ... */ comment at the given position
static bool IsInMultilineComment(int pos, wxStyledTextCtrl* ctrl)
{
    bool inComment = false;
    int i = 0;
    while (i < pos) {
        char c = ctrl->GetCharAt(i);
        if (!inComment && c == '/' && ctrl->GetCharAt(i + 1) == '*') {
            inComment = true;
            i += 2;
            continue;
        }
        if (inComment && c == '*' && ctrl->GetCharAt(i + 1) == '/') {
            inComment = false;
            i += 2;
            continue;
        }
        ++i;
    }
    return inComment;
}

// Folding logic for { ... } blocks
void geEditor::OnStyleNeeded(wxStyledTextEvent& event)
{
    // Folding logic.
    int lineCount = GetLineCount();
    int level = wxSTC_FOLDLEVELBASE;

    for( int line = 0; line < lineCount; ++line ) {
        wxString text = GetLine( line );

        // Count braces in this line
        int opens = 0, closes = 0;
        for( wxChar ch : text ) {
            if( ch == '{' ) ++opens;
            if( ch == '}' ) ++closes;
        }

        // Set fold level for this line
        int thisLevel = level;
        int nextLevel = level + opens - closes;

        int flags = thisLevel;
        if( opens > 0 ) {
            flags |= wxSTC_FOLDLEVELHEADERFLAG;
        }

        SetFoldLevel( line, flags );

        // The next line's level is increased if a block is opened
        level = thisLevel + opens - closes;
        if( level < wxSTC_FOLDLEVELBASE ) {
            level = wxSTC_FOLDLEVELBASE;
        }
    }

    // Syntax colouring.
    int startPos = GetEndStyled();
    int endPos = event.GetPosition();
    StartStyling(startPos);

    int length = GetTextLength();
    int pos = startPos;

    // Fix: determine if we are inside a multi-line comment at startPos
    bool inComment = IsInMultilineComment(startPos, this);

    while (pos < endPos && pos < length) {
        char c = GetCharAt(pos);

        // Multi-line comment start or inside comment
        if (!inComment && c == '/' && GetCharAt(pos + 1) == '*') {
            int start = pos;
            pos += 2;
            inComment = true;
            while (pos < endPos && pos < length) {
                if (GetCharAt(pos) == '*' && GetCharAt(pos + 1) == '/') {
                    pos += 2;
                    inComment = false;
                    break;
                }
                ++pos;
            }
            SetStyling(pos - start, STYLE_COMMENT);
            continue;
        }
        if (inComment) {
            int start = pos;
            while (pos < endPos && pos < length) {
                if (GetCharAt(pos) == '*' && GetCharAt(pos + 1) == '/') {
                    pos += 2;
                    inComment = false;
                    break;
                }
                ++pos;
            }
            SetStyling(pos - start, STYLE_COMMENT);
            continue;
        }

        // Single-line comment (// to end of line)
        if (c == '/' && GetCharAt(pos + 1) == '/') {
            int lineEnd = PositionFromLine(LineFromPosition(pos) + 1);
            if (lineEnd == wxNOT_FOUND) lineEnd = length;
            int n = lineEnd - pos;
            SetStyling(n, STYLE_COMMENT);
            pos += n;
            continue;
        }

        // String
        if (c == '"') {
            int start = pos;
            ++pos;
            while (pos < endPos && pos < length) {
                char d = GetCharAt(pos);
                if (d == '"' && GetCharAt(pos + 1) == '"') { // Escaped quote
                    pos += 2;
                } else if (d == '"') {
                    ++pos;
                    break;
                } else {
                    ++pos;
                }
            }
            SetStyling(pos - start, STYLE_STRING);
            continue;
        }

        // Number
        if (isdigit(c)) {
            int start = pos;
            while (pos < endPos && isdigit(GetCharAt(pos))) ++pos;
            SetStyling(pos - start, STYLE_NUMBER);
            continue;
        }

        // Identifier (allowing : and _)
        if (isalpha(c) || c == '_' || c == ':') {
            int start = pos;
            while (pos < endPos) {
                char d = GetCharAt(pos);
                if (isalnum(d) || d == '_' || d == ':')
                    ++pos;
                else
                    break;
            }
            wxString ident = GetTextRange(start, pos);
            std::string identStr = ident.ToStdString();
            int style = glich_keywords.count(identStr) ? STYLE_KEYWORD : STYLE_IDENTIFIER;
            SetStyling(pos - start, style);
            continue;
        }

        // Operator
        if (strchr("+-*/%&|^=<>!.,;()[]{}", c)) {
            SetStyling(1, STYLE_OPERATOR);
            ++pos;
            continue;
        }

        // Whitespace or default
        SetStyling(1, STYLE_DEFAULT);
        ++pos;
    }
}

void geEditor::OnMarginClick( wxStyledTextEvent& event )
{
    if( event.GetMargin() == 1 )
    {
        int line = LineFromPosition( event.GetPosition() );
        int level = GetFoldLevel( line );

        if( level & wxSTC_FOLDLEVELHEADERFLAG )
            ToggleFold( line );
    }
}

// Smart indentation
void geEditor::OnCharAdded(wxStyledTextEvent& event)
{
    if (event.GetKey() == '\n') {
        int line = GetCurrentLine();
        if (line == 0) return;
        wxString prevLine = GetLine(line - 1);
        wxString indent;

        // Copy indentation from previous line
        for (wxChar ch : prevLine) {
            if (ch == ' ' || ch == '\t') {
                indent += ch;
            } else {
                break;
            }
        }

        // Increase indent if previous line ends with '{'
        wxString trimmedPrev = prevLine.Trim(true).Trim(false);
        if (trimmedPrev.EndsWith("{")) {
            if (GetUseTabs())
                indent += '\t';
            else
                indent += wxString(' ', GetTabWidth());
        }

        InsertText(GetCurrentPos(), indent);
        GotoPos(PositionFromLine(line) + indent.Length());
    }
    // --- Auto-unindent when typing '}' at start of line ---
    else if (event.GetKey() == '}') {
        int pos = GetCurrentPos();
        int line = LineFromPosition(pos);
        wxString currLine = GetLine(line);
        int lineStart = PositionFromLine(line);
        int caretPosInLine = pos - lineStart;

        // Check if only whitespace before the caret
        bool onlyWhitespace = true;
        for (int i = 0; i < caretPosInLine - 1; ++i) {
            if (!isspace(GetCharAt(lineStart + i))) {
                onlyWhitespace = false;
                break;
            }
        }
        if (onlyWhitespace) {
            // Remove one indent level before the caret
            int tabWidth = GetTabWidth();
            int removeCount = 0;
            for (int i = 0; i < caretPosInLine - 1 && removeCount < tabWidth; ++i) {
                char ch = GetCharAt(lineStart + i);
                if (ch == ' ') removeCount++;
                else if (ch == '\t') removeCount = tabWidth; // Remove all if tab
                else break;
            }
            if (removeCount > 0) {
                SetTargetStart(lineStart);
                SetTargetEnd(lineStart + removeCount);
                ReplaceTarget("");
                GotoPos(pos - removeCount);
            }
        }
    }

    // --- Force syntax coloring for the current line after typing ---
    int line = GetCurrentLine();
    int start = PositionFromLine( line );
    int end = GetLineEndPosition( line );
    Colourise( start, end );
}

// Brace matching (unchanged)
void geEditor::OnUpdateUI(wxStyledTextEvent&)
{
    int pos = GetCurrentPos();
    int braceAtCaret = -1;
    int braceOpposite = -1;

    char ch = (pos > 0) ? GetCharAt(pos - 1) : 0;
    if (ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '[' || ch == ']')
        braceAtCaret = pos - 1;
    else
        braceAtCaret = pos;

    braceOpposite = BraceMatch(braceAtCaret);
    if (braceOpposite != wxSTC_INVALID_POSITION)
        BraceHighlight(braceAtCaret, braceOpposite);
    else
        BraceBadLight(braceAtCaret);
}

void geEditor::OnContentChanged( wxStyledTextEvent& )
{
    // Only start timer if autosave is enabled (ask main frame)
    if( m_filename.IsEmpty() ) return;
    geMainFrame* frame = dynamic_cast<geMainFrame*>(wxGetTopLevelParent( this ));
    if( frame && frame->IsAutosaveEnabled() ) {
        m_autosaveTimer.Start( 1000, wxTIMER_ONE_SHOT ); // 1 second debounce
    }
}

void geEditor::OnAutosaveTimer( wxTimerEvent& )
{
    if( !m_filename.IsEmpty() ) {
        SaveFile( m_filename );
    }
}
