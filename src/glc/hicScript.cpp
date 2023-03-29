/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/hicScript.cpp
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

#include "hicScript.h"

#include "glcScript.h"
#include "hicBase.h"
#include "hicGrammar.h"
#include "hicFormatText.h"
#include "hicFormatUnit.h"
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
            else if( token.get_str() == "grammar" ) {
                token = script.next_token();
                if( token.type() == SToken::Type::LCbracket ) {
                    gmr = do_create_grammar( script, "", base );
                    if( gmr == nullptr ) {
                        script.error( "Unable to create grammar." );
                    }
                }
                else {
                    gmr_code = script.get_name_or_primary( GetToken::current );
                    gmr = script.get_glich()->get_grammar( gmr_code );
                }
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
    if( gmr == nullptr ) {
        gmr = Grammar::create_default_grammar( base, script.get_glich() );
    }
    if( !base->attach_grammar( gmr ) ) {
        if( gmr_code.empty() ) {
            delete gmr;
        }
        delete base;
        script.error( "Unable to attach grammar." );
        return nullptr;
    }
    Scheme* sch = new Scheme( code, *base );
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

Grammar* glich::do_create_grammar( Script& script, const std::string& code, const Base* base )
{
    SToken token = script.current_token();
    if( token.type() != SToken::Type::LCbracket ) {
        script.error( "'{' expected." );
        return nullptr;
    }
    Grammar* gmr = new Grammar( code, script.get_glich() );
    string str;
    for( ;;) {
        token = script.next_token();
        if( token.type() == SToken::Type::LCbracket ) {
            continue;
        }
        else if( token.type() == SToken::Type::RCbracket ||
            token.type() == SToken::Type::End ) {
            break; // All done.
        }
        else if( token.type() == SToken::Type::Name ) {
            string name = token.get_str();
            if( name == "lexicons" ) {
                // TODO: do_grammar_vocabs( gmr );
                script.error( "lexicons not yet done." );
            }
            else if( name == "format" ) {
                script.do_format( gmr );
            }
            else if( name == "pref" ) {
                // TODO: str = get_name_or_primary( true );
                //       gmr->set_pref( str );
                script.error( "pref not yet done." );
            }
            else if( name == "calculate" ) {
                // TODO: do_grammar_calculate( gmr );
                script.error( "calculate not yet done." );
            }
            else if( name == "alias" ) {
                // TODO: do_grammar_alias( gmr );
                script.error( "alias not yet done." );
            }
            else if( name == "inherit" ) {
                // TODO: str = get_name_or_primary( true );
                //       gmr->set_inherit( str );
                script.error( "inherit not yet done." );
            }
            else if( name == "fields" ) {
                StdStrVec basefields = script.get_string_list( GetToken::next );
                gmr->set_base_fieldnames( basefields );
            }
            else if( name == "optional" ) {
                // TODO: StringVec optfields = get_string_list( true );
                //       gmr->set_opt_fieldnames( optfields );
                script.error( "optional not yet done." );
            }
            else if( name == "rank" ) {
                // TODO: StringVec rankfields = get_string_list( true );
                //       gmr->set_rank_fieldnames( rankfields );
                script.error( "rank not yet done." );
            }
        }
        else {
            script.error( "Grammar sub-statement expected." );
        }
    }
    gmr->constuct( base );
    if( !gmr->is_ok() ) {
        delete gmr;
        gmr = nullptr;
        script.error( "Unable to construct grammar \"" + code + "\"." );
    }
    return gmr;
}

namespace {

    FormatText* create_format_text( Glich* glc, const string& code, Grammar* gmr )
    {
        size_t pos = code.find( ':' );
        if( pos == string::npos ) {
            if( gmr == nullptr ) {
                return nullptr;
            }
            return gmr->create_format_text( code );
        }
        if( gmr != nullptr ) {
            return nullptr;
        }
        string gcode = code.substr( 0, pos );
        string fcode = code.substr( pos + 1 );
        gmr = glc->get_grammar( gcode );
        if( gmr == nullptr || gmr->get_format( fcode ) != nullptr ) {
            return nullptr;
        }
        FormatText* fmt = new FormatText( fcode, *gmr );
        if( !gmr->add_format( fmt ) ) {
            delete fmt;
            return nullptr;
        }
        glc->add_format( fmt, code );
        return fmt;
    }

    FormatUnit* create_format_unit( Glich* glc, const std::string& code, Grammar* gmr )
    {
        size_t pos = code.find( ':' );
        if( pos == string::npos ) {
            if( gmr == nullptr ) {
                return nullptr;
            }
            return gmr->create_format_unit( code );
        }
        if( gmr != nullptr ) {
            return nullptr;
        }
        string gcode = code.substr( 0, pos );
        string fcode = code.substr( pos + 1 );
        gmr = glc->get_grammar( gcode );
        if( gmr == nullptr || gmr->get_format( fcode ) != nullptr ) {
            return nullptr;
        }
        FormatUnit* fmt = new FormatUnit( fcode, *gmr );
        if( !gmr->add_format( fmt ) ) {
            delete fmt;
            return nullptr;
        }
        glc->add_format( fmt, code );
        return fmt;
    }
}

// If gmr == nullptr then this is a standalone format.
bool glich::do_create_format( Script& script, const string& code, Grammar* gmr )
{
    string format_in, format_out, instring, outstring, separators;
    StdStrVec rankfields, rankoutfields, rules;
    FormatStyle style = FormatStyle::Default;

    if( script.current_token().type() == SToken::Type::LCbracket ) {
        for( ;;) {
            SToken token = script.next_token();
            if( token.type() == SToken::Type::RCbracket ||
                token.type() == SToken::Type::End ) {
                break;
            }
            if( token.type() == SToken::Type::Semicolon ) {
                continue;
            }
            if( token.type() == SToken::Type::Name ) {
                string name = token.get_str();
                if( name == "output" ) {
                    format_out = script.expr( GetToken::next ).as_string();
                    continue;
                }
                if( name == "inout" ) {
                    format_out = script.expr( GetToken::next ).as_string();
                    format_in = format_out;
                    continue;
                }
                if( name == "input" ) {
                    format_in = script.expr( GetToken::next ).as_string();
                    continue;
                }
                if( name == "instring" ) {
                    instring = script.expr( GetToken::next ).as_string();
                    continue;
                }
                if( name == "outstring" ) {
                    outstring = script.expr( GetToken::next ).as_string();
                    continue;
                }
                if( name == "separators" ) {
                    separators = script.expr( GetToken::next ).as_string();
                    continue;
                }
                if( name == "rank" ) {
                    rankfields = script.get_string_list( GetToken::next );
                    continue;
                }
                if( name == "rankout" ) {
                    rankoutfields = script.get_string_list( GetToken::next );
                    continue;
                }
                if( name == "rules" ) {
                    rules = script.get_string_list( GetToken::next );
                    continue;
                }
                if( name == "style" ) {
                    string str = script.get_name_or_primary( GetToken::next );
                    if( str == "hide" ) {
                        style = FormatStyle::Hide;
                    }
                    else if( str != "none" ) {
                        script.error( "Style name expected." );
                    }
                    continue;
                }
                script.error( "Expected format sub-statement." );
            }
        }
    }
    else {
        if( script.current_token().type() == SToken::Type::Comma ) {
            script.next_token();
        }
        format_out = script.expr( GetToken::current ).as_string();
        if( format_out.empty() ) {
            script.error( "Format missing." );
            return false;
        }
        format_in = format_out;
        if( script.current_token().type() != SToken::Type::Semicolon ) {
            script.error( "';' expected." );
            return false;
        }
    }

    Format* fmt = nullptr;
    if( rules.empty() || rules[0] == "text" ) {
        if( format_in.empty() && format_out.empty() ) {
            script.error( "Format string not found." );
            return false;
        }
        FormatText* fmtt = create_format_text( script.get_glich(), code, gmr );
        if( fmtt == nullptr ) {
            script.error( "Unable to create format." );
            return false;
        }
        if( separators.size() ) {
            fmtt->set_separators( separators );
        }
        if( rankfields.size() ) {
            fmtt->set_rank_fieldnames( rankfields );
        }
        if( rankoutfields.size() ) {
            fmtt->set_rankout_fieldnames( rankoutfields );
        }
        if( !format_out.empty() ) {
            fmtt->set_control_out( format_out );
        }
        if( !format_in.empty() ) {
            fmtt->set_control_in( format_in );
        }
        if( !instring.empty() ) {
            fmtt->set_user_input_str( instring );
        }
        if( !instring.empty() ) {
            fmtt->set_user_output_str( outstring );
        }
        fmt = fmtt;
    }
#if 0
    else if( rules[0] == "iso8601" ) {
        fmt = m_cals->create_format_iso( code, gmr, rules );
        if( fmt == nullptr ) {
            error( "Unable to create ISO format." );
            return false;
        }
    }
#endif
    else if( rules[0] == "units" ) {
        fmt = create_format_unit( script.get_glich(), code, gmr );
        if( fmt == nullptr ) {
            script.error( "Unable to create Units format." );
            return false;
        }
    }
    else {
        script.error( "Unknown rules statement." );
        return false;
    }
    fmt->set_style( style );
    return true;
}


// End of src/glc/hicCreateSch.cpp file
