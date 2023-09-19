/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/hceapp.cpp
 * Project:     hcsedit: HistoryCal Script Editor.
 * Purpose:     Application main and supporting functions.
 * Author:      Nick Matthews
 * Created:     23rd May 2020
 * Copyright:   Copyright (c) 2020, Nick Matthews.
 * Licence:     Boost - Version 1.0
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file license.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

 */

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "gedApp.h"

#include "gedFrame.h"


IMPLEMENT_APP(gedApp)

bool gedApp::OnInit()
{
    long style = wxMINIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU |
        wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN | wxTAB_TRAVERSAL;
    frame = new gedFrame( "HCS Editor", wxDefaultPosition, wxSize( -1, -1 ), style );
    frame->Show();
    return true;
}

// End of src/hceapp.cpp file
