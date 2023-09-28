/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/gliched/gedApp.cpp
 * Project:     Gliched: Glich Script Language IDE.
 * Purpose:     Application main and supporting functions.
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

#include "gedFrame.h"

#include <glc/glc.h>

#include <wx/stc/stc.h>

using std::string;

#define MY_FOLDMARGIN 2

gedFrame::gedFrame(
    const wxString& title, const wxPoint& pos, const wxSize& size, long style )
    : fbGedFrame( (wxFrame*) nullptr, wxID_ANY, title, pos, size, style )
{
    // Set frames Icon
    SetIcon( wxICON( gliched_icon ) );

    m_treeListCtrl->AppendColumn( "Type" );
    m_treeListCtrl->AppendColumn( "Name" );
    m_treeListCtrl->AppendColumn( "Value" );

    UpdateDataTree();

    //set Lexer to LEX_CONTAINER: This will trigger the styleneeded event so you can do your own highlighting
    m_activeSTC->SetLexer( wxSTC_LEX_CONTAINER );
    //folding example by New Pagodi copied from WxWidgets Forum
    //Set the fold marging to have a width of 14 pixels and give it a
    //distinctive background
    m_activeSTC->SetMarginWidth( MY_FOLDMARGIN, 14 );
    m_activeSTC->SetMarginMask( MY_FOLDMARGIN, wxSTC_MASK_FOLDERS );
    m_activeSTC->SetFoldMarginColour( true, wxColor( 255, 255, 255 ) );
    m_activeSTC->SetFoldMarginHiColour( true, wxColor( 233, 233, 233 ) );
    //Set up the markers that will be shown in the fold margin
    m_activeSTC->MarkerDefine( wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUSCONNECTED );
    m_activeSTC->MarkerSetForeground( wxSTC_MARKNUM_FOLDEREND, wxColor( 243, 243, 243 ) );
    m_activeSTC->MarkerSetBackground( wxSTC_MARKNUM_FOLDEREND, wxColor( 128, 128, 128 ) );
    m_activeSTC->MarkerDefine( wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUSCONNECTED );
    m_activeSTC->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPENMID, wxColor( 243, 243, 243 ) );
    m_activeSTC->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPENMID, wxColor( 128, 128, 128 ) );
    m_activeSTC->MarkerDefine( wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_TCORNER );
    m_activeSTC->MarkerSetForeground( wxSTC_MARKNUM_FOLDERMIDTAIL, wxColor( 243, 243, 243 ) );
    m_activeSTC->MarkerSetBackground( wxSTC_MARKNUM_FOLDERMIDTAIL, wxColor( 128, 128, 128 ) );
    m_activeSTC->MarkerDefine( wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_LCORNER );
    m_activeSTC->MarkerSetForeground( wxSTC_MARKNUM_FOLDERTAIL, wxColor( 243, 243, 243 ) );
    m_activeSTC->MarkerSetBackground( wxSTC_MARKNUM_FOLDERTAIL, wxColor( 128, 128, 128 ) );
    m_activeSTC->MarkerDefine( wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_VLINE );
    m_activeSTC->MarkerSetForeground( wxSTC_MARKNUM_FOLDERSUB, wxColor( 243, 243, 243 ) );
    m_activeSTC->MarkerSetBackground( wxSTC_MARKNUM_FOLDERSUB, wxColor( 128, 128, 128 ) );
    m_activeSTC->MarkerDefine( wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS );
    m_activeSTC->MarkerSetForeground( wxSTC_MARKNUM_FOLDER, wxColor( 243, 243, 243 ) );
    m_activeSTC->MarkerSetBackground( wxSTC_MARKNUM_FOLDER, wxColor( 128, 128, 128 ) );
    m_activeSTC->MarkerDefine( wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS );
    m_activeSTC->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPEN, wxColor( 243, 243, 243 ) );
    m_activeSTC->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPEN, wxColor( 128, 128, 128 ) );
    //Turn the fold markers red when the caret is a line in the group (optional)
    m_activeSTC->MarkerEnableHighlight( true );
    //The margin will only respond to clicks if it set sensitive.  Also, connect
    //the event handler that will do the collapsing/restoring
    m_activeSTC->SetMarginSensitive( MY_FOLDMARGIN, true );
    m_activeSTC->Bind( wxEVT_STC_MARGINCLICK, &myFrame::onMarginClick, this );
    m_activeSTC->Bind( wxEVT_STC_STYLENEEDED, &myFrame::OnStyleNeeded, this );
    //set color for G-Code highlighting
    m_activeSTC->StyleSetForeground( 19, m_GCodecolor );
    //enter some text and set fold levels
    m_activeSTC->AppendText( ";Foldtest\n" );
    m_activeSTC->AppendText( "DEF BOOL TESTVAR=FALSE\n" );
    m_activeSTC->AppendText( "IF (TESTVAR)\n" );
    m_activeSTC->AppendText( "  G0 G54 M0\n" );
    m_activeSTC->AppendText( "ENDIF\n" );
    m_activeSTC->AppendText( "M30\n" );
    /*given the text above, folding should produce this output:
    m_activeSTC->SetFoldLevel(0, 1024);
    m_activeSTC->SetFoldLevel(1, 1024);
    m_activeSTC->SetFoldLevel(2, 1024|wxSTC_FOLDLEVELHEADERFLAG); //header flag: one item before increasing fold level!
    m_activeSTC->SetFoldLevel(3, 1025);  //here comes the new fold level in line G0 G54 M0
    m_activeSTC->SetFoldLevel(4, 1025);  //the ENDIF
    m_activeSTC->SetFoldLevel(5, 1024);  //and this has the lower fold level again
    m_activeSTC->SetFoldLevel(6, 1024|wxSTC_FOLDLEVELWHITEFLAG); //this is an empty line: set fold level white flag
    note: If you load text into the styled text control for example with file load or like we did above:
    style the whole document for once. If you don't, the document remains unstyled until you click some position below*/
    wxString text = m_activeSTC->GetText().Upper(); //Upper() makes it case insensitive
    this->highlightSTCsyntax( 0, m_activeSTC->GetTextLength(), text );
    this->setfoldlevels( 0, 0, text );

}

void gedFrame::OnExit( wxCommandEvent& event )
{
    // Close all windows and exit.
    wxTheApp->Exit();
}

void gedFrame::OnRun( wxCommandEvent& event )
{
    string script = m_ctrlEdit->GetText();
    string result = glich::get_glc()->run_script( script );
    m_ctrlResult->SetValue( result );
    UpdateDataTree();
}

void gedFrame::UpdateDataTree()
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
            item1 = m_treeListCtrl->InsertItem( item0, wxTLI_LAST, "varibles" );
            for( auto& data : mark.var ) {
                item2 = m_treeListCtrl->InsertItem( item1, wxTLI_LAST, data.type );
                m_treeListCtrl->SetItemText( item2, 1, data.name );
                m_treeListCtrl->SetItemText( item2, 2, data.value );
            }
        }
    }
}

// End of src/gliched/gedFrame.cpp file.
