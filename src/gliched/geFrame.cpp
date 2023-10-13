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
    : m_lang_index( geLang_glich ), /*m_lang(&g_LanguagePrefs[geLang_glich]),
    m_LineNrID( 0 ), m_LineNrMargin( m_ctrlEditSTC->TextWidth( wxSTC_STYLE_LINENUMBER, "_99999" ) ),
    m_DividerID( 1 ),
    m_FoldingID( 2 ), m_FoldingMargin( FromDIP( 16 ) ),
    m_calltipNo( 1 ),*/
    fbGeFrame( (wxFrame*) nullptr, wxID_ANY, title, pos, size, style )
{
    // Set frames Icon
    SetIcon( wxICON( gliched_icon ) );

    m_treeListCtrl->AppendColumn( "Type" );
    m_treeListCtrl->AppendColumn( "Name" );
    m_treeListCtrl->AppendColumn( "Value" );

    UpdateDataTree();

//    m_ctrlEditSTC->Bind( wxEVT_STC_MARGINCLICK, &geFrame::OnMarginClick, this );

    InitializePrefs( m_lang_index );

#if 0
    // set visibility
    m_ctrlEditSTC->SetVisiblePolicy( wxSTC_VISIBLE_STRICT | wxSTC_VISIBLE_SLOP, 1 );
    m_ctrlEditSTC->SetXCaretPolicy( wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1 );
    m_ctrlEditSTC->SetYCaretPolicy( wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1 );

    // annotations
    m_ctrlEditSTC->AnnotationSetVisible( wxSTC_ANNOTATION_BOXED );

    // autocompletion
    wxBitmap bmp( hashtag_xpm );
    m_ctrlEditSTC->RegisterImage( 0, bmp );

    // call tips
    m_ctrlEditSTC->CallTipSetBackground( *wxYELLOW );

    // miscellaneous
    m_ctrlEditSTC->CmdKeyClear( wxSTC_KEY_TAB, 0 ); // this is done by the menu accelerator key
    m_ctrlEditSTC->SetLayoutCache( wxSTC_CACHE_PAGE );
    m_ctrlEditSTC->UsePopUp( wxSTC_POPUP_ALL );
#endif
}

void geFrame::OnFileOpen( wxCommandEvent& event )
{
    if( !m_filename ) {
        wxFileDialog dlg( this, "Open file", wxEmptyString, wxEmptyString,
            "Any file (*)|*", wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR );
        if( dlg.ShowModal() != wxID_OK ) {
            return;
        }
        m_filename = dlg.GetPath();
    }
    DoFileOpen( m_filename );
}

void geFrame::OnFileSave( wxCommandEvent& event )
{
    if( m_filepath.empty() ) {
        OnFileSaveAs( event );
    }
    DoFileSave( m_filepath );
}

void geFrame::OnFileSaveAs( wxCommandEvent& event )
{
    wxFileDialog dlg( this, "Save file", wxString(), wxString(), m_lang->default_fp, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if( dlg.ShowModal() != wxID_OK ) return;
    m_filepath = dlg.GetPath();
    m_filename = wxFileNameFromPath( m_filepath );
    DoFileSave( m_filepath );
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
            item1 = m_treeListCtrl->InsertItem( item0, wxTLI_LAST, "varibles" );
            for( auto& data : mark.var ) {
                item2 = m_treeListCtrl->InsertItem( item1, wxTLI_LAST, data.type );
                m_treeListCtrl->SetItemText( item2, 1, data.name );
                m_treeListCtrl->SetItemText( item2, 2, data.value );
            }
        }
    }
}

void geFrame::OnMarginClick( wxStyledTextEvent& event )
{
#if 0
    if( event.GetMargin() == 2 ) {
        int lineClick = m_ctrlEditSTC->LineFromPosition( event.GetPosition() );
        int levelClick = m_ctrlEditSTC->GetFoldLevel( lineClick );
        if( (levelClick & wxSTC_FOLDLEVELHEADERFLAG) > 0 ) {
            m_ctrlEditSTC->ToggleFold( lineClick );
        }
    }
#endif
}

geLang geFrame::DeterminePrefs( const wxString& filename )
{
    // determine language from filepatterns
    for( size_t i = 0; i < geLang_size; i++ ) {
        wxString filepattern = g_lang_prefs[i].filepattern;
        filepattern.Lower();
        while( !filepattern.empty() ) {
            wxString cur = filepattern.BeforeFirst( ';' );
            if( (cur == filename) ||
                (cur == (filename.BeforeLast( '.' ) + ".*")) ||
                (cur == ("*." + filename.AfterLast( '.' ))) ) {
                return static_cast<geLang>(i);
            }
            filepattern = filepattern.AfterFirst( ';' );
        }
    }
    return geLang_text;
}

bool geFrame::InitializePrefs( geLang index )
{
#if 0
    // initialize styles
    m_ctrlEditSTC->StyleClearAll();

    m_lang = &g_LanguagePrefs[index];
//    LanguageInfo const* curInfo = nullptr;

    // determine language
    bool found = false;
    if( !found ) {
        return false;
    }

    // set lexer and language
    m_ctrlEditSTC->SetLexer( m_lang->lexer );

    // set margin for line numbers
    m_ctrlEditSTC->SetMarginType( m_LineNrID, wxSTC_MARGIN_NUMBER );
    m_ctrlEditSTC->StyleSetForeground( wxSTC_STYLE_LINENUMBER, wxColour( "DARK GREY" ) );
    m_ctrlEditSTC->StyleSetBackground( wxSTC_STYLE_LINENUMBER, *wxWHITE );
    m_ctrlEditSTC->SetMarginWidth( m_LineNrID, m_LineNrMargin ); // start out visible

    // annotations style
    m_ctrlEditSTC->StyleSetBackground( ANNOTATION_STYLE, wxColour( 244, 220, 220 ) );
    m_ctrlEditSTC->StyleSetForeground( ANNOTATION_STYLE, *wxBLACK );
    m_ctrlEditSTC->StyleSetSizeFractional( ANNOTATION_STYLE,
        (m_ctrlEditSTC->StyleGetSizeFractional( wxSTC_STYLE_DEFAULT ) * 4) / 5 );

    // default fonts for all styles!
    int Nr;
    for( Nr = 0; Nr < wxSTC_STYLE_LASTPREDEFINED; Nr++ ) {
        wxFont font( wxFontInfo( 10 ).Family( wxFONTFAMILY_MODERN ) );
        m_ctrlEditSTC->StyleSetFont( Nr, font );
    }

    // set common styles
    m_ctrlEditSTC->StyleSetForeground( wxSTC_STYLE_DEFAULT, wxColour( "DARK GREY" ) );
    m_ctrlEditSTC->StyleSetForeground( wxSTC_STYLE_INDENTGUIDE, wxColour( "DARK GREY" ) );

    // initialize settings
    if( g_CommonPrefs.syntaxEnable ) {
        int keywordnr = 0;
        for( Nr = 0; Nr < STYLE_TYPES_COUNT; Nr++ ) {
            if( m_lang->styles[Nr].type == -1 ) continue;
            const StyleInfo& curType = g_StylePrefs[m_lang->styles[Nr].type];
            wxFont font( wxFontInfo( curType.fontsize )
                .Family( wxFONTFAMILY_MODERN )
                .FaceName( curType.fontname ) );
            m_ctrlEditSTC->StyleSetFont( Nr, font );
            if( curType.foreground.length() ) {
                m_ctrlEditSTC->StyleSetForeground( Nr, wxColour( curType.foreground ) );
            }
            if( curType.background.length() ) {
                m_ctrlEditSTC->StyleSetBackground( Nr, wxColour( curType.background ) );
            }
            m_ctrlEditSTC->StyleSetBold( Nr, (curType.fontstyle & mySTC_STYLE_BOLD) > 0 );
            m_ctrlEditSTC->StyleSetItalic( Nr, (curType.fontstyle & mySTC_STYLE_ITALIC) > 0 );
            m_ctrlEditSTC->StyleSetUnderline( Nr, (curType.fontstyle & mySTC_STYLE_UNDERL) > 0 );
            m_ctrlEditSTC->StyleSetVisible( Nr, (curType.fontstyle & mySTC_STYLE_HIDDEN) == 0 );
            m_ctrlEditSTC->StyleSetCase( Nr, curType.lettercase );
            const char* pwords = m_lang->styles[Nr].words;
            if( pwords ) {
                m_ctrlEditSTC->SetKeyWords( keywordnr, pwords );
                keywordnr += 1;
            }
        }
    }

    // set margin as unused
    m_ctrlEditSTC->SetMarginType( m_DividerID, wxSTC_MARGIN_SYMBOL );
    m_ctrlEditSTC->SetMarginWidth( m_DividerID, 0 );
    m_ctrlEditSTC->SetMarginSensitive( m_DividerID, false );

    // folding
    m_ctrlEditSTC->SetMarginType( m_FoldingID, wxSTC_MARGIN_SYMBOL );
    m_ctrlEditSTC->SetMarginMask( m_FoldingID, wxSTC_MASK_FOLDERS );
    m_ctrlEditSTC->StyleSetBackground( m_FoldingID, *wxWHITE );
    m_ctrlEditSTC->SetMarginWidth( m_FoldingID, 0 );
    m_ctrlEditSTC->SetMarginSensitive( m_FoldingID, false );
    if( g_CommonPrefs.foldEnable ) {
        m_ctrlEditSTC->SetMarginWidth( m_FoldingID, m_lang->folds != 0 ? m_FoldingMargin : 0 );
        m_ctrlEditSTC->SetMarginSensitive( m_FoldingID, m_lang->folds != 0 );
        m_ctrlEditSTC->SetProperty( "fold", m_lang->folds != 0 ? "1" : "0" );
        m_ctrlEditSTC->SetProperty( "fold.comment",
            (m_lang->folds & mySTC_FOLD_COMMENT) > 0 ? "1" : "0" );
        m_ctrlEditSTC->SetProperty( "fold.compact",
            (m_lang->folds & mySTC_FOLD_COMPACT) > 0 ? "1" : "0" );
        m_ctrlEditSTC->SetProperty( "fold.preprocessor",
            (m_lang->folds & mySTC_FOLD_PREPROC) > 0 ? "1" : "0" );
        m_ctrlEditSTC->SetProperty( "fold.html",
            (m_lang->folds & mySTC_FOLD_HTML) > 0 ? "1" : "0" );
        m_ctrlEditSTC->SetProperty( "fold.html.preprocessor",
            (m_lang->folds & mySTC_FOLD_HTMLPREP) > 0 ? "1" : "0" );
        m_ctrlEditSTC->SetProperty( "fold.comment.python",
            (m_lang->folds & mySTC_FOLD_COMMENTPY) > 0 ? "1" : "0" );
        m_ctrlEditSTC->SetProperty( "fold.quotes.python",
            (m_lang->folds & mySTC_FOLD_QUOTESPY) > 0 ? "1" : "0" );
    }
    m_ctrlEditSTC->SetFoldFlags( wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED |
        wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED );

    // others
    m_ctrlEditSTC->SetIndentationGuides( g_CommonPrefs.indentGuideEnable );
    m_ctrlEditSTC->SetEdgeColumn( 80 );
    m_ctrlEditSTC->SetEdgeMode( g_CommonPrefs.longLineOnEnable ? wxSTC_EDGE_LINE : wxSTC_EDGE_NONE );
    m_ctrlEditSTC->SetOvertype( g_CommonPrefs.overTypeInitial );
    m_ctrlEditSTC->SetReadOnly( g_CommonPrefs.readOnlyInitial );
    m_ctrlEditSTC->SetWrapMode( g_CommonPrefs.wrapModeInitial ?
        wxSTC_WRAP_WORD : wxSTC_WRAP_NONE );
#endif
    return true;
}

bool geFrame::DoFileOpen( wxString filename )
{
    // load file in edit and clear undo
    if( !filename.empty() ) {
        m_filename = filename;
    }
    m_edit->LoadFile( m_filename );

    m_edit->EmptyUndoBuffer();

    // determine lexer language
    wxFileName fname( m_filename );
    InitializePrefs( DeterminePrefs( fname.GetFullName() ) );

    return true;
}

bool geFrame::DoFileSave( wxString filename )
{
    return m_edit->SaveFile( filename );
}

// End of src/gliched/gedFrame.cpp file.
