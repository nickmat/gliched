/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/gliched/geEdit.cpp
 * Project:     Gliched: Glich Script Language IDE.
 * Purpose:     Custom style text control class source.
 * Author:      Nick Matthews
 * Created:     13th October 2023
 * Copyright:   Copyright (c) 2023..2024, Nick Matthews.
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


geEdit::geEdit( wxWindow* parent ) :
    m_font( 11, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL ),
    wxStyledTextCtrl( parent )
{
    StyleClearAll();
    SetLexer( wxSTC_LEX_CONTAINER );

    // Set default styling
    StyleSetFont( wxSTC_STYLE_DEFAULT, m_font );
    SetIndent( 4 );
    SetTabWidth( 4 );
    SetUseTabs( false );
    StyleClearAll();

    // Setup line numbers
    int line_num_width = TextWidth( wxSTC_STYLE_LINENUMBER, "_99999" );
    SetMarginType( c_line_num_margin, wxSTC_MARGIN_NUMBER );
    StyleSetForeground( wxSTC_STYLE_LINENUMBER, wxColour( "DARK GREY" ) );
    StyleSetBackground( wxSTC_STYLE_LINENUMBER, *wxWHITE );
    SetMarginWidth( c_line_num_margin, line_num_width );

    Bind( wxEVT_STC_CHARADDED, &geEdit::OnCharAdded, this );
}

void geEdit::OnFileNew()
{
    if( IsModified() ) {
        int rep = wxMessageBox("Save current script?", "New File", wxYES_NO );
        if( rep == wxYES ) {
            OnFileSave();
        }
    }
    ClearAll();
    EmptyUndoBuffer();
    m_filename = wxString();
    m_filepath = wxString();
}

wxString geEdit::OnFileOpen()
{
    wxFileDialog dlg( this, "Open file", wxEmptyString, wxEmptyString,
        "Glich (*.glcs)|*.glcs", wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR );
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
    wxFileDialog dlg( this, "Save file", wxString(), wxString(), "Glich(*.glcs) | *.glcs", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
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

// Thanks to New Pagodi, see 
// https://stackoverflow.com/questions/42241874/how-to-control-lineindentation-in-wxstyledtextctrl-when-user-presses-enter
void geEdit::OnCharAdded( wxStyledTextEvent& event )
{
    int new_line_key = (GetEOLMode() == wxSTC_EOL_CR) ? 13 : 10;

    if( event.GetKey() == new_line_key ) {
        int cur_pos = GetCurrentPos();
        int cur_line = LineFromPosition( cur_pos );

        if( cur_line > 0 ) {
            wxString prev_line = GetLine( cur_line - 1 );
            size_t prev_line_indent_chars( 0 );

            for( size_t i = 0; i < prev_line.Length(); ++i ) {
                wxUniChar cur_char = prev_line.GetChar( i );

                if( cur_char == ' ' ) {
                    ++prev_line_indent_chars;
                }
                else if( cur_char == '\t' ) {
                    ++prev_line_indent_chars;
                }
                else {
                    break;
                }
            }

            AddText( prev_line.Left( prev_line_indent_chars ) );
        }
    }
}


// End of src/gliched/geEdit.cpp file.
