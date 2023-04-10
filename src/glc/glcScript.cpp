/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/glcScript.cpp
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Run Glich script.
 * Author:      Nick Matthews
 * Website:     https://github.com/nickmat/glich
 * Created:     5th February 2023
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

#include "glcScript.h"

#include "glcFile.h"
#include "glcFunction.h"
#include "glcObject.h"
#include "hicBase.h"
#include "hicElement.h"
#include "hicGregorian.h"
#include "hicHelper.h"
#include "hicScheme.h"
#include "hicScript.h"

#include <cassert>
#include <fstream>
#include <sstream>


using namespace glich;
using std::string;
using std::vector;


STokenStream* Script::s_current_ts = nullptr;

Script::Script( Glich* glc, std::istream& in, std::ostream& out )
    : m_glc( glc ), m_ts( in, out ), m_out( &out ), m_err( &out )
{
    assert( glc != nullptr );
}

bool Script::run()
{
    bool ret = false;
    STokenStream* prev_ts = s_current_ts;
    s_current_ts = &m_ts;
    for( ;;) {
        next_token();
        if( current_token().type() == SToken::Type::End ) {
            ret = true;
            break;
        }
        if( statement() == false ) {
            ret = (m_ts.errors() == 0);
            break;
        }
    }
    s_current_ts = prev_ts;
    return ret;
}

ScriptStore* Script::store() const
{
    return m_glc->get_store();
}

bool Script::statement()
{
    SToken token = current_token();

    if( token.type() == SToken::Type::Name ) {
        string name = token.get_str();
        if( name == "end" ) return false;
        if( name == "mark" ) return do_mark();
        if( name == "clear" ) return do_clear();
        if( name == "if" ) return do_if();
        if( name == "do" ) return do_do();
        if( name == "set" ) return do_set();
        if( name == "let" ) return do_let();
        if( name == "write" ) return do_write();
        if( name == "writeln" ) return do_writeln();
        if( name == "call" ) return do_call();
        if( name == "function" ) return do_function();
        if( name == "command" ) return do_command();
        if( name == "object" ) return do_object();
        if( name == "file" ) return do_file();
        if( name == "scheme" ) return do_scheme();
        if( name == "lexicon" ) return do_lexicon();
        if( name == "grammar" ) return do_grammar();
        if( store()->exists( name ) ) return do_assign( name );
    }
    else if( token.type() == SToken::Type::Semicolon ) {
        return true; // Empty statement
    }
    // Assume error
    return error( "Unrecognised statement." );
}

bool Script::do_mark()
{
    string mark = get_name_or_primary( GetToken::next );
    if( current_token().type() != SToken::Type::Semicolon ) {
        error( "';' expected." );
        return false;
    }
    if( mark.empty() ) {
        error( "Mark name string expected." );
        return false;
    }
    m_glc->add_or_replace_mark( mark );
    return true;
}

bool Script::do_clear()
{
    string mark;
    SToken token = next_token();
    if( token.type() != SToken::Type::Semicolon ) {
        mark = get_name_or_primary( GetToken::current );
        token = current_token();
    }
    if( token.type() != SToken::Type::Semicolon ) {
        error( "';' expected." );
        return false;
    }
    if( !mark.empty() ) {
        m_glc->clear_mark( mark );
    }
    store()->clear();
    return true;
}

bool Script::do_if()
{
    bool done = false, result = false, ok = false;;
    int nested = 0;
    const char* enderr = "if ended unexpectedly.";
    for( ;;) {
        if( !result && !done ) {
            result = expr( GetToken::next ).get_bool( ok );
            if( !ok ) {
                error( "Boolean expression expected." );
                return false;
            }
        }
        SToken token = current_token();
        if( result ) {
            // Run the statements
            for( ;;) {
                if( token.type() == SToken::Type::End ) {
                    error( enderr );
                    return false;
                }
                if( token.type() == SToken::Type::Name ) {
                    string name = token.get_str();
                    if( name == "endif" ) {
                        return true;
                    }
                    if( name == "else" || name == "elseif" ) {
                        break;
                    }
                }
                if( statement() == false ) {
                    return false;
                }
                token = next_token();
            }
            done = true;
            result = false;
        }
        else {
            // move on to the next "elseif" or "else"
            nested = 0;
            for( ;;) {
                if( token.type() == SToken::Type::At ) {
                    token = next_token();
                    token = next_token(); // Ignore function name
                }
                if( token.type() == SToken::Type::End ) {
                    error( enderr );
                    return false;
                }
                if( token.type() == SToken::Type::Name ) {
                    string name = token.get_str();
                    if( name == "if" ) {
                        nested++;
                    }
                    else if( name == "endif" ) {
                        if( nested > 0 ) {
                            --nested;
                        }
                        else {
                            return true;
                        }
                    }
                    else if( name == "else" && nested == 0 ) {
                        next_token();
                        result = true;
                        break;
                    }
                    else if( name == "elseif" && nested == 0 ) {
                        break;
                    }
                }
                token = next_token();
            }
        }
        if( done ) {
            // Skip to "endif"
            nested = 0;
            for( ;;) {
                token = next_token();
                if( token.type() == SToken::Type::End ) {
                    error( enderr );
                    return false;
                }
                if( token.type() == SToken::Type::Name ) {
                    string name = token.get_str();
                    if( name == "if" ) {
                        nested++;
                    }
                    else if( name == "endif" ) {
                        if( nested > 0 ) {
                            --nested;
                        }
                        else {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool Script::do_do()
{
    int start_line = m_ts.get_line();
    string code = m_ts.read_until( "loop", "do" );
    if( code.empty() ) {
        error( "Do loop not terminated." );
        return false;
    }
    int end_line = m_ts.get_line();
    std::istringstream iss( code );
    std::istream* prev_iss = m_ts.reset_in( &iss );
    for( size_t i = 1000; i > 0; --i ) {  // We have a limit of 1000 reiterations
        m_ts.set_line( start_line );
        bool exit = false;
        SToken token = next_token();
        for( ;;) {
            if( token.type() == SToken::Type::End ) {
                exit = true;
                break;
            }
            if( token.type() == SToken::Type::Name ) {
                string name = token.get_str();
                if( name == "until" || name == "while" ) {
                    bool ok = false;
                    exit = expr( GetToken::next ).get_bool( ok );
                    if( !ok ) {
                        error( "Boolean expression expected." );
                        exit = true;
                        break;
                    }
                    if( name == "while" ) {
                        exit = !exit;
                    }
                    if( exit ) {
                        break;
                    }
                    token = current_token();
                    continue;
                }
                if( name == "loop" ) {
                    break;
                }
            }
            if( statement() == false ) {
                exit = true;
                break;
            }
            token = next_token();
        }
        if( exit ) {
            break;
        }
        iss.clear();
        iss.seekg( 0 );
    }
    m_ts.reset_in( prev_iss );
    m_ts.set_line( end_line );
    return true;
}

bool Script::do_set()
{
    string prop;
    SToken token = next_token();
    if( token.type() == SToken::Type::Name ) {
        prop = token.get_str();
    }
    else {
        error( "Set property expected." );
        return false;
    }
    string value = get_name_or_primary( GetToken::next );
    if( current_token().type() != SToken::Type::Semicolon ) {
        error( "set statement is \"set propery value;\"." );
        return false;
    }
    if( prop == "context" ) {
        if( value == "glich" ) {
            m_glc->set_context( Context::glich );
        }
        else if( value == "hics" ) {
            m_glc->set_context( Context::hics );
        }
        else {
            error( "Unknown context value \"" + value + "\"." );
        }
        return true;
    }
    string scode, fcode;
    split_code( &scode, &fcode, value );
    Scheme* sch = dynamic_cast<Scheme*>(m_glc->get_object( scode ));
    if( sch != nullptr ) {
        if( prop == "input" ) {
            m_glc->set_ischeme( sch );
            if( !fcode.empty() ) {
                sch->set_input_format( fcode );
            }
            return true;
        }
        if( prop == "output" ) {
            m_glc->set_oscheme( sch );
            if( !fcode.empty() ) {
                sch->set_output_format( fcode );
            }
            return true;
        }
        if( prop == "inout" ) {
            m_glc->set_ischeme( sch );
            m_glc->set_oscheme( sch );
            if( !fcode.empty() ) {
                sch->set_input_format( fcode );
                sch->set_output_format( fcode );
            }
            return true;
        }

    }
    error( "Set property \"" + prop + "\" not recognised." );
    return false;
}

bool Script::do_let()
{
    SToken token = next_token();
    if( token.type() != SToken::Type::Name ) {
        error( "Variable name expected." );
        return false;
    }
    ScriptStore* vars = store();
    assert( vars != nullptr );
    string name = token.get_str();
    if( !vars->exists( name ) ) {
        store()->set( name, SValue() ); // Put a null SValue into store
    }
    return do_assign( name );
}

bool Script::do_assign( const std::string& name )
{
    SToken token = next_token();
    SValue* vp = store()->get_vp( name );
    assert( vp != nullptr );
    SValue value;

    while( token.type() == SToken::Type::LSbracket ) {
        if( !vp || vp->type() != SValue::Type::Object ) {
            error( "Object type expected." );
            return false;
        }
        value = expr( GetToken::next );
        bool success = false;
        size_t index = value.get_int_as_size_t( success );
        if( !success ) {
            error( "Positive number expected." );
            return false;
        }
        vp = vp->get_object_element( index );
        if( vp == nullptr ) {
            error( "Unable to access object." );
            return false;
        }
        if( current_token().type() != SToken::Type::RSbracket ) {
            error( "']' expected." );
            return false;
        }
        token = next_token();
    }

    if( token.type() == SToken::Type::Equal ) {
        value = expr( GetToken::next );
    }
    else if( store()->get( &value, name ) ) {
        switch( token.type() )
        {
        case SToken::Type::PlusEq:
            value.plus( expr( GetToken::next ) );
            break;
        case SToken::Type::MinusEq:
            value.minus( expr( GetToken::next ) );
            break;
        case SToken::Type::DivideEq:
            value.divide( expr( GetToken::next ) );
            break;
        case SToken::Type::StarEq:
            value.multiply( expr( GetToken::next ) );
            break;
        default:
            error( "Assign operator expected." );
            return false;
        }
    }
    else {
        error( "'=' expected." );
        return false;
    }
    *vp = value;
    if( value.type() == SValue::Type::Error ) {
        m_ts.skip_to( SToken::Type::Semicolon );
    }
    if( current_token().type() != SToken::Type::Semicolon ) {
        error( "';' expected." );
        return false;
    }
    return true;
}

bool Script::do_write( const std::string& term )
{
    SToken token = next_token();
    string filecode;
    if( token.type() == SToken::Type::Dot ) {
        filecode = get_name_or_primary( GetToken::next );
    }
    std::ostream* out = nullptr;
    if( !filecode.empty() ) {
        File* file = m_glc->get_file( filecode );
        if( file == nullptr ) {
            error( "File \"" + filecode + "\" not found." );
            return false;
        }
        out = file->get_out();
    }
    if( out == nullptr ) {
        out = m_out;
    }

    SValue value = expr( GetToken::current );
    *out << value.as_string() << term;

    if( value.type() == SValue::Type::Error ) {
        m_ts.skip_to( SToken::Type::Semicolon );
    }
    if( current_token().type() != SToken::Type::Semicolon ) {
        error( "';' expected." );
        return false;
    }
    return true;
}

Function* Script::create_function()
{
    string code = get_name_or_primary( GetToken::next );
    if( code.empty() ) {
        error( "Function name missing." );
        return nullptr;
    }
    const char* builtin[] = { "if", "read" };
    for( auto bi : builtin ) {
        if( code.compare( bi ) == 0 ) {
            error( "Can not redefine built-in function \"@" + string( bi ) + "\"." );
            return nullptr;
        }
    }
    if( m_glc->get_function( code ) != nullptr ) {
        error( "function \"" + code + "\" already exists." );
        return nullptr;
    }
    SToken token = current_token();
    StdStrVec args;
    SValueVec defs;
    if( token.type() == SToken::Type::Lbracket ) {
        token = next_token();
        for( ;;) {
            if( token.type() == SToken::Type::Rbracket ) {
                break;
            }
            string str = current_token().get_str();
            if( token.type() != SToken::Type::Name || str.empty() ) {
                error( "Argument name expected." );
                return nullptr;
            }
            args.push_back( str );
            token = next_token();
            SValue value;
            if( token.type() == SToken::Type::Equal ) {
                value = expr( GetToken::next );
            }
            defs.push_back( value );
            token = current_token();
            if( token.type() == SToken::Type::Comma ) {
                token = next_token();
            }
        }
        token = next_token();
    }
    if( token.type() != SToken::Type::LCbracket ) {
        error( "'{' expected." );
        return nullptr;
    }
    int line = m_ts.get_line();
    string script = m_ts.read_until( "}", "{" );
    if( script.empty() ) {
        error( "Terminating '}' not found." );
        return nullptr;
    }
    Function* fun = new Function( code );
    if( fun == nullptr ) {
        return nullptr;
    }
    fun->set_args( args );
    fun->set_defaults( defs );
    fun->set_line( line );
    fun->set_script( script );
    return fun;
}

bool glich::Script::do_function()
{
    Function* fun = create_function();
    if( fun == nullptr ) {
        return false;
    }
    return m_glc->add_function( fun );
}

bool glich::Script::do_command()
{
    string code = get_name_or_primary( GetToken::next );
    if( code.empty() ) {
        error( "Command name missing." );
        return false;
    }
    if( m_glc->get_command( code ) != NULL ) {
        error( "command \"" + code + "\" already exists." );
        return false;
    }
    SToken token = current_token();
    StdStrVec args;
    SValueVec defs;
    if( token.type() == SToken::Type::Lbracket ) {
        token = next_token();
        for( ;;) {
            if( token.type() == SToken::Type::Rbracket ) {
                break;
            }
            string str = current_token().get_str();
            if( token.type() != SToken::Type::Name || str.empty() ) {
                error( "Argument name expected." );
                return false;
            }
            args.push_back( str );
            token = next_token();
            SValue value;
            if( token.type() == SToken::Type::Equal ) {
                value = expr( GetToken::next );
            }
            defs.push_back( value );
            token = current_token();
            if( token.type() == SToken::Type::Comma ) {
                token = next_token();
            }
        }
        token = next_token();
    }
    if( token.type() != SToken::Type::LCbracket ) {
        error( "'{' expected." );
        return false;
    }
    int line = m_ts.get_line();
    string script = m_ts.read_until( "}", "{" );
    if( script.empty() ) {
        error( "Terminating '}' not found." );
        return false;
    }
    Command* com = m_glc->create_command( code );
    if( com == nullptr ) {
        return false;
    }
    com->set_args( args );
    com->set_defaults( defs );
    com->set_line( line );
    com->set_script( script );
    return true;
}

bool Script::do_call()
{
    SValue value = command_call();
    if( value.is_error() ) {
        error( value.get_str() );
        return false;
    }
    if( current_token().type() != SToken::Type::Semicolon ) {
        error( "';' expected." );
        return false;
    }
    return true;
}

bool glich::Script::do_object()
{
    string code = get_name_or_primary( GetToken::next );
    if( code.empty() ) {
        error( "Object name missing." );
        return false;
    }
    if( m_glc->get_object( code ) != nullptr ) {
        error( "object \"" + code + "\" already exists." );
        return false;
    }
    if( current_token().type() != SToken::Type::LCbracket ) {
        error( "'{' expected." );
        return false;
    }
    Object* obj = m_glc->create_object( code );
    string str;
    for( ;;) {
        SToken token = next_token();
        if( token.type() == SToken::Type::RCbracket ||
            token.type() == SToken::Type::End ) {
            break; // All done.
        }
        else if( token.type() == SToken::Type::Name ) {
            string name = token.get_str();
            if( name == "values" ) {
                StdStrVec values = get_string_list( GetToken::next );
                obj->set_value_names( values );
            }
            else if( name == "function" ) {
                Function* fun = create_function();
                if( fun == nullptr ) {
                    error( "Unable to create function." );
                    return false;
                }
                obj->add_function( fun );
            }
            else {
                error( "Unknown object subcommand." );
                return false;
            }
        }
    }
    return true;
}

bool Script::do_file()
{
    string code = get_name_or_primary( GetToken::next );
    if( code.empty() ) {
        error( "File code missing." );
        return false;
    }
    string name = expr( GetToken::current ).as_string();
    if( name.empty() ) {
        error( "Filename missing." );
        return false;
    }
    File::FileType type = File::FT_null;
    if( current_token().type() != SToken::Type::Semicolon ) {
        string type_str = get_name_or_primary( GetToken::next );
        if( type_str.empty() ) {
            error( "';' or switch expected." );
            return false;
        }
        if( type_str == "write" ) {
            type = File::FT_write;
        }
        else if( type_str == "append" ) {
            type = File::FT_append;
        }
    }
    if( current_token().type() != SToken::Type::Semicolon ) {
        error( "';' expected." );
        return false;
    }
    File* file = m_glc->create_file( code );
    if( type != File::FT_null ) {
        file->set_filetype( type );
    }
    file->set_filename( name );
    bool ok = file->open();
    if( !ok ) {
        delete file;
        error( "Unable to open file: " + name );
        return false;
    }
    return true;
}

bool Script::do_scheme()
{
    string code = get_name_or_primary( GetToken::next );
    if( code.empty() ) {
        error( "Scheme code missing." );
        return false;
    }
    if( m_glc->get_object( code ) != nullptr ) {
        error( "Scheme \"" + code + "\" already exists." );
        return false;
    }
    Scheme* sch = do_create_scheme( *this, code );
    return m_glc->add_object( sch, code );
}

bool Script::do_lexicon()
{
    string code = get_name_or_primary( GetToken::next );
    if( code.empty() ) {
        error( "Lexicon code missing." );
        return false;
    }
    if( m_glc->get_lexicon( code ) != nullptr ) {
        error( "Lexicon \"" + code + "\" already exists." );
        return false;
    }
    Lexicon* lex = do_create_lexicon( *this, code );
    return m_glc->add_lexicon( lex, code );
}

bool Script::do_grammar()
{
    string code = get_name_or_primary( GetToken::next );
    if( code.empty() ) {
        error( "Grammar code missing." );
        return false;
    }
    if( m_glc->get_grammar( code ) != nullptr ) {
        error( "Grammar \"" + code + "\" already exists." );
        return false;
    }
    Grammar* gmr = do_create_grammar( *this, code, nullptr );
    return m_glc->add_grammar( gmr, code );
}

// If parsing the format in the global space then gmr is a nullptr and
// the Grammar to be used should be encoded into the Format code,
// as in "gmr:fmt"
// If parsing the format within a grammar then gmr is not a nullptr
// and the Format code just contains just the Format code, as in "fmt".
bool glich::Script::do_format( Grammar* gmr )
{
    string code = get_name_or_primary( GetToken::next );
    if( code.empty() ) {
        error( "Format code missing." );
        return false;
    }
    return do_create_format( *this, code, gmr );
}

SValue Script::expr( GetToken get )
{
    SValue left = compare( get );
    for( ;;) {
        SToken token = current_token();
        switch( token.type() )
        {
        case SToken::Type::Or:
            left.logical_or( compare( GetToken::next ) );
            break;
        case SToken::Type::And:
            left.logical_and( compare( GetToken::next ) );
            break;
        default:
            return left;
        }
    }
}

SValue Script::compare( GetToken get )
{
    SValue left = combine( get );
    for( ;;) {
        SToken token = current_token();
        switch( token.type() )
        {
        case SToken::Type::Equal:
            left.equal( combine( GetToken::next ) );
            break;
        case SToken::Type::NotEqual:
            left.equal( combine( GetToken::next ) );
            left.logical_not();
            break;
        case SToken::Type::GtThan:
            left.greater_than( combine( GetToken::next ) );
            break;
        case SToken::Type::GtThanEq:
            left.less_than( combine( GetToken::next ) );
            left.logical_not();
            break;
        case SToken::Type::LessThan:
            left.less_than( combine( GetToken::next ) );
            break;
        case SToken::Type::LessThanEq:
            left.greater_than( combine( GetToken::next ) );
            left.logical_not();
            break;
        default:
            return left;
        }
    }
}

SValue Script::combine( GetToken get )
{
    SValue left = range( get );

    for( ;;) {
        SToken token = current_token();
        switch( token.type() )
        {
        case SToken::Type::UNION:
            left.rlist_union( range( GetToken::next ) );
            break;
        case SToken::Type::INTERSECTION:
            left.intersection( range( GetToken::next ) );
            break;
        case SToken::Type::REL_COMPLEMENT:
            left.rel_complement( range( GetToken::next ) );
            break;
        case SToken::Type::SYM_DIFFERENCE:
            left.sym_difference( range( GetToken::next ) );
            break;
        default:
            return left;
        }
    }
}

SValue Script::range( GetToken get )
{
    SValue left = sum( get );

    for( ;;) {
        SToken token = current_token();
        switch( token.type() )
        {
        case SToken::Type::DotDot:
            left.range_op( sum( GetToken::next ) );
            break;
        default:
            return left;
        }
    }
}

SValue Script::sum( GetToken get )
{
    SValue left = term( get );
    for( ;;) {
        SToken token = current_token();
        switch( token.type() )
        {
        case SToken::Type::Plus:
            left.plus( term( GetToken::next ) );
            break;
        case SToken::Type::Minus:
            left.minus( term( GetToken::next ) );
            break;
        default:
            return left;
        }
    }
}

SValue Script::term( GetToken get )
{
    SValue left = subscript( get );

    for( ;;) {
        SToken token = current_token();
        switch( token.type() )
        {
        case SToken::Type::Star:
            left.multiply( subscript( GetToken::next ) );
            break;
        case SToken::Type::Divide:
            left.divide( subscript( GetToken::next ) );
            break;
        case SToken::Type::Div:
            left.int_div( subscript( GetToken::next ) );
            break;
        case SToken::Type::Mod:
            left.modulus( subscript( GetToken::next ) );
            break;
        default:
            return left;
        }
    }
}

SValue Script::subscript( GetToken get )
{
    SValue left = primary( get );

    for( ;;) {
        SToken token = current_token();
        switch( token.type() )
        {
        case SToken::Type::LSbracket: {
                SValue right;
                token = next_token();
                if( token.type() == SToken::Type::Dot ) {
                    token = next_token();
                    if( token.type() == SToken::Type::Name ) {
                        right.set_str( token.get_str() );
                        next_token();
                    }
                    else {
                        right = expr( GetToken::current );
                    }
                    left.property_op( right );
                }
                else {
                    if( token.type() == SToken::Type::Name ) {
                        right.set_str( token.get_str() );
                        next_token();
                    }
                    else {
                        right = expr( GetToken::current );
                    }
                    left = do_subscript( left, right );
                }
                if( current_token().type() != SToken::Type::RSbracket ) {
                    error( "']' expected." );
                }
                next_token();
            }
            break;
        case SToken::Type::Dot:
            left = do_dot( left, get_name_or_primary( GetToken::next ) );
            break;
        default:
            return left;
        }
    }
}

SValue Script::primary( GetToken get )
{
    SValue value;
    SToken token = (get == GetToken::next) ? next_token() : current_token();
    switch( token.type() )
    {
    case SToken::Type::Number:
    case SToken::Type::Field:
    case SToken::Type::Real:
        value = token.value();
        next_token();
        break;
    case SToken::Type::Qmark:
        value.set_field( f_invalid );
        next_token();
        break;
    case SToken::Type::String:
        value.set_str( token.get_str() );
        next_token();
        break;
    case SToken::Type::Name:
        value = get_value_var( token.get_str() );
        next_token();
        break;
    case SToken::Type::Lbracket:
        value = expr( GetToken::next );
        if( current_token().type() != SToken::Type::Rbracket ) {
            error( "')' expected." );
            break;
        }
        next_token();
        break;
    case SToken::Type::LCbracket:
        value = get_object( GetToken::next );
        next_token();
        break;
    case SToken::Type::text:
        value = text_cast();
        break;
    case SToken::Type::date:
        value = date_cast();
        break;
    case SToken::Type::record:
        value = record_cast();
        break;
    case SToken::Type::element:
        value = element_cast();
        break;
    case SToken::Type::Error:
        value = error_cast();
        break;
    case SToken::Type::At:
        value = function_call();
        break;
    case SToken::Type::Minus:
        value = subscript( GetToken::next );
        value.negate();
        break;
    case SToken::Type::Plus:
        value = subscript( GetToken::next );
        break;
    case SToken::Type::Not:
        value = subscript( GetToken::next );
        value.logical_not();
        break;
    case SToken::Type::COMPLEMENT:
        value = subscript( GetToken::next );
        value.compliment();
        break;
    default:
        value.set_error( "Primary value expected." );
    }
    return value;
}

std::string Script::get_name_or_primary( GetToken get )
{
    string str;
    SToken token = (get == GetToken::next) ? next_token() : current_token();
    if( token.type() == SToken::Type::Name ) {
        str = token.get_str();
        next_token();
    }
    else {
        SValue value = primary( GetToken::current );
        if( value.type() == SValue::Type::String || value.type() == SValue::Type::Number ) {
            str = value.as_string();
        }
    }
    return str;
}

StdStrVec glich::Script::get_string_list( GetToken get )
{
    StdStrVec vec;
    SToken token = (get == GetToken::next) ? next_token() : current_token();
    while( token.type() != SToken::Type::Semicolon &&
        token.type() != SToken::Type::End )
    {
        string str = get_name_or_primary( GetToken::current );
        if( !str.empty() ) {
            vec.push_back( str );
        }
        token = current_token();
        if( token.type() == SToken::Type::Comma ) {
            token = next_token();
        }
    }
    return vec;
}

SValue glich::Script::get_object( GetToken get )
{
    string ocode = get_name_or_primary( get );

    SValueVec vlist;
    SValue vo( ocode );
    vlist.push_back( vo );

    SToken token = current_token();
    bool comma_next = false;
    bool done = false;
    for( ;;) {
        switch( token.type() )
        {
        case SToken::Type::End:
        case SToken::Type::RCbracket:
            if( vlist.size() > 1 && !comma_next ) {
                vlist.push_back( SValue() );
            }
            done = true;
            break;
        case SToken::Type::Comma:
            if( comma_next ) {
                comma_next = false;
            }
            else {
                vlist.push_back( SValue() );
            }
            break;
        default: {
                SValue value = expr( GetToken::current );
                vlist.push_back( value );
            }
            token = current_token();
            comma_next = true;
            continue;
        }
        if( done ) break;
        token = next_token();
    }
    return SValue( vlist );
}

SValue Script::do_subscript( const SValue& left, const SValue& right )
{
    SValue value( left );
    if( left.type() == SValue::Type::Object ) {
        SValueVec values = left.get_object();
        if( values.empty() || values[0].type() != SValue::Type::String ) {
            value.set_error( "Malformed Object." );
            return value;
        }
        string code = values[0].get_str();
        Object* obj = m_glc->get_object( code );
        if( right.type() == SValue::Type::String ) {
            size_t index = obj->get_vindex( right.get_str() );
            if( index < values.size() ) {
                return values[index];
            }
        }
        else {
            bool success = true;
            Num num = right.get_number( success );
            if( success && num >= 0 && num < (values.size() - 1) ) {
                return values[num + 1];
            }
        }
    }
    return SValue();
}

SValue Script::do_dot( const SValue& left, const SValue& right )
{
    string ocode = left.get_object_code();
    if( ocode.empty() ) {
        return SValue( "Object expected.", SValue::Type::Error );
    }
    Object* obj = m_glc->get_object( ocode );
    if( obj == nullptr ) {
        return SValue( "Object not found.", SValue::Type::Error );
    }
    string fcode = right.as_string();
    if( fcode.empty() ) {
        return SValue( "Object function expected.", SValue::Type::Error );
    }
    Function* fun = obj->get_function( fcode );
    if( fun == nullptr ) {
        return SValue( "Function not found.", SValue::Type::Error );
    }
    return run_function( fun, obj, &left );
}

SValue Script::text_cast()
{
    SToken token = next_token();
    Scheme* sch = nullptr;
    string sig, scode, fcode;
    if( token.type() == SToken::Type::Dot ) {
        // Includes scheme:format signiture
        sig = get_name_or_primary( GetToken::next );
        split_code( &scode, &fcode, sig );
        sch = dynamic_cast<Scheme*>(m_glc->get_object( scode ));
    }
    SValue value = primary( GetToken::current );
    if( value.type() == SValue::Type::Object ) {


        Object* obj = value.get_object_ptr();
        if( obj == nullptr ) {
            value.set_error( "Object type not recognised." );
            return value;
        }
        // We ignore any suffix scheme setting
        sch = dynamic_cast<Scheme*>(obj);
        if( obj == nullptr ) {
            value.set_error( "Object is not a scheme." );
            return value;
        }
        SValueVec vals = value.get_object();
        FieldVec fields = sch->get_object_fields( vals );
        Field jdn = sch->get_base().get_jdn( fields );
        value.set_str( sch->jdn_to_str( jdn, fcode ) );
        return value;
    }
    if( sch == nullptr ) {
        sch = m_glc->get_oscheme();
        if( sch == nullptr ) {
            value.set_error( "No default scheme set." );
            return value;
        }
    }
    bool success = false;
    Field jdn = value.get_field( success );
    if( success ) {
        value.set_str( sch->jdn_to_str( jdn, fcode ) );
        return value;
    }
    Range rng = value.get_range( success );
    if( success ) {
        value.set_str( sch->range_to_str( rng, fcode ) );
        return value;
    }
    RList rlist = value.get_rlist( success );
    if( !success ) {
        value.set_error( "Expected field, range, rlist or record type." );
        return value;
    }
    value.set_error( "Not range or rlist yet." );
    value.set_str( sch->rlist_to_str( rlist, fcode ) );
    return value;
}

SValue Script::date_cast()
{
    SToken token = next_token();
    Scheme* sch = nullptr;
    string sig, scode, fcode;
    if( token.type() == SToken::Type::Dot ) {
        // Includes scheme:format signiture
        sig = get_name_or_primary( GetToken::next );
        split_code( &scode, &fcode, sig );
        sch = dynamic_cast<Scheme*>(m_glc->get_object( scode ));
    }
    SValue value = primary( GetToken::current );
    if( value.type() == SValue::Type::Object ) {
        Object* obj = value.get_object_ptr();
        if( obj == nullptr ) {
            value.set_error( "Object type not recognised." );
            return value;
        }
        // We ignore any suffix scheme setting
        sch = dynamic_cast<Scheme*>(obj);
        if( obj == nullptr ) {
            value.set_error( "Object is not a scheme." );
            return value;
        }
        SValueVec vals = value.get_object();
        FieldVec fields = sch->get_object_fields( vals );
        Field fld = sch->get_base().get_jdn( fields );
        value.set_field( fld );
        return value;
    }
    if( value.type() == SValue::Type::String ) {
        if( sch == nullptr ) {
            sch = m_glc->get_ischeme();
            if( sch == nullptr ) {
                value.set_error( "No default scheme set." );
                return value;
            }
            scode = sch->get_code();
        }
        RList rlist = sch->str_to_rlist( value.get_str(), fcode );
        value.set_rlist_demote( rlist );
        return value;
    }
    value.set_error( "Expected an object or string type." );
    return value;
}

SValue Script::record_cast()
{
    SToken token = next_token();
    Scheme* sch = nullptr;
    string sig, scode, fcode;
    if( token.type() == SToken::Type::Dot ) {
        // Includes scheme:format signiture
        sig = get_name_or_primary( GetToken::next );
        split_code( &scode, &fcode, sig );
        sch = dynamic_cast<Scheme*>(m_glc->get_object( scode ));
    }
    SValue value = primary( GetToken::current );
    if( sch == nullptr ) {
        sch = m_glc->get_ischeme();
        if( sch == nullptr ) {
            value.set_error( "No default scheme set." );
            return value;
        }
        scode = sch->get_code();
    }
    if( scode.empty() ) {
        value.set_error( "No scheme set." );
        return value;
    }
    bool success = false;
    Field jdn = value.get_field( success );
    if( success ) {
        FieldVec fields = sch->get_base().get_fields( jdn );
        value.set_object( scode, fields );
        return value;
    }
    if( value.type() == SValue::Type::String ) {

        RList rlist = sch->str_to_rlist( value.get_str(), fcode );
        value.set_rlist_demote( rlist );
        if( value.type() != SValue::Type::field ) {
            value.set_error( "String must reduce to Field type." );
            return value;
        }
        jdn = value.get_field();
        FieldVec fields = sch->get_base().get_fields( jdn );
        value.set_object( scode, fields );
        return value;
    }
    value.set_error( "Expected a field or string type." );
    return value;
}

SValue Script::element_cast()
{
    SToken token = next_token();
    string sig;
    if( token.type() == SToken::Type::Dot ) {
        sig = get_name_or_primary( GetToken::next );
    }
    SValue value = primary( GetToken::current );
    Element ele;
    if( !sig.empty() ) {
        ele.add_char( ':' );
        ele.add_string( sig );
    }
    bool success = false;
    Field fld = value.get_field( success );
    if( success ) {
        value.set_str( ele.get_formatted_element( *m_glc, fld ) );
    }
    else if( value.type() == SValue::Type::String ) {
        value.set_field( ele.get_converted_field( m_glc, value.get_str() ) );
    }
    else {
        value.set_error( "Element requires field like or string type." );
    }
    return value;
}

SValue Script::error_cast()
{
    SValue value = primary( GetToken::next );
    bool ok = false;
    string mess = value.get_str( ok );
    if( !ok ) {
        mess = "Unable to read error message.";
    }
    value.set_error( mess );
    return value;
}

SValueVec Script::get_args( SValue& value, GetToken get )
{
    SToken token = (get == GetToken::next) ? m_ts.next() : current_token();
    SValueVec args;
    if( token.type() == SToken::Type::Lbracket ) {
        for( ;; ) {
            SValue arg = expr( GetToken::next );
            args.push_back( arg );
            token = current_token();
            if( token.type() == SToken::Type::Rbracket ||
                token.type() == SToken::Type::End )
            {
                break;
            }
            if( token.type() != SToken::Type::Comma ) {
                value.set_error( "',' expected." );
                return SValueVec();
            }
        }
        token = next_token();
    }
    return args;
}

SValue Script::function_call()
{
    SToken token = next_token();
    if( token.type() != SToken::Type::Name ) {
        return SValue( "Function name expected.", SValue::Type::Error );
    }
    string name = token.get_str();
    if( name == "if" ) {
        return at_if();
    }
    else if( name == "read" ) {
        return at_read();
    }
    Function* fun = m_glc->get_function( name );
    if( fun == nullptr ) {
        string mess = "Function \"" + name + "\" not found.";
        return SValue( mess.c_str(), SValue::Type::Error);
    }
    return run_function( fun );
}

SValue Script::run_function( Function* fun, const Object* obj, const SValue* left )
{
    SValue value;
    GetToken get = (obj == nullptr) ? GetToken::next : GetToken::current;
    SValueVec args = get_args( value, get );
    if( value.is_error() ) {
        return value;
    }
    STokenStream prev_ts = m_ts;
    m_ts.set_line( fun->get_line() );
    std::istringstream iss( fun->get_script() );
    m_ts.reset_in( &iss );
    m_glc->push_store();

    store()->set( "result", SValue() );
    if( obj != nullptr && left != nullptr && left->type() == SValue::Type::Object ) {
        SValueVec left_values = left->get_object();
        const NameIndexMap& vnames = obj->get_vnames_map();
        for( const auto& vname : vnames ) {
            size_t index = vname.second;
            if( index < left_values.size() ) {
                store()->set( vname.first, left_values[vname.second] );
            }
            else {
                store()->set( vname.first, SValue() );
            }
        }
    }
    for( size_t i = 0; i < fun->get_arg_size(); i++ ) {
        if( i < args.size() ) {
            store()->set( fun->get_arg_name( i ), args[i] );
        }
        else {
            store()->set( fun->get_arg_name( i ), fun->get_default_value( i ) );
        }
    }

    next_token();
    while( statement() ) {
        if( next_token().type() == SToken::Type::End ) {
            break;
        }
    }

    store()->get( &value, "result" );
    m_glc->pop_store();
    m_ts = prev_ts;

    return value;
}

SValue glich::Script::command_call()
{
    SValue value;
    SToken token = next_token();
    if( token.type() != SToken::Type::Name ) {
        value.set_error( "Command name expected." );
        return value;
    }
    string name = token.get_str();

    SValueVec args = get_args( value, GetToken::next );
    if( value.is_error() ) {
        return value;
    }
    Command* com = m_glc->get_command( name );
    if( com == nullptr ) {
        value.set_error( "Command " + name + " not found." );
        return value;
    }

    STokenStream prev_ts = m_ts;
    m_ts.set_line( com->get_line() );
    std::istringstream iss( com->get_script() );
    m_ts.reset_in( &iss );
    m_glc->push_store();

    for( size_t i = 0; i < com->get_arg_size(); i++ ) {
        if( i < args.size() ) {
            store()->set( com->get_arg_name( i ), args[i] );
        }
        else {
            store()->set( com->get_arg_name( i ), com->get_default_value( i ) );
        }
    }

    next_token();
    while( statement() ) {
        if( next_token().type() == SToken::Type::End ) {
            break;
        }
    }

    m_glc->pop_store();
    m_ts = prev_ts;

    return SValue();
}

// Evaluate the built-in @if function:
// @if( c, a, b )
// An arithmetic 'if' expression,
// result is 'a' if 'c' is true, or 'b' if 'c' is false.
// Equivalent to the statement:
// function if(c,a,b) { if c result=a; else result=b; endif }
SValue Script::at_if()
{
    SValue value;
    SValueVec args = get_args( value, GetToken::next );
    if( value.is_error() ) {
        return value;
    }
    if( args.size() != 3 ) {
        value.set_error( "@if requires 3 arguments." );
        return value;
    }
    if( args[0].type() != SValue::Type::Bool ) {
        value.set_error( "1st argument of @if must be a boolean." );
        return value;
    }
    if( args[0].get_bool() ) {
        return args[1];
    }
    return args[2];
}

// Evaluate the built-in @read function:
// @read( prompt = "" )
// If prompt is given, it is output to stdout first,
// then input is read from stdin until a newline.
// The result is always a string.
SValue Script::at_read()
{
    SValue value;
    SValueVec args = get_args( value, GetToken::next );
    if( value.is_error() ) {
        return value;
    }
    string prompt;
    if( args.size() > 0 && args[0].type() == SValue::Type::String ) {
        prompt = args[0].get_str();
    }
    return m_glc->read_input( prompt );
}

SValue Script::get_value_var( const std::string& name )
{
    if( name == "true" ) {
        return SValue( true );
    }
    if( name == "false" ) {
        return SValue( false );
    }
    if( name == "infinity" ) {
        return SValue( f_maximum, SValue::Type::field );
    }
    if( name == "past" ) {
        return SValue( f_minimum, SValue::Type::field );
    }
    if( name == "future" ) {
        return SValue( f_maximum, SValue::Type::field );
    }
    if( name == "today" ) {
        return SValue( Gregorian::today() );
    }
    if( name == "inf" ) {
        return SValue( std::numeric_limits<double>::infinity() );
    }
    if( name == "nan" ) {
        return SValue( std::numeric_limits<double>::quiet_NaN() );
    }
    if( name == "null" ) {
        return SValue();
    }
    if( name == "empty" ) {
        return SValue( RList() );
    }
    SValue value;
    if( store()->get( &value, name ) ) {
        return value;
    }
    value.set_error( "Variable \"" + name + "\" not found." );
    return value;
}

// End of src/glc/glcScript.cpp