/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/gliched/geFrame.cpp
 * Project:     Gliched: Glich Script Language IDE.
 * Purpose:     Program Main Frame Class Source.
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

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "geFrame.h"

#include <glc/glc.h>

using std::string;


geFrame::geFrame(
    const wxString& title, const wxPoint& pos, const wxSize& size, long style )
    : fbGeFrame( (wxFrame*) nullptr, wxID_ANY, title, pos, size, style )
{
    // Set frames Icon
    SetIcon( wxICON( gliched_icon ) );

    m_treeListCtrl->AppendColumn( "Type" );
    m_treeListCtrl->AppendColumn( "Name" );
    m_treeListCtrl->AppendColumn( "Value" );

    UpdateDataTree();
}

void geFrame::OnFileOpen( wxCommandEvent& event )
{
    m_edit->OnFileOpen();
}

void geFrame::OnFileSave( wxCommandEvent& event )
{
    m_edit->OnFileSave();
}

void geFrame::OnFileSaveAs( wxCommandEvent& event )
{
    m_edit->OnFileSaveAs();
}

void geFrame::OnExit( wxCommandEvent& event )
{
    // Close all windows and exit.
    wxTheApp->Exit();
}

void geFrame::OnRun( wxCommandEvent& event )
{
    string script = m_edit->GetText();
    string result = glich::get_glc()->run_script( script );
    m_ctrlResult->SetValue( result );
    UpdateDataTree();
}

void geFrame::UpdateDataTree()
{
    m_treeListCtrl->DeleteAllItems();
    glich::GlcMarkVec tree = glich::get_glc()->get_glc_data();
    wxTreeListItem root = m_treeListCtrl->GetRootItem();
    for( auto& mark : tree ) {
        string type = mark.name.empty() ? "root" : "mark";
        if( mark.name.empty() ) {}
        wxTreeListItem item0 = m_treeListCtrl->InsertItem( root, wxTLI_LAST, type );
        wxTreeListItem item1;
        wxTreeListItem item2;
        m_treeListCtrl->SetItemText( item0, 1, mark.name );
        if( !mark.obj.empty() ) {
            item1 = m_treeListCtrl->InsertItem( item0, wxTLI_LAST, "object" );
            for( auto& data : mark.obj ) {
                item2 = m_treeListCtrl->InsertItem( item1, wxTLI_LAST, data.name );
                m_treeListCtrl->SetItemText( item2, 1, data.value );
            }
        }
        if( !mark.file.empty() ) {
            item1 = m_treeListCtrl->InsertItem( item0, wxTLI_LAST, "file" );
            for( auto& data : mark.file ) {
                item2 = m_treeListCtrl->InsertItem( item1, wxTLI_LAST, data.name );
                m_treeListCtrl->SetItemText( item2, 1, data.value );
            }
        }
        if( !mark.fun.empty() ) {
            item1 = m_treeListCtrl->InsertItem( item0, wxTLI_LAST, "function" );
            for( auto& data : mark.fun ) {
                item2 = m_treeListCtrl->InsertItem( item1, wxTLI_LAST, data.name );
                m_treeListCtrl->SetItemText( item2, 1, data.value );
            }
        }
        if( !mark.com.empty() ) {
            item1 = m_treeListCtrl->InsertItem( item0, wxTLI_LAST, "command" );
            for( auto& data : mark.com ) {
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
        if( !mark.var.empty() ) {
            item1 = m_treeListCtrl->InsertItem( item0, wxTLI_LAST, "variables" );
            for( auto& data : mark.var ) {
                item2 = m_treeListCtrl->InsertItem( item1, wxTLI_LAST, data.type );
                m_treeListCtrl->SetItemText( item2, 1, data.name );
                m_treeListCtrl->SetItemText( item2, 2, data.value );
            }
        }
    }
}


// End of src/gliched/gedFrame.cpp file.
