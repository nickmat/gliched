/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/hicGrammar.cpp
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Grammar class to control date formatting.
 * Author:      Nick Matthews
 * Website:     https://github.com/nickmat/glich
 * Created:     24th March 2023
 * Copyright:   Copyright (c) 2023, Nick Matthews.
 * Licence:     GNU GPLv3
 *
 *  Glich is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Glich is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Glich.  If not, see <http://www.gnu.org/licenses/>.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

*/

#include "hicGrammar.h"

#include <glc/glc.h>
#include "hicBase.h"
#include "hicFormatText.h"
#include "hicFormatUnit.h"
#include "hicLexicon.h"

#include <cassert>

using namespace glich;
using std::string;


Grammar::Grammar( const string& code, Glich* glc )
    : m_code( code ), m_glc( glc ), m_ok( false ), m_inherit( nullptr )
{
}

Grammar::~Grammar()
{
}

bool Grammar::constuct( const Base* base )
{
    if( m_ok ) {
        return false; // Only run construct once.
    }
    // TODO:

    // Construct all contained format definitions.
    for( auto pair : m_formats ) {
        pair.second->construct();
    }

    m_ok = true;
    return true;
}

FormatText* Grammar::create_format_text( const string& code )
{
    if( m_formats.count( code ) ) {
        // Already there
        return nullptr;
    }
    FormatText* fmt = new FormatText( code, *this );
    m_formats[code] = fmt;
    return fmt;
}

FormatUnit* Grammar::create_format_unit( const string& code )
{
    if( m_formats.count( code ) ) {
        // Already there
        return nullptr;
    }
    FormatUnit* fmt = new FormatUnit( code, *this );
    m_formats[code] = fmt;
    return fmt;
}

bool Grammar::add_format( Format* fmt )
{
    assert( fmt != NULL );
    string code = fmt->get_code();
    if( m_formats.count( code ) ) {
        // Already there
        return false;
    }
    m_formats[code] = fmt;
#if 0
    // Set preferences to the first format by default.
    if( m_pref_input_fcode.empty() ) {
        m_pref_input_fcode = code;
    }
    if( m_pref_output_fcode.empty() ) {
        m_pref_output_fcode = code;
    }
#endif
    return true;
}

std::string glich::Grammar::resolve_field_alias( const std::string& alias )
{
    if( m_field_alias.count( alias ) > 0 ) {
        return m_field_alias.find( alias )->second;
    }
    if( m_inherit ) {
        return m_inherit->resolve_field_alias( alias );
    }
    return alias;
}

std::string Grammar::resolve_unit_alias( const std::string& alias )
{
    if( m_unit_alias.count( alias ) > 0 ) {
        return m_unit_alias.find( alias )->second;
    }
    if( m_inherit ) {
        return m_inherit->resolve_unit_alias( alias );
    }
    return resolve_field_alias( alias );
}

Format* Grammar::get_format( const string& code ) const
{
    auto it = m_formats.find( code );
    if( it == m_formats.end() ) {
        if( m_inherit ) {
            return m_inherit->get_format( code );
        }
        return nullptr;
    }
    return it->second;
}

Field Grammar::find_token( Lexicon** lex, const std::string& word ) const
{
    Field field = f_invalid;
    for( size_t i = 0; i < m_lexicons.size(); i++ ) {
        field = m_lexicons[i]->find( word );
        if( field != f_invalid ) {
            if( lex ) {
                *lex = m_lexicons[i];
            }
            return field;
        }
    }
    if( m_inherit ) {
        field = m_inherit->find_token( lex, word );
        if( field != f_invalid ) {
            return field;
        }
    }
    if( lex ) {
        *lex = nullptr;
    }
    return f_invalid;
}

Grammar* Grammar::create_default_grammar( const Base* base, Glich* glc )
{
    Grammar* gmr = new Grammar( "", glc );
    gmr->set_base_fieldnames( base->get_fieldnames() );
    // TODO: Add default formats.
    return gmr;
}


// End of src/cal/calgrammar.cpp file
