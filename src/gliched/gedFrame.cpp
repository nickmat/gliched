/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/gliched/gedApp.cpp
 * Project:     Gliched: Glich Script Language IDE.
 * Purpose:     Application main and supporting functions.
 * Author:      Nick Matthews
 * Created:     19th September 2023
 * Copyright:   Copyright (c) 2023, Nick Matthews.
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

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "gedFrame.h"

#include <glc/glc.h>

using std::string;


gedFrame::gedFrame(
    const wxString& title, const wxPoint& pos, const wxSize& size, long style )
    : fbGedFrame( (wxFrame*) nullptr, wxID_ANY, title, pos, size, style )
{
    // Set frames Icon
    SetIcon( wxICON( gliched_icon ) );
}

void gedFrame::OnExit( wxCommandEvent& event )
{
    // Close all windows and exit.
    wxTheApp->Exit();
}

void gedFrame::OnRun( wxCommandEvent& event )
{
    string script = m_ctrlEdit->GetText();
    string result = glich::get_glc()->run_script( script );
    m_ctrlResult->SetValue( result );
}

// End of src/gliched/gedFrame.cpp file.
