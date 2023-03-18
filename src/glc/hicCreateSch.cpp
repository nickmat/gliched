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
    Scheme* sch = new Scheme( name, base );
    sch->set_style( style );
    sch->set_code( code );
    return sch;
}


// End of src/glc/hicCreateSch.cpp file
