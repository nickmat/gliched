#include "geEditor.h"

geEditor::geEditor(wxWindow* parent)
    : wxStyledTextCtrl(parent, wxID_ANY)
{
    SetLexer(wxSTC_LEX_CONTAINER);
    StyleSetFont(wxSTC_STYLE_DEFAULT, wxFont(11, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    SetTabWidth(4);
    SetUseTabs(false);
    StyleClearAll();
}

bool geEditor::LoadFile(const wxString& path)
{
    if (wxStyledTextCtrl::LoadFile(path))
    {
        m_filename = path;
        return true;
    }
    return false;
}

bool geEditor::SaveFile(const wxString& path)
{
    if (wxStyledTextCtrl::SaveFile(path))
    {
        m_filename = path;
        return true;
    }
    return false;
}
