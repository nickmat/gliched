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

    // Setup line numbers
    int line_num_width = TextWidth( wxSTC_STYLE_LINENUMBER, "_99999" );
    SetMarginType( c_line_num_margin, wxSTC_MARGIN_NUMBER );
    StyleSetForeground( wxSTC_STYLE_LINENUMBER, wxColour( "DARK GREY" ) );
    StyleSetBackground( wxSTC_STYLE_LINENUMBER, *wxWHITE );
    SetMarginWidth( c_line_num_margin, line_num_width );
}

wxString geEdit::OnFileOpen()
{
    wxFileDialog dlg( this, "Open file", wxEmptyString, wxEmptyString,
        "Glich (*.glc)|*.glcs", wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR );
    if( dlg.ShowModal() != wxID_OK ) {
        return wxString();
    }
    wxString filepath = dlg.GetPath();
    if( filepath.empty() ) {
        return wxString();
    }

    return DoFileOpen( filepath );
}

wxString geEdit::OnFileSave()
{
    if( m_filepath.empty() ) {
        return OnFileSaveAs();
    }
    SaveFile( m_filepath );
    return m_filename;
}

wxString geEdit::OnFileSaveAs()
{
    wxFileDialog dlg( this, "Save file", wxString(), wxString(), "Glich(*.glc) | *.glcs", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if( dlg.ShowModal() != wxID_OK ) {
        return wxString();
    }
    m_filepath = dlg.GetPath();
    m_filename = wxFileNameFromPath( m_filepath );
    if( SaveFile( m_filepath ) ) {
        return m_filename;
    }
    return wxString();
}

wxString geEdit::DoFileOpen( const wxString& filepath )
{
    ClearAll();
    EmptyUndoBuffer();

    m_filepath = filepath;
    m_filename = wxFileNameFromPath( m_filepath );

    if( LoadFile( m_filepath ) ) {
        return m_filename;
    }
    return wxString();
}


// End of src/gliched/geEdit.cpp file.
