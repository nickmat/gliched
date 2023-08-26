/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/hicLexicon.cpp
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Lexicon class to handle date element names.
 * Author:      Nick Matthews
 * Website:     https://github.com/nickmat/glich
 * Created:     19th March 2023
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

#include "hicLexicon.h"

#include "hicHelper.h"


using namespace glich;
using std::string;


Lexicon::Lexicon( const std::string& code ) : m_code(code)
{
}

void Lexicon::set_pseudo_names( const StdStrVec & pseudos )
{
    if ( pseudos.size() > 0 ) {
        m_pseudo_name = pseudos[0];
    }
    if ( pseudos.size() > 1 ) {
        m_pseudo_a_name = pseudos[1];
    }
}

void Lexicon::add_token( Field value, const string& name, const string& abbrev )
{
    if( name.empty() ) {
        return;
    }
    Token token( value, name, abbrev );
    string key = make_key( name );
    m_words[key] = token;
    if( abbrev.size() && abbrev != name ) {
        key = make_key( abbrev );
        m_words[key] = token;
    }
    m_fields[value] = token;
}

string Lexicon::get_pseudo_name( Pseudo style ) const
{
    return style == Lexicon::Pseudo::full ? m_pseudo_name : m_pseudo_a_name;
}

void Lexicon::get_info( Lexicon_info* info ) const
{
    if( info ) {
        info->name = m_name;
        info->lang = m_lang;
        info->style_full_name = m_pseudo_name;
        info->style_abbrev_name = m_pseudo_a_name;
        std::map<Field,Token>::const_iterator it;
        for( it = m_fields.begin() ; it != m_fields.end() ; it++ ) {
            Token token = it->second;
            info->words.push_back( token.get_word() );
            string abbrev = token.get_abbrev();
            if( abbrev.empty() ) {
                info->abbrevs.push_back( token.get_word() );
            } else {
                info->abbrevs.push_back( token.get_abbrev() );
            }
        }
    }
}

Field Lexicon::find( const string& word ) const
{
    string key = make_key( word );
    if( m_words.count( key ) > 0 ) {
        Token token = m_words.find( key )->second;
        return token.get_field();
    }
    return f_invalid;
}

string Lexicon::lookup( Field field, Pseudo style ) const
{
    string result;
    if( m_fields.count( field ) > 0 ) {
        Token token = m_fields.find( field )->second;
        if( style == Lexicon::Pseudo::abbrev ) {
            result = token.get_abbrev();
        }
        if( result.empty() ) {
            result = token.get_word();
        }
    }
    return result;
}

// End of src/glc/hicLexicon.cpp file
