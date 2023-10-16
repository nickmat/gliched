/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/gliched/geEdit.cpp
 * Project:     Gliched: Glich Script Language IDE.
 * Purpose:     Custom style text control class source.
 * Author:      Nick Matthews
 * Created:     13th October 2023
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

#include "geEdit.h"

#include <glc/glc.h>

#include <wx/filename.h>

using std::string;


geEdit::geEdit( wxWindow* parent ) : wxStyledTextCtrl( parent )
{
    StyleClearAll();
    SetLexer( wxSTC_LEX_CONTAINER );
}

void geEdit::OnFileOpen()
{
    wxFileDialog dlg( this, "Open file", wxEmptyString, wxEmptyString,
        "Glich (*.glc)|*.glcs", wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR );
    if( dlg.ShowModal() != wxID_OK ) {
        return;
    }
    wxString filepath = dlg.GetPath();
    if( filepath.empty() ) {
        return;
    }

    ClearAll();
    EmptyUndoBuffer();

    m_filepath = filepath;
    m_filename = wxFileNameFromPath( m_filepath );

    LoadFile( m_filepath );
}

void geEdit::OnFileSave()
{
    if( m_filepath.empty() ) {
        OnFileSaveAs();
    }
    SaveFile( m_filepath );
}

void geEdit::OnFileSaveAs()
{
    wxFileDialog dlg( this, "Save file", wxString(), wxString(), "Glich(*.glc) | *.glcs", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if( dlg.ShowModal() != wxID_OK ) return;
    m_filepath = dlg.GetPath();
    m_filename = wxFileNameFromPath( m_filepath );
    SaveFile( m_filepath );
}


// End of src/gliched/geEdit.cpp file.
