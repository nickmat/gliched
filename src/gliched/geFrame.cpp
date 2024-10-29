/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/gliched/geFrame.cpp
 * Project:     Gliched: Glich Script Language IDE.
 * Purpose:     Program Main Frame Class Source.
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

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "geFrame.h"

#include "geVersion.h"

#include <glc/hic.h>

using std::string;


geFrame::geFrame(
    const wxString& title, const wxString& filepath )
    : m_title( title ), fbGeFrame( nullptr )
{
    // Set frames Icon
    SetIcon( wxICON( gliched_icon ) );

    m_treeListCtrl->AppendColumn( "Type" );
    m_treeListCtrl->AppendColumn( "Name" );
    m_treeListCtrl->AppendColumn( "Value" );

    UpdateDataTree();

    wxString filename;
    if( !filepath.empty() ) {
        filename = m_edit->DoFileOpen( filepath );
    }
    SetGlichTitle( filename );
}

void geFrame::OnFileNew( wxCommandEvent& event )
{
    m_edit->OnFileNew();
    SetGlichTitle( wxString() );
}

void geFrame::OnFileOpen( wxCommandEvent& event )
{
    wxString filename = m_edit->OnFileOpen();
    SetGlichTitle( filename );
}

void geFrame::OnFileSave( wxCommandEvent& event )
{
    wxString filename = m_edit->OnFileSave();
    SetGlichTitle( filename );
}

void geFrame::OnFileSaveAs( wxCommandEvent& event )
{
    wxString filename = m_edit->OnFileSaveAs();
    SetGlichTitle( filename );
}

void geFrame::OnExit( wxCommandEvent& event )
{
    // Close all windows and exit.
    wxTheApp->Exit();
}

void geFrame::OnEditUndo( wxCommandEvent& event )
{
    m_edit->Undo();
}

void geFrame::OnEditRedo( wxCommandEvent& event )
{
    m_edit->Redo();
}

void geFrame::OnEditCut( wxCommandEvent& event )
{
    m_edit->Cut();
}

void geFrame::OnEditCopy( wxCommandEvent& event )
{
    m_edit->Copy();
}

void geFrame::OnEditPaste( wxCommandEvent& event )
{
    m_edit->Paste();
}

void geFrame::OnEditDelete( wxCommandEvent& event )
{
    m_edit->Clear();
}

void geFrame::OnRun( wxCommandEvent& event )
{
    string script = m_edit->GetText();
    string result = glich::hic().run_script( script );
    m_ctrlResult->SetValue( result );
    UpdateDataTree();
}

void geFrame::OnWebsite( wxCommandEvent& event )
{
    wxLaunchDefaultBrowser( "https://nickmat.github.io/gliched/index.htm" );
}

void geFrame::OnAbout( wxCommandEvent& event )
{
    // Get version of Scintilla
    wxVersionInfo vi = wxStyledTextCtrl::GetLibraryVersionInfo();

    wxMessageBox(
        wxString::Format(
            _( "%s"
                "Built with %s,\n"
                "with %s\n"
                "and Glich library %s\n" ),
            geTitle,
            wxVERSION_STRING,
            vi.GetVersionString(),
            glich::hic().version()
        ),
        _( "About Gliched" ),
        wxOK | wxICON_INFORMATION,
        this
    );
}

void geFrame::SetGlichTitle( const wxString& filename )
{
    if( !filename.empty() ) {
        SetTitle( m_title + " - " + filename );
    }
    else {
        SetTitle( m_title );
    }
}

void geFrame::UpdateDataTree()
{
    m_treeListCtrl->DeleteAllItems();
    glich::HicMarkDataVec tree = glich::hic().get_hic_data();
    wxTreeListItem root = m_treeListCtrl->GetRootItem();
    for( auto& mark : tree ) {
        string type = mark.glc.name.empty() ? "root" : "mark";
        wxTreeListItem item0 = m_treeListCtrl->InsertItem( root, wxTLI_LAST, type );
        wxTreeListItem item1;
        wxTreeListItem item2;
        m_treeListCtrl->SetItemText( item0, 1, mark.glc.name );
        if( !mark.glc.obj.empty() ) {
            item1 = m_treeListCtrl->InsertItem( item0, wxTLI_LAST, "object" );
            for( auto& data : mark.glc.obj ) {
                item2 = m_treeListCtrl->InsertItem( item1, wxTLI_LAST, data.name );
                m_treeListCtrl->SetItemText( item2, 1, data.value );
            }
        }
        if( !mark.glc.file.empty() ) {
            item1 = m_treeListCtrl->InsertItem( item0, wxTLI_LAST, "file" );
            for( auto& data : mark.glc.file ) {
                item2 = m_treeListCtrl->InsertItem( item1, wxTLI_LAST, data.name );
                m_treeListCtrl->SetItemText( item2, 1, data.value );
            }
        }
        if( !mark.glc.fun.empty() ) {
            item1 = m_treeListCtrl->InsertItem( item0, wxTLI_LAST, "function" );
            for( auto& data : mark.glc.fun ) {
                item2 = m_treeListCtrl->InsertItem( item1, wxTLI_LAST, data.name );
                m_treeListCtrl->SetItemText( item2, 1, data.value );
            }
        }
        if( !mark.glc.com.empty() ) {
            item1 = m_treeListCtrl->InsertItem( item0, wxTLI_LAST, "command" );
            for( auto& data : mark.glc.com ) {
                item2 = m_treeListCtrl->InsertItem( item1, wxTLI_LAST, data.name );
                m_treeListCtrl->SetItemText( item2, 1, data.value );
            }
        }
        if( !mark.lex.empty() ) {
            item1 = m_treeListCtrl->InsertItem( item0, wxTLI_LAST, "lexicon" );
            for( auto& data : mark.lex ) {
                item2 = m_treeListCtrl->InsertItem( item1, wxTLI_LAST, data.name );
                m_treeListCtrl->SetItemText( item2, 1, data.value );
            }
        }
        if( !mark.gmr.empty() ) {
            item1 = m_treeListCtrl->InsertItem( item0, wxTLI_LAST, "grammar" );
            for( auto& data : mark.gmr ) {
                item2 = m_treeListCtrl->InsertItem( item1, wxTLI_LAST, data.name );
                m_treeListCtrl->SetItemText( item2, 1, data.value );
            }
        }
        if( !mark.fmt.empty() ) {
            item1 = m_treeListCtrl->InsertItem( item0, wxTLI_LAST, "format" );
            for( auto& data : mark.fmt ) {
                item2 = m_treeListCtrl->InsertItem( item1, wxTLI_LAST, data.name );
                m_treeListCtrl->SetItemText( item2, 1, data.value );
            }
        }
        if( !mark.sch.empty() ) {
            item1 = m_treeListCtrl->InsertItem( item0, wxTLI_LAST, "scheme" );
            for( auto& data : mark.sch ) {
                item2 = m_treeListCtrl->InsertItem( item1, wxTLI_LAST, data.name );
                m_treeListCtrl->SetItemText( item2, 1, data.value );
            }
        }
        if( !mark.glc.var.empty() ) {
            item1 = m_treeListCtrl->InsertItem( item0, wxTLI_LAST, "variables" );
            for( auto& data : mark.glc.var ) {
                item2 = m_treeListCtrl->InsertItem( item1, wxTLI_LAST, data.type );
                m_treeListCtrl->SetItemText( item2, 1, data.name );
                m_treeListCtrl->SetItemText( item2, 2, data.value );
            }
        }
    }
}


// End of src/gliched/gedFrame.cpp file.
