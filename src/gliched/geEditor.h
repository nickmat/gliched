#pragma once

#include <wx/stc/stc.h>

class geEditor : public wxStyledTextCtrl
{
public:
    geEditor(wxWindow* parent);

    bool LoadFile(const wxString& path);
    bool SaveFile(const wxString& path);
    wxString GetFilename() const { return m_filename; }
    void SetFilename(const wxString& path) { m_filename = path; }

private:
    void OnUpdateUI( wxStyledTextEvent& );
    void OnStyleNeeded(wxStyledTextEvent& event);
    void OnMarginClick( wxStyledTextEvent& event );
    void OnCharAdded(wxStyledTextEvent& event);

    wxString m_filename;
};
