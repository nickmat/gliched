/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/gliched/geApp.cpp
 * Project:     Gliched: Glich Script Language IDE.
 * Purpose:     Program Main Application Source.
 * Author:      Nick Matthews
 * Created:     19th September 2023
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

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "geApp.h"

#include "geFrame.h"

#include <glc/glc.h>

IMPLEMENT_APP(geApp)


class GeInOut : public glich::InOut
{
public:
    std::string get_input( const std::string& prompt ) override;
};

std::string GeInOut::get_input( const std::string& prompt )
{
    wxTextEntryDialog dialog(
        nullptr, prompt,
        _( "Gliched Input" ), "", wxOK | wxCANCEL
    );
    if( dialog.ShowModal() == wxID_OK ) {
        return dialog.GetValue().ToStdString();
    }
    return std::string();
}

bool geApp::OnInit()
{
    glich::InitLibrary lib = glich::InitLibrary::Hics;
    wxString filename;
    if( argc > 1 ) {
        if( argv[1] == "--lib" && argc > 2 ) {
            wxString libname = argv[2];
            if( libname == "none" ) {
                lib = glich::InitLibrary::None;
            }
            else if( libname == "hics" ) {
                lib = glich::InitLibrary::Hics;
            }
            if( argc > 3 ) {
                filename = argv[3];
            }
        }
        else {
            filename = argv[1];
        }
    }
    glich::init_glc( lib, new GeInOut );

    geFrame* frame = new geFrame( "Glich Editor", filename );

    frame->Show( true );

    SetTopWindow( frame );

    return true;
}

int geApp::OnExit()
{
    glich::exit_glc();
    return 0;
}

// End of src/gliched/geApp.cpp file
