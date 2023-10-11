/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/gliched/geApp.cpp
 * Project:     Gliched: Glich Script Language IDE.
 * Purpose:     Program Main Application Source.
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

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "geApp.h"

#include "geFrame.h"

#include <glc/glc.h>

IMPLEMENT_APP(geApp)

bool geApp::OnInit()
{
    glich::init_glc( glich::InitLibrary::Hics );

    long style = wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU |
        wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN | wxTAB_TRAVERSAL;

    m_frame = new geFrame( "Glich Editor", wxDefaultPosition, wxSize( -1, -1 ), style );

    m_frame->Show( true );

    SetTopWindow( m_frame );

    return true;
}

int geApp::OnExit()
{
    glich::exit_glc();
    return 0;
}

// End of src/gliched/geApp.cpp file
