/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/hicCreateSch.cpp
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Create Scheme and associated class implimentations.
 * Author:      Nick Matthews
 * Website:     https://github.com/nickmat/glich
 * Created:     17th March 2023
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

#include "hicCreateSch.h"

#include "glcScript.h"
#include "hicLexicon.h"
#include "hicScheme.h"

using namespace glich;
using std::string;

namespace {

    Base* do_base( Script& script, GetToken get )
    {
        Scheme::BaseName bs = Scheme::BaseName::null;
        string data;
        SToken token = (get == GetToken::next) ? script.next_token() : script.current_token();
        if( token.type() == SToken::Type::Name ) {
            string name = token.get_str();
            if( name == "jdn" ) {
                bs = Scheme::BaseName::jdn;
            }
            else if( name == "julian" ) {
                bs = Scheme::BaseName::julian;
            }
            else {
                script.error( "Base scheme not recognised." );
                return nullptr;
            }
            token = script.next_token();
            if( token.type() != SToken::Type::Semicolon ) {
                data = script.get_name_or_primary( GetToken::current );
            }
        }
        else {
            script.error( "Base name expected." );
        }
        return Scheme::create_base( bs, data );
    }

} // namespace

Scheme* glich::do_create_scheme( Script& script, const std::string& code )
{
    SToken token = script.current_token();
    if( token.type() != SToken::Type::LCbracket ) {
        script.error( "'{' expected." );
        return nullptr;
    }
    Base* base = nullptr;
    string name, gmr_code;
    Grammar* gmr = nullptr;
    Scheme_style style = SCH_STYLE_Default;
    for( ;;) {
        token = script.next_token();
        if( token.type() == SToken::Type::RCbracket ||
            token.type() == SToken::Type::End ) {
            break;
        }
        if( token.type() == SToken::Type::Semicolon ) {
            continue;
        }
        if( token.type() == SToken::Type::Name ) {
            if( token.get_str() == "name" ) {
                name = script.expr( GetToken::next ).as_string();
            }
            else if( token.get_str() == "base" ) {
                base = do_base( script, GetToken::next );
            }
            else {
                script.error( "Scheme sub-statement expected." );
            }
        }
    }
    if( base == nullptr ) {
        if( gmr_code.empty() ) {
            delete gmr;
        }
        script.error( "Scheme \"" + code + "\" not created." );
        return nullptr;
    }
    Scheme* sch = new Scheme( code, base );
    sch->set_name( name );
    sch->set_style( style );
    return sch;
}

namespace {

    bool do_lexicon_tokens( Script& script, Lexicon* voc )
    {
        SToken token = script.next_token();
        if( token.type() != SToken::Type::LCbracket ) {
            script.error( "'{' expected." );
            return false;
        }
        while( script.next_token().type() != SToken::Type::RCbracket &&
            script.current_token().type() != SToken::Type::End )
        {
            bool success = false;
            Field number = script.expr( GetToken::current ).get_field( success );
            if( !success ) {
                script.error( "Number expected." );
                return false;
            }
            if( script.current_token().type() != SToken::Type::Comma ) {
                script.error( "',' expected." );
                return false;
            }
            StdStrVec names = script.get_string_list( GetToken::next );
            string name, abbrev;
            if( names.size() > 0 ) {
                name = names[0];
            }
            if( names.size() > 1 ) {
                abbrev = names[1];
            }
            voc->add_token( number, name, abbrev );
        }
        return true;
    }

} // namespace

Lexicon* glich::do_create_lexicon( Script& script, const std::string& code )
{
    SToken token = script.current_token();
    if( token.type() != SToken::Type::LCbracket ) {
        script.error( "'{' expected." );
        return nullptr;
    }
    Lexicon* lex = new Lexicon( code );
    string str;
    for( ;;) {
        token = script.next_token();
        if( token.type() == SToken::Type::RCbracket ||
            token.type() == SToken::Type::End ) {
            break; // All done.
        }
        else if( token.type() == SToken::Type::Name ) {
            string name = token.get_str();
            if( name == "name" ) {
                name = script.expr( GetToken::next ).as_string();
                lex->set_name( str );
            }
            else if( name == "fieldname" ) {
                str = script.get_name_or_primary( GetToken::next );
                lex->set_fieldname( str );
            }
            else if( name == "lang" ) {
                str = script.get_name_or_primary( GetToken::next );
                lex->set_lang( str );
            }
            else if( name == "pseudo" ) {
                StdStrVec pseudos = script.get_string_list( GetToken::next );
                lex->set_pseudo_names( pseudos );
            }
            else if( name == "tokens" ) {
                do_lexicon_tokens( script, lex );
            }
            else {
                script.error( "Unknown lexicon subcommand." );
            }
        }
    }
    return lex;

}

// End of src/glc/hicCreateSch.cpp file
