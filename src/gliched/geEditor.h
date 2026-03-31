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
    wxString GetTabName() const { return m_tabName; }
    void SetTabName( const wxString& name ) { m_tabName = name; }
    bool IsRunPage() const { return m_runPage; }
    void SetRunPage( bool run ) { m_runPage = run; }

private:
    void OnUpdateUI( wxStyledTextEvent& );
    void OnStyleNeeded(wxStyledTextEvent& event);
    void OnMarginClick( wxStyledTextEvent& event );
    void OnCharAdded(wxStyledTextEvent& event);

    wxString m_filename;
    wxString m_tabName;
    bool m_runPage;
};
