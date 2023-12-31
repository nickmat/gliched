///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/toolbar.h>
#include <wx/treelist.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include "geEdit.h"
#include <wx/textctrl.h>
#include <wx/splitter.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class fbGeFrame
///////////////////////////////////////////////////////////////////////////////
class fbGeFrame : public wxFrame
{
	private:

	protected:
		wxMenu* m_menuTools;
		wxToolBar* m_toolBar1;
		wxToolBarToolBase* m_toolNew;
		wxToolBarToolBase* m_toolOpen;
		wxToolBarToolBase* m_toolSave;
		wxToolBarToolBase* m_toolUndo;
		wxToolBarToolBase* m_toolRedo;
		wxToolBarToolBase* m_toolCut;
		wxToolBarToolBase* m_toolCopy;
		wxToolBarToolBase* m_toolPaste;
		wxButton* m_buttonRun;
		wxSplitterWindow* m_splitter2;
		wxPanel* m_panel3;
		wxTreeListCtrl* m_treeListCtrl;
		wxPanel* m_panel4;
		wxSplitterWindow* m_splitter4;
		wxPanel* m_panelEdit;
		geEdit* m_edit;
		wxPanel* m_panel7;
		wxTextCtrl* m_ctrlResult;
		wxStatusBar* m_statusBar1;

		// Virtual event handlers, override them in your derived class
		virtual void OnFileNew( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFileOpen( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFileSave( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFileSaveAs( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEditUndo( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEditRedo( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEditCut( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEditCopy( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEditPaste( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEditDelete( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRun( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnWebsite( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }


	public:

		fbGeFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Glich Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~fbGeFrame();

		void m_splitter2OnIdle( wxIdleEvent& )
		{
			m_splitter2->SetSashPosition( 250 );
			m_splitter2->Disconnect( wxEVT_IDLE, wxIdleEventHandler( fbGeFrame::m_splitter2OnIdle ), NULL, this );
		}

		void m_splitter4OnIdle( wxIdleEvent& )
		{
			m_splitter4->SetSashPosition( 500 );
			m_splitter4->Disconnect( wxEVT_IDLE, wxIdleEventHandler( fbGeFrame::m_splitter4OnIdle ), NULL, this );
		}

};

