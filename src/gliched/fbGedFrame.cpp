///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>

#include "fbGedFrame.h"

///////////////////////////////////////////////////////////////////////////

fbGedFrame::fbGedFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxMenuBar* m_menubar;
	m_menubar = new wxMenuBar( 0 );
	wxMenu* m_menuFile;
	m_menuFile = new wxMenu();
	wxMenuItem* m_menuFileNew;
	m_menuFileNew = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("&New") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuFileNew );

	wxMenuItem* m_menuFileOpen;
	m_menuFileOpen = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("&Open...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuFileOpen );

	wxMenuItem* m_menuFileSave;
	m_menuFileSave = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("Save") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuFileSave );

	wxMenuItem* m_menuFileSaveAs;
	m_menuFileSaveAs = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("Save &As...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuFileSaveAs );

	wxMenuItem* m_menuFileClose;
	m_menuFileClose = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("&Close") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuFileClose );

	m_menuFile->AppendSeparator();

	wxMenuItem* m_menuExit;
	m_menuExit = new wxMenuItem( m_menuFile, wxID_EXIT, wxString( _("E&xit") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuFile->Append( m_menuExit );

	m_menubar->Append( m_menuFile, _("&File") );

	wxMenu* m_menuEdit;
	m_menuEdit = new wxMenu();
	wxMenuItem* m_menuEditUndo;
	m_menuEditUndo = new wxMenuItem( m_menuEdit, wxID_ANY, wxString( _("&Undo") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuEdit->Append( m_menuEditUndo );

	wxMenuItem* m_menuEditRedo;
	m_menuEditRedo = new wxMenuItem( m_menuEdit, wxID_ANY, wxString( _("&Redo") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuEdit->Append( m_menuEditRedo );

	m_menuEdit->AppendSeparator();

	wxMenuItem* m_menuEditCut;
	m_menuEditCut = new wxMenuItem( m_menuEdit, wxID_ANY, wxString( _("Cu&t") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuEdit->Append( m_menuEditCut );

	wxMenuItem* m_menuEditCopy;
	m_menuEditCopy = new wxMenuItem( m_menuEdit, wxID_ANY, wxString( _("&Copy") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuEdit->Append( m_menuEditCopy );

	wxMenuItem* m_menuEditPaste;
	m_menuEditPaste = new wxMenuItem( m_menuEdit, wxID_ANY, wxString( _("&Paste") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuEdit->Append( m_menuEditPaste );

	wxMenuItem* m_menuEditDelete;
	m_menuEditDelete = new wxMenuItem( m_menuEdit, wxID_ANY, wxString( _("&Delete") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuEdit->Append( m_menuEditDelete );

	m_menubar->Append( m_menuEdit, _("&Edit") );

	wxMenu* m_menuHelp;
	m_menuHelp = new wxMenu();
	wxMenuItem* m_menuHelpWebsite;
	m_menuHelpWebsite = new wxMenuItem( m_menuHelp, wxID_ANY, wxString( _("&Website") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuHelp->Append( m_menuHelpWebsite );

	wxMenuItem* m_menuHelpAbout;
	m_menuHelpAbout = new wxMenuItem( m_menuHelp, wxID_ABOUT, wxString( _("&About...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuHelp->Append( m_menuHelpAbout );

	m_menubar->Append( m_menuHelp, _("&Help") );

	this->SetMenuBar( m_menubar );

	m_toolBar1 = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY );
	m_buttonRun = new wxButton( m_toolBar1, wxID_ANY, _("&Run"), wxDefaultPosition, wxDefaultSize, 0 );
	m_toolBar1->AddControl( m_buttonRun );
	m_toolBar1->Realize();

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );

	m_splitter2 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NO_XP_THEME );
	m_splitter2->Connect( wxEVT_IDLE, wxIdleEventHandler( fbGedFrame::m_splitter2OnIdle ), NULL, this );

	m_panel3 = new wxPanel( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );

	m_dataViewCtrl1 = new wxDataViewCtrl( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_dataViewCtrl1, 1, wxALL|wxEXPAND, 5 );


	m_panel3->SetSizer( bSizer16 );
	m_panel3->Layout();
	bSizer16->Fit( m_panel3 );
	m_panel4 = new wxPanel( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );

	m_splitter4 = new wxSplitterWindow( m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NO_XP_THEME );
	m_splitter4->Connect( wxEVT_IDLE, wxIdleEventHandler( fbGedFrame::m_splitter4OnIdle ), NULL, this );

	m_panel6 = new wxPanel( m_splitter4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxVERTICAL );

	m_scintilla3 = new wxStyledTextCtrl( m_panel6, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, wxEmptyString );
	m_scintilla3->SetUseTabs( true );
	m_scintilla3->SetTabWidth( 4 );
	m_scintilla3->SetIndent( 4 );
	m_scintilla3->SetTabIndents( true );
	m_scintilla3->SetBackSpaceUnIndents( true );
	m_scintilla3->SetViewEOL( false );
	m_scintilla3->SetViewWhiteSpace( false );
	m_scintilla3->SetMarginWidth( 2, 0 );
	m_scintilla3->SetIndentationGuides( true );
	m_scintilla3->SetReadOnly( false );
	m_scintilla3->SetMarginType( 1, wxSTC_MARGIN_SYMBOL );
	m_scintilla3->SetMarginMask( 1, wxSTC_MASK_FOLDERS );
	m_scintilla3->SetMarginWidth( 1, 16);
	m_scintilla3->SetMarginSensitive( 1, true );
	m_scintilla3->SetProperty( wxT("fold"), wxT("1") );
	m_scintilla3->SetFoldFlags( wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED );
	m_scintilla3->SetMarginType( 0, wxSTC_MARGIN_NUMBER );
	m_scintilla3->SetMarginWidth( 0, m_scintilla3->TextWidth( wxSTC_STYLE_LINENUMBER, wxT("_99999") ) );
	m_scintilla3->MarkerDefine( wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS );
	m_scintilla3->MarkerSetBackground( wxSTC_MARKNUM_FOLDER, wxColour( wxT("BLACK") ) );
	m_scintilla3->MarkerSetForeground( wxSTC_MARKNUM_FOLDER, wxColour( wxT("WHITE") ) );
	m_scintilla3->MarkerDefine( wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS );
	m_scintilla3->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPEN, wxColour( wxT("BLACK") ) );
	m_scintilla3->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPEN, wxColour( wxT("WHITE") ) );
	m_scintilla3->MarkerDefine( wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY );
	m_scintilla3->MarkerDefine( wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUS );
	m_scintilla3->MarkerSetBackground( wxSTC_MARKNUM_FOLDEREND, wxColour( wxT("BLACK") ) );
	m_scintilla3->MarkerSetForeground( wxSTC_MARKNUM_FOLDEREND, wxColour( wxT("WHITE") ) );
	m_scintilla3->MarkerDefine( wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUS );
	m_scintilla3->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPENMID, wxColour( wxT("BLACK") ) );
	m_scintilla3->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPENMID, wxColour( wxT("WHITE") ) );
	m_scintilla3->MarkerDefine( wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY );
	m_scintilla3->MarkerDefine( wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY );
	m_scintilla3->SetSelBackground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );
	m_scintilla3->SetSelForeground( true, wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );
	bSizer17->Add( m_scintilla3, 1, wxEXPAND | wxALL, 5 );


	m_panel6->SetSizer( bSizer17 );
	m_panel6->Layout();
	bSizer17->Fit( m_panel6 );
	m_panel7 = new wxPanel( m_splitter4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );

	m_textCtrl5 = new wxTextCtrl( m_panel7, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP );
	bSizer18->Add( m_textCtrl5, 1, wxALL|wxEXPAND, 5 );


	m_panel7->SetSizer( bSizer18 );
	m_panel7->Layout();
	bSizer18->Fit( m_panel7 );
	m_splitter4->SplitHorizontally( m_panel6, m_panel7, 180 );
	bSizer15->Add( m_splitter4, 1, wxEXPAND, 5 );


	m_panel4->SetSizer( bSizer15 );
	m_panel4->Layout();
	bSizer15->Fit( m_panel4 );
	m_splitter2->SplitVertically( m_panel3, m_panel4, 143 );
	bSizer11->Add( m_splitter2, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer11 );
	this->Layout();
	m_statusBar1 = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );

	this->Centre( wxBOTH );

	// Connect Events
	m_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGedFrame::OnFileNew ), this, m_menuFileNew->GetId());
	m_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGedFrame::OnFileOpen ), this, m_menuFileOpen->GetId());
	m_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGedFrame::OnFileSave ), this, m_menuFileSave->GetId());
	m_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGedFrame::OnFileSaveAs ), this, m_menuFileSaveAs->GetId());
	m_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGedFrame::OnFileClose ), this, m_menuFileClose->GetId());
	m_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGedFrame::OnExit ), this, m_menuExit->GetId());
	m_menuEdit->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGedFrame::OnEditUndo ), this, m_menuEditUndo->GetId());
	m_menuEdit->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGedFrame::OnEditRedo ), this, m_menuEditRedo->GetId());
	m_menuEdit->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGedFrame::OnEditCut ), this, m_menuEditCut->GetId());
	m_menuEdit->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGedFrame::OnEditCopy ), this, m_menuEditCopy->GetId());
	m_menuEdit->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGedFrame::OnEditPaste ), this, m_menuEditPaste->GetId());
	m_menuEdit->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGedFrame::OnEditDelete ), this, m_menuEditDelete->GetId());
	m_menuHelp->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGedFrame::OnWebsite ), this, m_menuHelpWebsite->GetId());
	m_menuHelp->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGedFrame::OnAbout ), this, m_menuHelpAbout->GetId());
	m_buttonRun->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( fbGedFrame::OnRun ), NULL, this );
}

fbGedFrame::~fbGedFrame()
{
	// Disconnect Events
	m_buttonRun->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( fbGedFrame::OnRun ), NULL, this );

}
