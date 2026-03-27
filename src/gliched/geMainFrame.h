#pragma once

#include <wx/frame.h>
#include <wx/aui/aui.h>
#include <wx/aui/auibook.h>
#include <wx/toolbar.h>
#include <wx/statusbr.h>
#include <wx/textctrl.h>
#include <wx/treelist.h>

class geMainFrame : public wxFrame
{
public:
    geMainFrame();
    ~geMainFrame();

private:
    wxAuiManager m_mgr;
    wxAuiNotebook* m_notebook;
    wxToolBar* m_toolbar;
    wxTextCtrl* m_output;
    wxTreeListCtrl* m_stateTree;

    void OnNew(wxCommandEvent& evt);
    void OnOpen(wxCommandEvent& evt);
    void OnSave(wxCommandEvent& evt);
    void OnSaveAs(wxCommandEvent& evt);
    void OnExit(wxCommandEvent& evt);
    void OnUndo( wxCommandEvent& evt );
    void OnRedo( wxCommandEvent& evt );
    void OnCut( wxCommandEvent& evt );
    void OnCopy( wxCommandEvent& evt );
    void OnPaste( wxCommandEvent& evt );
    void OnHelpWebsite( wxCommandEvent& evt );
    void OnHelpAbout( wxCommandEvent& evt );
    void OnRun( wxCommandEvent& evt );
    void OnTabChanged(wxAuiNotebookEvent& evt);

    void UpdateStateTree();
    void UpdateStatusBar();

    wxDECLARE_EVENT_TABLE();
};
