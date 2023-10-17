/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/gliched/geEdit.h
 * Project:     Gliched: Glich Script Language IDE.
 * Purpose:     Custom style text control class header.
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

#ifndef SRC_GLICHED_GEEDIT_H_GUARD
#define SRC_GLICHED_GEEDIT_H_GUARD

#include <wx/stc/stc.h>

class geEdit : public wxStyledTextCtrl {
public:
    geEdit( wxWindow* parent );
    ~geEdit() {}

    void OnFileNew();
    wxString OnFileOpen();
    wxString OnFileSave();
    wxString OnFileSaveAs();

    wxString DoFileOpen( const wxString& filepath );

private:
    wxString m_filename;
    wxString m_filepath;
};

constexpr int c_line_num_margin = 0;
constexpr int c_symbol_margin = 1;
constexpr int c_folding_margin = 2;

#endif // SRC_GLICHED_GEEDIT_H_GUARD