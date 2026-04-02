/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/gliched/geMainFrame.h
 * Project:     Gliched: Glich Script Language IDE.
 * Purpose:     Program Main Frame Class Header.
 * Author:      Nick Matthews
 * Created:     19th September 2023
 * Copyright:   Copyright (c) 2023..2026, Nick Matthews.
 * Licence:     GNU GPLv3
 *
 *  Gliched is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Gliched is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Gliched.  If not, see <http://www.gnu.org/licenses/>.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

 */

#pragma once

#include <wx/frame.h>
#include <wx/aui/aui.h>
#include <wx/aui/auibook.h>
#include <wx/toolbar.h>
#include <wx/statusbr.h>
#include <wx/textctrl.h>
#include <wx/treelist.h>

#include <vector>
#include <string>

class geMainFrame : public wxFrame
{
public:
    geMainFrame();
    ~geMainFrame();

    bool IsAutosaveEnabled() const { return m_autosaveEnabled; }

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
    void OnTabRightClick( wxAuiNotebookEvent& evt );
    void OnTabClose( wxAuiNotebookEvent& evt );
    void OnSetAsRunFile( wxCommandEvent& evt );
    void OnClearRunFile( wxCommandEvent& evt );
    void OnClose( wxCloseEvent& event );
    void OnToggleAutosave( wxCommandEvent& );

    wxString GetFilePathForTab( int idx ) const;
    bool IsTabSetAsRunFile( int idx ) const;
    wxString GetTabLabelForFile( const wxString& filePath ) const;

    int GetRunTab() const;
    void UpdateTabIndicators();
    void UpdateStateTree();
    void UpdateStatusBar();
    void AddModulePath( const std::string& path );

    int m_tabContextIndex; // Index of the tab for which the context menu is currently open, or -1 if none
    int m_newTabCounter; // Counter for naming new tabs
    std::vector<std::string> m_modulePaths; // File paths of each open file used for locating modules.
    bool m_autosaveEnabled = true;

    wxDECLARE_EVENT_TABLE();
};
