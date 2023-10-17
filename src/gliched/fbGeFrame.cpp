///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>

#include "fbGeFrame.h"

///////////////////////////////////////////////////////////////////////////

fbGeFrame::fbGeFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 1000,750 ), wxDefaultSize );

	wxMenuBar* m_menubar;
	m_menubar = new wxMenuBar( 0 );
	wxMenu* m_menuFile;
	m_menuFile = new wxMenu();
	wxMenuItem* m_menuFileNew;
	m_menuFileNew = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("&New") ) + wxT('\t') + wxT("Ctrl+N"), _("Close current file and start new one."), wxITEM_NORMAL );
	m_menuFile->Append( m_menuFileNew );

	wxMenuItem* m_menuFileOpen;
	m_menuFileOpen = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("&Open...") ) + wxT('\t') + wxT("Ctrl+O"), _("Open a new file."), wxITEM_NORMAL );
	m_menuFile->Append( m_menuFileOpen );

	wxMenuItem* m_menuFileSave;
	m_menuFileSave = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("Save") ) + wxT('\t') + wxT("Ctrl+S"), _("Save current file."), wxITEM_NORMAL );
	m_menuFile->Append( m_menuFileSave );

	wxMenuItem* m_menuFileSaveAs;
	m_menuFileSaveAs = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("Save &As...") ) + wxT('\t') + wxT("Ctrl-Shift+S"), _("Save current file as..."), wxITEM_NORMAL );
	m_menuFile->Append( m_menuFileSaveAs );

	m_menuFile->AppendSeparator();

	wxMenuItem* m_menuExit;
	m_menuExit = new wxMenuItem( m_menuFile, wxID_EXIT, wxString( _("E&xit") ) + wxT('\t') + wxT("Alt+F4"), _("Quit Glich Editor"), wxITEM_NORMAL );
	m_menuFile->Append( m_menuExit );

	m_menubar->Append( m_menuFile, _("&File") );

	wxMenu* m_menuEdit;
	m_menuEdit = new wxMenu();
	wxMenuItem* m_menuEditUndo;
	m_menuEditUndo = new wxMenuItem( m_menuEdit, wxID_ANY, wxString( _("&Undo") ) + wxT('\t') + wxT("Ctrl+Z"), _("Undo the last action."), wxITEM_NORMAL );
	m_menuEdit->Append( m_menuEditUndo );

	wxMenuItem* m_menuEditRedo;
	m_menuEditRedo = new wxMenuItem( m_menuEdit, wxID_ANY, wxString( _("&Redo") ) + wxT('\t') + wxT("Ctrl+Y"), _("Replace the last undo."), wxITEM_NORMAL );
	m_menuEdit->Append( m_menuEditRedo );

	m_menuEdit->AppendSeparator();

	wxMenuItem* m_menuEditCut;
	m_menuEditCut = new wxMenuItem( m_menuEdit, wxID_ANY, wxString( _("Cu&t") ) + wxT('\t') + wxT("Ctrl+X"), _("Move selected text to the clipboard."), wxITEM_NORMAL );
	m_menuEdit->Append( m_menuEditCut );

	wxMenuItem* m_menuEditCopy;
	m_menuEditCopy = new wxMenuItem( m_menuEdit, wxID_ANY, wxString( _("&Copy") ) + wxT('\t') + wxT("Ctrl+C"), _("Copy selected text to the clipboard."), wxITEM_NORMAL );
	m_menuEdit->Append( m_menuEditCopy );

	wxMenuItem* m_menuEditPaste;
	m_menuEditPaste = new wxMenuItem( m_menuEdit, wxID_ANY, wxString( _("&Paste") ) + wxT('\t') + wxT("Ctrl+V"), _("Insert text from clipboard."), wxITEM_NORMAL );
	m_menuEdit->Append( m_menuEditPaste );

	wxMenuItem* m_menuEditDelete;
	m_menuEditDelete = new wxMenuItem( m_menuEdit, wxID_ANY, wxString( _("&Delete") ) + wxT('\t') + wxT("Del"), _("Remove selected text."), wxITEM_NORMAL );
	m_menuEdit->Append( m_menuEditDelete );

	m_menubar->Append( m_menuEdit, _("&Edit") );

	m_menuTools = new wxMenu();
	wxMenuItem* m_menuToolsRun;
	m_menuToolsRun = new wxMenuItem( m_menuTools, wxID_ANY, wxString( _("&Run") ) + wxT('\t') + wxT("F5"), _("Run Glich script."), wxITEM_NORMAL );
	m_menuTools->Append( m_menuToolsRun );

	m_menubar->Append( m_menuTools, _("&Tools") );

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
	m_toolNew = m_toolBar1->AddTool( wxID_ANY, _("New"), wxArtProvider::GetBitmap( wxART_NEW, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, _("New Script [Ctrl+N]"), _("Close current file and start a new one."), NULL );

	m_toolOpen = m_toolBar1->AddTool( wxID_ANY, _("Open"), wxArtProvider::GetBitmap( wxART_FILE_OPEN, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, _("Open File [Ctrl+O]"), _("Open a new file."), NULL );

	m_toolSave = m_toolBar1->AddTool( wxID_ANY, _("Save"), wxArtProvider::GetBitmap( wxART_FILE_SAVE, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, _("Save File [Ctrl+S]"), _("Save current file."), NULL );

	m_toolBar1->AddSeparator();

	m_toolUndo = m_toolBar1->AddTool( wxID_ANY, _("Undo"), wxArtProvider::GetBitmap( wxART_UNDO, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, _("Undo [Ctrl+Z]"), _("Undo the last action."), NULL );

	m_toolRedo = m_toolBar1->AddTool( wxID_ANY, _("Redo"), wxArtProvider::GetBitmap( wxART_REDO, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, _("Redo [Ctrl+Y]"), _("Replace the last undo."), NULL );

	m_toolBar1->AddSeparator();

	m_toolCut = m_toolBar1->AddTool( wxID_ANY, _("Cut"), wxArtProvider::GetBitmap( wxART_CUT, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, _("Cut [Ctrl+X]"), _("Move selected text to the clipboard."), NULL );

	m_toolCopy = m_toolBar1->AddTool( wxID_ANY, _("Copy"), wxArtProvider::GetBitmap( wxART_COPY, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, _("Copy [Ctrl+C]"), _("Copy selected text to clipboard"), NULL );

	m_toolPaste = m_toolBar1->AddTool( wxID_ANY, _("Paste"), wxArtProvider::GetBitmap( wxART_PASTE, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, _("Paste [Ctrl+V]"), _("Insert text from clipboard"), NULL );

	m_toolBar1->AddSeparator();

	m_buttonRun = new wxButton( m_toolBar1, wxID_ANY, _("Run"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonRun->SetToolTip( _("Run Script [F5]") );

	m_toolBar1->AddControl( m_buttonRun );
	m_toolBar1->Realize();

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );

	m_splitter2 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NO_XP_THEME );
	m_splitter2->SetSashGravity( 0.25 );
	m_splitter2->Connect( wxEVT_IDLE, wxIdleEventHandler( fbGeFrame::m_splitter2OnIdle ), NULL, this );

	m_panel3 = new wxPanel( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );

	m_treeListCtrl = new wxTreeListCtrl( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTL_DEFAULT_STYLE );

	bSizer16->Add( m_treeListCtrl, 1, wxEXPAND | wxALL, 5 );


	m_panel3->SetSizer( bSizer16 );
	m_panel3->Layout();
	bSizer16->Fit( m_panel3 );
	m_panel4 = new wxPanel( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );

	m_splitter4 = new wxSplitterWindow( m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NO_XP_THEME );
	m_splitter4->SetSashGravity( 0.75 );
	m_splitter4->Connect( wxEVT_IDLE, wxIdleEventHandler( fbGeFrame::m_splitter4OnIdle ), NULL, this );
	m_splitter4->SetMinimumPaneSize( 100 );

	m_panelEdit = new wxPanel( m_splitter4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxVERTICAL );

	m_edit = new geEdit( m_panelEdit );
	bSizer17->Add( m_edit, 1, wxALL|wxEXPAND, 5 );


	m_panelEdit->SetSizer( bSizer17 );
	m_panelEdit->Layout();
	bSizer17->Fit( m_panelEdit );
	m_panel7 = new wxPanel( m_splitter4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );

	m_ctrlResult = new wxTextCtrl( m_panel7, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP );
	bSizer18->Add( m_ctrlResult, 1, wxALL|wxEXPAND, 5 );


	m_panel7->SetSizer( bSizer18 );
	m_panel7->Layout();
	bSizer18->Fit( m_panel7 );
	m_splitter4->SplitHorizontally( m_panelEdit, m_panel7, 500 );
	bSizer15->Add( m_splitter4, 1, wxEXPAND, 5 );


	m_panel4->SetSizer( bSizer15 );
	m_panel4->Layout();
	bSizer15->Fit( m_panel4 );
	m_splitter2->SplitVertically( m_panel3, m_panel4, 250 );
	bSizer11->Add( m_splitter2, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer11 );
	this->Layout();
	bSizer11->Fit( this );
	m_statusBar1 = this->CreateStatusBar( 2, wxSTB_SIZEGRIP, wxID_ANY );

	this->Centre( wxBOTH );

	// Connect Events
	m_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGeFrame::OnFileNew ), this, m_menuFileNew->GetId());
	m_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGeFrame::OnFileOpen ), this, m_menuFileOpen->GetId());
	m_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGeFrame::OnFileSave ), this, m_menuFileSave->GetId());
	m_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGeFrame::OnFileSaveAs ), this, m_menuFileSaveAs->GetId());
	m_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGeFrame::OnExit ), this, m_menuExit->GetId());
	m_menuEdit->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGeFrame::OnEditUndo ), this, m_menuEditUndo->GetId());
	m_menuEdit->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGeFrame::OnEditRedo ), this, m_menuEditRedo->GetId());
	m_menuEdit->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGeFrame::OnEditCut ), this, m_menuEditCut->GetId());
	m_menuEdit->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGeFrame::OnEditCopy ), this, m_menuEditCopy->GetId());
	m_menuEdit->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGeFrame::OnEditPaste ), this, m_menuEditPaste->GetId());
	m_menuEdit->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGeFrame::OnEditDelete ), this, m_menuEditDelete->GetId());
	m_menuTools->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGeFrame::OnRun ), this, m_menuToolsRun->GetId());
	m_menuHelp->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGeFrame::OnWebsite ), this, m_menuHelpWebsite->GetId());
	m_menuHelp->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( fbGeFrame::OnAbout ), this, m_menuHelpAbout->GetId());
	this->Connect( m_toolNew->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( fbGeFrame::OnFileNew ) );
	this->Connect( m_toolOpen->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( fbGeFrame::OnFileOpen ) );
	this->Connect( m_toolSave->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( fbGeFrame::OnFileSave ) );
	this->Connect( m_toolUndo->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( fbGeFrame::OnEditUndo ) );
	this->Connect( m_toolRedo->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( fbGeFrame::OnEditRedo ) );
	this->Connect( m_toolCut->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( fbGeFrame::OnEditCut ) );
	this->Connect( m_toolCopy->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( fbGeFrame::OnEditCopy ) );
	this->Connect( m_toolPaste->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( fbGeFrame::OnEditPaste ) );
	m_buttonRun->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( fbGeFrame::OnRun ), NULL, this );
}

fbGeFrame::~fbGeFrame()
{
	// Disconnect Events
	this->Disconnect( m_toolNew->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( fbGeFrame::OnFileNew ) );
	this->Disconnect( m_toolOpen->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( fbGeFrame::OnFileOpen ) );
	this->Disconnect( m_toolSave->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( fbGeFrame::OnFileSave ) );
	this->Disconnect( m_toolUndo->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( fbGeFrame::OnEditUndo ) );
	this->Disconnect( m_toolRedo->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( fbGeFrame::OnEditRedo ) );
	this->Disconnect( m_toolCut->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( fbGeFrame::OnEditCut ) );
	this->Disconnect( m_toolCopy->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( fbGeFrame::OnEditCopy ) );
	this->Disconnect( m_toolPaste->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( fbGeFrame::OnEditPaste ) );
	m_buttonRun->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( fbGeFrame::OnRun ), NULL, this );

}
