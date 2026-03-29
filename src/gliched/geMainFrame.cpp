
#include "geMainFrame.h"

#include "geEditor.h"
#include "geImages.h"

#include <glc/hic.h>

#include <wx/artprov.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/treelist.h>
#include <wx/bmpbndl.h>
#include <wx/icon.h>
#include <wx/image.h>
#include <wx/mstream.h>


enum
{
    ID_New = wxID_HIGHEST + 1,
    ID_Open,
    ID_Save,
    ID_SaveAs,
    ID_Undo,
    ID_Redo,
    ID_Cut,
    ID_Copy,
    ID_Paste,
    ID_Help_Website,
    ID_Help_About,
    ID_Run
};

wxBEGIN_EVENT_TABLE(geMainFrame, wxFrame)
    EVT_MENU(ID_New, geMainFrame::OnNew)
    EVT_MENU(ID_Open, geMainFrame::OnOpen)
    EVT_MENU(ID_Save, geMainFrame::OnSave)
    EVT_MENU(ID_SaveAs, geMainFrame::OnSaveAs)
    EVT_MENU(wxID_EXIT, geMainFrame::OnExit)
    EVT_TOOL( ID_Undo, geMainFrame::OnUndo )
    EVT_TOOL( ID_Redo, geMainFrame::OnRedo )
    EVT_TOOL( ID_Cut, geMainFrame::OnCut )
    EVT_TOOL( ID_Copy, geMainFrame::OnCopy )
    EVT_TOOL( ID_Paste, geMainFrame::OnPaste )
    EVT_MENU( ID_Help_Website, geMainFrame::OnHelpWebsite )
    EVT_MENU( ID_Help_About, geMainFrame::OnHelpAbout )
    EVT_TOOL( ID_Run, geMainFrame::OnRun )
    EVT_AUINOTEBOOK_PAGE_CHANGED(wxID_ANY, geMainFrame::OnTabChanged)
wxEND_EVENT_TABLE()

geMainFrame::geMainFrame()
    : wxFrame(nullptr, wxID_ANY, "Gliched IDE", wxDefaultPosition, wxSize(900, 700)),
      m_mgr(this)
{
    wxBitmapBundle bundle = wxBitmapBundle::FromSVG( glich_icon_svg, wxSize( 32, 32 ) );
    if( bundle.IsOk() ) {
        wxBitmap bmap = bundle.GetBitmap( wxSize( -1, -1 ) );
        wxIcon icon;
        icon.CopyFromBitmap( bmap );
        SetIcon( icon );
    }

    // Menu
    wxMenuBar* menuBar = new wxMenuBar;

    // File menu
    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append(ID_New, "&New\tCtrl+N");
    fileMenu->Append(ID_Open, "&Open...\tCtrl+O");
    fileMenu->Append(ID_Save, "&Save\tCtrl+S");
    fileMenu->Append(ID_SaveAs, "Save &As...");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "E&xit");
    menuBar->Append(fileMenu, "&File");

    // Edit menu
    wxMenu* editMenu = new wxMenu;
    editMenu->Append( ID_Undo, "&Undo\tCtrl+Z" );
    editMenu->Append( ID_Redo, "&Redo\tCtrl+Y" );
    editMenu->AppendSeparator();
    editMenu->Append( ID_Cut, "Cu&t\tCtrl+X" );
    editMenu->Append( ID_Copy, "&Copy\tCtrl+C" );
    editMenu->Append( ID_Paste, "&Paste\tCtrl+V" );
    menuBar->Append( editMenu, "&Edit" );

    // Help menu
    wxMenu* helpMenu = new wxMenu;
    helpMenu->Append( ID_Help_Website, "Gliched &Website...\tF1" );
    helpMenu->AppendSeparator();
    helpMenu->Append( ID_Help_About, "&About Gliched..." );
    menuBar->Append( helpMenu, "&Help" );

    SetMenuBar( menuBar );

    // Toolbar
    m_toolbar = CreateToolBar();
    m_toolbar->AddTool(ID_New, "New", wxArtProvider::GetBitmap(wxART_NEW, wxART_TOOLBAR));
    m_toolbar->AddTool(ID_Open, "Open", wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR));
    m_toolbar->AddTool(ID_Save, "Save", wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_TOOLBAR));
    m_toolbar->AddTool(ID_SaveAs, "Save As", wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_TOOLBAR));
    m_toolbar->AddSeparator();
    m_toolbar->AddTool( ID_Undo, "Undo", wxArtProvider::GetBitmap( wxART_UNDO, wxART_TOOLBAR ) );
    m_toolbar->AddTool( ID_Redo, "Redo", wxArtProvider::GetBitmap( wxART_REDO, wxART_TOOLBAR ) );
    m_toolbar->AddSeparator();
    m_toolbar->AddTool( ID_Cut, "Cut", wxArtProvider::GetBitmap( wxART_CUT, wxART_TOOLBAR ) );
    m_toolbar->AddTool( ID_Copy, "Copy", wxArtProvider::GetBitmap( wxART_COPY, wxART_TOOLBAR ) );
    m_toolbar->AddTool( ID_Paste, "Paste", wxArtProvider::GetBitmap( wxART_PASTE, wxART_TOOLBAR ) );
    m_toolbar->AddSeparator();
    m_toolbar->AddTool( ID_Run, "Run", wxArtProvider::GetBitmap( wxART_EXECUTABLE_FILE, wxART_TOOLBAR ) );
    m_toolbar->Realize();

    // Status bar
    CreateStatusBar(1);
    SetStatusText("Ready");

    // Notebook for editors
    m_notebook = new wxAuiNotebook(this, wxID_ANY);

    // Output pane
    m_output = new wxTextCtrl( this, wxID_ANY, wxEmptyString,
        wxDefaultPosition, wxSize( -1, 120 ),
        wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH2 );

    // State tree (left pane) - wxTreeListCtrl with columns
    m_stateTree = new wxTreeListCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( 250, -1 ),
        wxTL_DEFAULT_STYLE | wxTL_MULTIPLE );
    m_stateTree->AppendColumn( "Type", wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxCOL_RESIZABLE );
    m_stateTree->AppendColumn( "Name", wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxCOL_RESIZABLE );
    m_stateTree->AppendColumn( "Value", wxCOL_WIDTH_AUTOSIZE, wxALIGN_LEFT, wxCOL_RESIZABLE );

    wxImageList* images = new wxImageList(16, 16);
    images->Add(wxArtProvider::GetBitmap(wxART_FOLDER, wxART_OTHER, wxSize(16,16)));
    images->Add(wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16)));
    m_stateTree->SetImageList(images);

    m_mgr.AddPane( m_notebook, wxAuiPaneInfo().CenterPane().PaneBorder( false ) );
    m_mgr.AddPane( m_output, wxAuiPaneInfo().Bottom().Caption( "Output" ).BestSize( -1, 120 ).MinSize( -1, 60 ).Resizable( true ).CloseButton( false ) );
    m_mgr.AddPane( m_stateTree, wxAuiPaneInfo().Left().Caption( "Glich State" ).BestSize( 250, -1 ).MinSize( 150, -1 ).Resizable( true ).CloseButton( false ) );
    m_mgr.Update();

    // Add initial tab
    m_notebook->AddPage(new geEditor(m_notebook), "Untitled", true);

    UpdateStateTree();
}

geMainFrame::~geMainFrame()
{
    m_mgr.UnInit();
}

void geMainFrame::OnNew(wxCommandEvent&)
{
    m_notebook->AddPage(new geEditor(m_notebook), "Untitled", true);
}

void geMainFrame::OnOpen(wxCommandEvent&)
{
    wxFileDialog dlg(this, "Open file", wxEmptyString, wxEmptyString,
                     "Glich Script (*.glcs)|*.glcs|All Files (*.*)|*.*",
                     wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (dlg.ShowModal() == wxID_OK)
    {
        geEditor* editor = new geEditor(m_notebook);
        if (editor->LoadFile(dlg.GetPath()))
        {
            m_notebook->AddPage(editor, wxFileNameFromPath(dlg.GetPath()), true);
        }
        else
        {
            wxMessageBox("Failed to open file.", "Error", wxICON_ERROR);
            delete editor;
        }
    }
}

void geMainFrame::OnSave(wxCommandEvent&)
{
    int sel = m_notebook->GetSelection();
    if (sel == wxNOT_FOUND) return;
    geEditor* editor = dynamic_cast<geEditor*>(m_notebook->GetPage(sel));
    if (!editor) return;

    wxString path = editor->GetFilename();
    if (path.IsEmpty())
    {
        OnSaveAs(wxCommandEvent());
        return;
    }
    if (editor->SaveFile(path))
    {
        SetStatusText("Saved: " + path);
    }
    else
    {
        wxMessageBox("Failed to save file.", "Error", wxICON_ERROR);
    }
}

void geMainFrame::OnSaveAs(wxCommandEvent&)
{
    int sel = m_notebook->GetSelection();
    if (sel == wxNOT_FOUND) return;
    geEditor* editor = dynamic_cast<geEditor*>(m_notebook->GetPage(sel));
    if (!editor) return;

    wxFileDialog dlg(this, "Save file as", wxEmptyString, wxEmptyString,
                     "Glich Script (*.glcs)|*.glcs|All Files (*.*)|*.*",
                     wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (dlg.ShowModal() == wxID_OK)
    {
        if (editor->SaveFile(dlg.GetPath()))
        {
            m_notebook->SetPageText(sel, wxFileNameFromPath(dlg.GetPath()));
            editor->SetFilename(dlg.GetPath());
            SetStatusText("Saved: " + dlg.GetPath());
        }
        else
        {
            wxMessageBox("Failed to save file.", "Error", wxICON_ERROR);
        }
    }
}

void geMainFrame::OnExit(wxCommandEvent&)
{
    Close(true);
}

// --- Edit command handlers ---
void geMainFrame::OnUndo( wxCommandEvent& )
{
    int sel = m_notebook->GetSelection();
    if( sel == wxNOT_FOUND ) return;
    geEditor* editor = dynamic_cast<geEditor*>(m_notebook->GetPage( sel ));
    if( editor ) editor->Undo();
}

void geMainFrame::OnRedo( wxCommandEvent& )
{
    int sel = m_notebook->GetSelection();
    if( sel == wxNOT_FOUND ) return;
    geEditor* editor = dynamic_cast<geEditor*>(m_notebook->GetPage( sel ));
    if( editor ) editor->Redo();
}

void geMainFrame::OnCut( wxCommandEvent& )
{
    int sel = m_notebook->GetSelection();
    if( sel == wxNOT_FOUND ) return;
    geEditor* editor = dynamic_cast<geEditor*>(m_notebook->GetPage( sel ));
    if( editor ) editor->Cut();
}

void geMainFrame::OnCopy( wxCommandEvent& )
{
    int sel = m_notebook->GetSelection();
    if( sel == wxNOT_FOUND ) return;
    geEditor* editor = dynamic_cast<geEditor*>(m_notebook->GetPage( sel ));
    if( editor ) editor->Copy();
}

void geMainFrame::OnPaste( wxCommandEvent& )
{
    int sel = m_notebook->GetSelection();
    if( sel == wxNOT_FOUND ) return;
    geEditor* editor = dynamic_cast<geEditor*>(m_notebook->GetPage( sel ));
    if( editor ) editor->Paste();
}

void geMainFrame::OnHelpWebsite( wxCommandEvent& )
{
    wxLaunchDefaultBrowser( "https://nickmat.github.io/gliched/index.htm" );
}

void geMainFrame::OnHelpAbout( wxCommandEvent& )
{
#if 0
    wxMessageBox(
        "Gliched IDE\n"
        "Version 1.0\n"
        "Copyright (c) 2023-2026 Nick Matthews\n"
        "https://nickmat.github.io/gliched/index.htm",
        "About Gliched",
        wxOK | wxICON_INFORMATION,
        this
    );
#endif
    wxMessageBox(
        wxString::Format(
            "Gliched IDE version 0.1\n"
            "Built with %s,\n"
                "and Glich library %s\n",
            wxVERSION_STRING,
            glich::hic().version()
        ),
        "About Gliched",
        wxOK | wxICON_INFORMATION,
        this
    );

}

void geMainFrame::OnRun( wxCommandEvent& )
{
    int sel = m_notebook->GetSelection();
    if( sel == wxNOT_FOUND ) return;
    geEditor* editor = dynamic_cast<geEditor*>(m_notebook->GetPage( sel ));
    if( !editor ) return;

    std::string script = editor->GetText().mb_str( wxConvUTF8 );
    std::string result = glich::hic().run_script( script, "module" );
    m_output->SetValue( wxString::FromUTF8( result.c_str() ) );
    UpdateStateTree();
}

void geMainFrame::OnTabChanged( wxAuiNotebookEvent& )
{
    UpdateStatusBar();
}

void geMainFrame::UpdateStateTree()
{
    m_stateTree->DeleteAllItems();

    glich::HicMarkDataVec state = glich::hic().get_hic_data();

    wxTreeListItem root = m_stateTree->GetRootItem();
    if( !root.IsOk() )
        root = m_stateTree->AppendItem( m_stateTree->GetRootItem(), "Glich State" );

    for( const auto& mark : state )
    {
        const char* type = mark.glc.name.empty() ? "root" : "mark";
        wxTreeListItem markItem = m_stateTree->InsertItem( root, wxTLI_LAST, type );
        m_stateTree->SetItemText( markItem, 1, mark.glc.name );

        auto addDataList = [&]( const std::string& label, const auto& list ) {
            if( !list.empty() ) {
                wxTreeListItem cat = m_stateTree->AppendItem( markItem, label );
                for( const auto& data : list ) {
                    wxTreeListItem item = m_stateTree->AppendItem( cat, "" );
                    m_stateTree->SetItemText( item, 0, wxString::FromUTF8( data.name.c_str() ) );
                    m_stateTree->SetItemText( item, 1, wxString::FromUTF8( data.value.c_str() ) );
                }
            }
        };

        addDataList( "object", mark.glc.obj );
        addDataList( "file", mark.glc.file );
        addDataList( "function", mark.glc.fun );
        addDataList( "command", mark.glc.com );
        addDataList( "lexicon", mark.lex );
        addDataList( "grammar", mark.gmr );
        addDataList( "format", mark.fmt );
        addDataList( "scheme", mark.sch );

        if( !mark.glc.var.empty() ) {
            wxTreeListItem cat = m_stateTree->AppendItem( markItem, "variables" );
            for( const auto& data : mark.glc.var ) {
                wxTreeListItem item = m_stateTree->AppendItem( cat, "" );
                m_stateTree->SetItemText( item, 0, wxString::FromUTF8( data.type.c_str() ) );
                m_stateTree->SetItemText( item, 1, wxString::FromUTF8( data.name.c_str() ) );
                m_stateTree->SetItemText( item, 2, wxString::FromUTF8( data.value.c_str() ) );
            }
        }
        if( !mark.glc.global.empty() ) {
            wxTreeListItem cat = m_stateTree->AppendItem( markItem, "globals" );
            for( const auto& data : mark.glc.global ) {
                wxTreeListItem item = m_stateTree->AppendItem( cat, "" );
                m_stateTree->SetItemText( item, 0, wxString::FromUTF8( data.type.c_str() ) );
                m_stateTree->SetItemText( item, 1, wxString::FromUTF8( data.name.c_str() ) );
                m_stateTree->SetItemText( item, 2, wxString::FromUTF8( data.value.c_str() ) );
            }
        }
        if( !mark.glc.constant.empty() ) {
            wxTreeListItem cat = m_stateTree->AppendItem( markItem, "constants" );
            for( const auto& data : mark.glc.constant ) {
                wxTreeListItem item = m_stateTree->AppendItem( cat, "" );
                m_stateTree->SetItemText( item, 0, wxString::FromUTF8( data.type.c_str() ) );
                m_stateTree->SetItemText( item, 1, wxString::FromUTF8( data.name.c_str() ) );
                m_stateTree->SetItemText( item, 2, wxString::FromUTF8( data.value.c_str() ) );
            }
        }
    }
//    m_stateTree->ExpandAll();
}

void geMainFrame::UpdateStatusBar()
{
    int sel = m_notebook->GetSelection();
    if (sel == wxNOT_FOUND)
    {
        SetStatusText("No file");
        return;
    }
    geEditor* editor = dynamic_cast<geEditor*>(m_notebook->GetPage(sel));
    if (editor)
    {
        wxString name = editor->GetFilename();
        if (name.IsEmpty()) name = "Untitled";
        SetStatusText(name);
    }
}
