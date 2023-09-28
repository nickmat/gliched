/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/gliched/gedFrame.h
 * Project:     Gliched: Glich Script Language IDE.
 * Purpose:     Program Main Frame.
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

#ifndef SRC_GLICHED_GEFRAME_H_GUARD
#define SRC_GLICHED_GEFRAME_H_GUARD

#include "fbGeFrame.h"

class geFrame : public fbGeFrame
{
public:
    geFrame( 
        const wxString& title, 
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE );

private:
	void OnFileNew( wxCommandEvent& event ) override { event.Skip(); }
	void OnFileOpen( wxCommandEvent& event ) override { event.Skip(); }
	void OnFileSave( wxCommandEvent& event ) override { event.Skip(); }
	void OnFileSaveAs( wxCommandEvent& event ) override { event.Skip(); }
	void OnFileClose( wxCommandEvent& event ) override { event.Skip(); }
	void OnExit( wxCommandEvent& event ) override;
	void OnEditUndo( wxCommandEvent& event ) override { event.Skip(); }
	void OnEditRedo( wxCommandEvent& event ) override { event.Skip(); }
	void OnEditCut( wxCommandEvent& event ) override { event.Skip(); }
	void OnEditCopy( wxCommandEvent& event ) override { event.Skip(); }
	void OnEditPaste( wxCommandEvent& event ) override { event.Skip(); }
	void OnEditDelete( wxCommandEvent& event ) override { event.Skip(); }
	void OnWebsite( wxCommandEvent& event ) override { event.Skip(); }
	void OnAbout( wxCommandEvent& event ) override { event.Skip(); }
	void OnRun( wxCommandEvent& event ) override;

	void UpdateDataTree();

};

#endif // SRC_GLICHED_GEFRAME_H_GUARD