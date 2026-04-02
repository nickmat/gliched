/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/gliched/geEditor.h
 * Project:     Gliched: Glich Script Language IDE.
 * Purpose:     Custom style text control class header.
 * Author:      Nick Matthews
 * Created:     13th October 2023
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

#include <wx/stc/stc.h>
#include <wx/timer.h>

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
    void OnStyleNeeded(wxStyledTextEvent& event);
    void OnMarginClick( wxStyledTextEvent& event );
    void OnCharAdded(wxStyledTextEvent& event);
    void OnUpdateUI( wxStyledTextEvent& );
    void OnContentChanged( wxStyledTextEvent& );
    void OnAutosaveTimer( wxTimerEvent& );

    wxString m_filename;
    wxString m_tabName;
    bool m_runPage;
    wxTimer m_autosaveTimer;
};
