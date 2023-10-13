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


geEdit::geEdit( wxWindow* parent )
    : m_lang_index( geLang_glich ), m_lang( &g_lang_prefs[geLang_glich] ),
    wxStyledTextCtrl( parent )
{
}

bool geEdit::InitializePrefs( geLang index )
{
    return false;
}

void geEdit::OnFileOpen()
{
    if( !m_filename ) {
        wxFileDialog dlg( this, "Open file", wxEmptyString, wxEmptyString,
            "Any file (*)|*", wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR );
        if( dlg.ShowModal() != wxID_OK ) {
            return;
        }
        m_filename = dlg.GetPath();
    }
    DoFileOpen( m_filename );
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
    wxFileDialog dlg( this, "Save file", wxString(), wxString(), m_lang->default_fp, wxFD_SAVE | wxFD_OVERWRITE_PROMPT );
    if( dlg.ShowModal() != wxID_OK ) return;
    m_filepath = dlg.GetPath();
    m_filename = wxFileNameFromPath( m_filepath );
    SaveFile( m_filepath );
}

geLang geEdit::DeterminePrefs( const wxString& filename )
{
    // determine language from filepatterns
    for( size_t i = 0; i < geLang_size; i++ ) {
        wxString filepattern = g_lang_prefs[i].filepattern;
        filepattern.Lower();
        while( !filepattern.empty() ) {
            wxString cur = filepattern.BeforeFirst( ';' );
            if( (cur == filename) ||
                (cur == (filename.BeforeLast( '.' ) + ".*")) ||
                (cur == ("*." + filename.AfterLast( '.' ))) ) {
                return static_cast<geLang>(i);
            }
            filepattern = filepattern.AfterFirst( ';' );
        }
    }
    return geLang_text;
}

bool geEdit::DoFileOpen( wxString filename )
{
    // load file in edit and clear undo
    if( !filename.empty() ) {
        m_filename = filename;
    }
    LoadFile( m_filename );

    EmptyUndoBuffer();

    // determine lexer language
    wxFileName fname( m_filename );
    InitializePrefs( DeterminePrefs( fname.GetFullName() ) );

    return true;
}


// End of src/gliched/geEdit.cpp file.
