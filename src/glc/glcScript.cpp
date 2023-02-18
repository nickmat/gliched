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

#include <cassert>
#include <fstream>
#include <sstream>


using namespace glich;
using std::string;
using std::vector;


STokenStream* Script::s_current_ts = nullptr;

Script::Script( Glich* db, std::istream& in, std::ostream& out )
    : m_db(db), m_ts(in,out), m_out(&out), m_err(&out)
{
    assert( db != nullptr );
}

bool Script::run()
{
    bool ret = false;
    STokenStream* prev_ts = s_current_ts;
    s_current_ts = &m_ts;
    for( ;;) {
        m_ts.next();
        if( m_ts.current().type() == SToken::Type::End ) {
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
    return m_db->get_store();
}

bool Script::statement()
{
    SToken token = m_ts.current();

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
        if( name == "file" ) return do_file();
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
    string mark = get_name_or_primary( true );
    if( m_ts.current().type() != SToken::Type::Semicolon ) {
        error( "';' expected." );
        return false;
    }
    if( mark.empty() ) {
        error( "Mark name string expected." );
        return false;
    }
    m_db->add_or_replace_mark( mark );
    return true;
}

bool Script::do_clear()
{
    string mark;
    SToken token = m_ts.next();
    if( token.type() != SToken::Type::Semicolon ) {
        mark = get_name_or_primary( false );
        token = m_ts.current();
    }
    if( token.type() != SToken::Type::Semicolon ) {
        error( "';' expected." );
        return false;
    }
    if( !mark.empty() ) {
        m_db->clear_mark( mark );
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
            result = expr( true ).get_bool( ok );
            if( !ok ) {
                error( "Boolean expression expected." );
                return false;
            }
        }
        SToken token = m_ts.current();
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
                token = m_ts.next();
            }
            done = true;
            result = false;
        }
        else {
            // move on to the next "elseif" or "else"
            nested = 0;
            for( ;;) {
                if( token.type() == SToken::Type::At ) {
                    token = m_ts.next();
                    token = m_ts.next(); // Ignore function name
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
                        m_ts.next();
                        result = true;
                        break;
                    }
                    else if( name == "elseif" && nested == 0 ) {
                        break;
                    }
                }
                token = m_ts.next();
            }
        }
        if( done ) {
            // Skip to "endif"
            nested = 0;
            for( ;;) {
                token = m_ts.next();
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
        SToken token = m_ts.next();
        for( ;;) {
            if( token.type() == SToken::Type::End ) {
                exit = true;
                break;
            }
            if( token.type() == SToken::Type::Name ) {
                string name = token.get_str();
                if( name == "until" || name == "while" ) {
                    bool ok = false;
                    exit = expr( true ).get_bool( ok );
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
                    token = m_ts.current();
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
            token = m_ts.next();
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
    SToken token = m_ts.next();
    if( token.type() == SToken::Type::Name ) {
        prop = token.get_str();
    }
    else {
        error( "Set property expected." );
        return false;
    }
    string value = get_name_or_primary( true );
    if( m_ts.current().type() != SToken::Type::Semicolon ) {
        error( "set statement is \"set propery value;\"." );
        return false;
    }
    if( prop == "context" ) {
        if( value == "number" ) {
            m_db->set_context( Context::number );
        }
        else if( value == "field" ) {
            m_db->set_context( Context::field );
        }
        else {
            error( "Unknown context value \"" + value + "\"." );
        }
    }
    else {
        error( "Set property \"" + prop + "\" not recognised." );
    }
    return true;
}

bool Script::do_let()
{
    SToken token = m_ts.next();
    if( token.type() != SToken::Type::Name ) {
        error( "Variable name expected." );
        return false;
    }
    return do_assign( token.get_str() );
}

bool Script::do_assign( const std::string& name )
{
    SToken token = m_ts.next();
    SValue value;
    if( token.type() == SToken::Type::Equal ) {
        value = expr( true );
    }
    else if( store()->get( &value, name ) ) {
        switch( token.type() )
        {
        case SToken::Type::PlusEq:
            value.plus( expr( true ) );
            break;
        case SToken::Type::MinusEq:
            value.minus( expr( true ) );
            break;
        case SToken::Type::DivideEq:
            value.divide( expr( true ) );
            break;
        case SToken::Type::StarEq:
            value.multiply( expr( true ) );
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
    store()->set( name, value );
    if( value.type() == SValue::Type::Error ) {
        m_ts.skip_to( SToken::Type::Semicolon );
    }
    if( m_ts.current().type() != SToken::Type::Semicolon ) {
        error( "';' expected." );
        return false;
    }
    return true;
}

bool Script::do_write( const std::string& term )
{
    SToken token = m_ts.next();
    string filecode;
    if( token.type() == SToken::Type::Dot ) {
        filecode = get_name_or_primary( true );
    }
    std::ostream* out = nullptr;
    if( !filecode.empty() ) {
        File* file = m_db->get_file( filecode );
        if( file == nullptr ) {
            error( "File \"" + filecode + "\" not found." );
            return false;
        }
        out = file->get_out();
    }
    if( out == nullptr ) {
        out = m_out;
    }

    SValue value = expr( false );
    *out << value.as_string() << term;

    if( value.type() == SValue::Type::Error ) {
        m_ts.skip_to( SToken::Type::Semicolon );
    }
    if( m_ts.current().type() != SToken::Type::Semicolon ) {
        error( "';' expected." );
        return false;
    }
    return true;
}

bool Script::do_function()
{
    string code = get_name_or_primary( true );
    if( code.empty() ) {
        error( "Function name missing." );
        return false;
    }
    const char* builtin[] = { "if", "read" };
    for( auto bi : builtin ) {
        if( code.compare( bi ) == 0 ) {
            error( "Can not redefine built-in function \"@" + string( bi ) + "\"." );
            return false;
        }
    }
    if( m_db->get_function( code ) != NULL ) {
        error( "function \"" + code + "\" already exists." );
        return false;
    }
    SToken token = m_ts.current();
    StdStrVec args;
    SValueVec defs;
    if( token.type() == SToken::Type::Lbracket ) {
        token = m_ts.next();
        for( ;;) {
            if( token.type() == SToken::Type::Rbracket ) {
                break;
            }
            string str = m_ts.current().get_str();
            if( token.type() != SToken::Type::Name || str.empty() ) {
                error( "Argument name expected." );
                return false;
            }
            args.push_back( str );
            token = m_ts.next();
            SValue value;
            if( token.type() == SToken::Type::Equal ) {
                value = expr( true );
            }
            defs.push_back( value );
            token = m_ts.current();
            if( token.type() == SToken::Type::Comma ) {
                token = m_ts.next();
            }
        }
        token = m_ts.next();
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
    Function* fun = m_db->create_function( code );
    if( fun == nullptr ) {
        return false;
    }
    fun->set_args( args );
    fun->set_defaults( defs );
    fun->set_line( line );
    fun->set_script( script );
    return true;
}

bool glich::Script::do_command()
{
    string code = get_name_or_primary( true );
    if( code.empty() ) {
        error( "Command name missing." );
        return false;
    }
    if( m_db->get_command( code ) != NULL ) {
        error( "command \"" + code + "\" already exists." );
        return false;
    }
    SToken token = m_ts.current();
    StdStrVec args;
    SValueVec defs;
    if( token.type() == SToken::Type::Lbracket ) {
        token = m_ts.next();
        for( ;;) {
            if( token.type() == SToken::Type::Rbracket ) {
                break;
            }
            string str = m_ts.current().get_str();
            if( token.type() != SToken::Type::Name || str.empty() ) {
                error( "Argument name expected." );
                return false;
            }
            args.push_back( str );
            token = m_ts.next();
            SValue value;
            if( token.type() == SToken::Type::Equal ) {
                value = expr( true );
            }
            defs.push_back( value );
            token = m_ts.current();
            if( token.type() == SToken::Type::Comma ) {
                token = m_ts.next();
            }
        }
        token = m_ts.next();
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
    Command* com = m_db->create_command( code );
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
    if( m_ts.current().type() != SToken::Type::Semicolon ) {
        error( "';' expected." );
        return false;
    }
    return true;
}

bool Script::do_file()
{
    string code = get_name_or_primary( true );
    if( code.empty() ) {
        error( "File code missing." );
        return false;
    }
    string name = expr( false ).as_string();
    if( name.empty() ) {
        error( "Filename missing." );
        return false;
    }
    File::FileType type = File::FT_null;
    if( m_ts.current().type() != SToken::Type::Semicolon ) {
        string type = get_name_or_primary( true );
        if( type.empty() ) {
            error( "';' or switch expected." );
            return false;
        }
        if( type == "write" ) {
            type = File::FT_write;
        }
        else if( type == "append" ) {
            type = File::FT_append;
        }
    }
    if( m_ts.current().type() != SToken::Type::Semicolon ) {
        error( "';' expected." );
        return false;
    }
    File* file = m_db->create_file( code );
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

SValue Script::expr( bool get )
{
    SValue left = compare( get );
    for( ;;) {
        SToken token = m_ts.current();
        switch( token.type() )
        {
        case SToken::Type::Or:
            left.logical_or( compare( true ) );
            break;
        case SToken::Type::And:
            left.logical_and( compare( true ) );
            break;
        default:
            return left;
        }
    }
}

SValue Script::compare( bool get )
{
    SValue left = combine( get );
    for( ;;) {
        SToken token = m_ts.current();
        switch( token.type() )
        {
        case SToken::Type::Equal:
            left.equal( combine( true ) );
            break;
        case SToken::Type::NotEqual:
            left.equal( combine( true ) );
            left.logical_not();
            break;
        case SToken::Type::GtThan:
            left.greater_than( combine( true ) );
            break;
        case SToken::Type::GtThanEq:
            left.less_than( combine( true ) );
            left.logical_not();
            break;
        case SToken::Type::LessThan:
            left.less_than( combine( true ) );
            break;
        case SToken::Type::LessThanEq:
            left.greater_than( combine( true ) );
            left.logical_not();
            break;
        default:
            return left;
        }
    }
}

SValue Script::combine( bool get )
{
    SValue left = range( get );

    for( ;;) {
        SToken token = m_ts.current();
        switch( token.type() )
        {
        case SToken::Type::UNION:
            left.rlist_union( range( true ) );
            break;
        case SToken::Type::INTERSECTION:
            left.intersection( range( true ) );
            break;
        case SToken::Type::REL_COMPLEMENT:
            left.rel_complement( range( true ) );
            break;
        case SToken::Type::SYM_DIFFERENCE:
            left.sym_difference( range( true ) );
            break;
        default:
            return left;
        }
    }
}

SValue Script::range( bool get )
{
    SValue left = sum( get );

    for( ;;) {
        SToken token = m_ts.current();
        switch( token.type() )
        {
        case SToken::Type::DotDot:
            left.range_op( sum( true ) );
            break;
        default:
            return left;
        }
    }
}

SValue Script::sum( bool get )
{
    SValue left = term( get );
    for( ;;) {
        SToken token = m_ts.current();
        switch( token.type() )
        {
        case SToken::Type::Plus:
            left.plus( term( true ) );
            break;
        case SToken::Type::Minus:
            left.minus( term( true ) );
            break;
        default:
            return left;
        }
    }
}

SValue Script::term( bool get )
{
    SValue left = primary( get );

    for( ;;) {
        SToken token = m_ts.current();
        switch( token.type() )
        {
        case SToken::Type::Star:
            left.multiply( primary( true ) );
            break;
        case SToken::Type::Divide:
            left.divide( primary( true ) );
            break;
        case SToken::Type::Mod:
            left.modulus( primary( true ) );
            break;
        default:
            return left;
        }
    }
}

SValue Script::primary( bool get )
{
    SValue value;
    SToken token = get ? m_ts.next() : m_ts.current();
    switch( token.type() )
    {
    case SToken::Type::Number:
    {
        Num num = token.get_number();
        assert( num >= 0 ); // Literals are always positive.
        value.set_number( num );
    }
    m_ts.next();
    break;
    case SToken::Type::Field:
    {
        Field fld = token.get_field();
        assert( fld >= 0 ); // Literals are always positive.
        value.set_field( fld );
    }
    m_ts.next();
    break;
    case SToken::Type::String:
        value.set_str( token.get_str() );
        m_ts.next();
        break;
    case SToken::Type::Name:
        value = get_value_var( token.get_str() );
        m_ts.next();
        break;
    case SToken::Type::Lbracket:
        value = expr( true );
        if( m_ts.current().type() != SToken::Type::Rbracket ) {
            error( "')' expected." );
            break;
        }
        m_ts.next();
        break;
    case SToken::Type::Error:
        value = error_cast();
        break;
    case SToken::Type::At:
        value = function_call();
        break;
    case SToken::Type::Minus:
        value = primary( true );
        value.negate();
        break;
    case SToken::Type::Plus:
        value = primary( true );
        break;
    case SToken::Type::Not:
        value = primary( true );
        value.logical_not();
        break;
    default:
        value.set_error( "Primary value expected." );
    }
    return value;
}

std::string Script::get_name_or_primary( bool get )
{
    string str;
    SToken token = get ? m_ts.next() : m_ts.current();
    if( token.type() == SToken::Type::Name ) {
        str = token.get_str();
        m_ts.next();
    }
    else {
        SValue value = primary( false );
        if( value.type() == SValue::Type::String || value.type() == SValue::Type::Number ) {
            str = value.as_string();
        }
    }
    return str;
}

SValue Script::error_cast()
{
    SValue value = primary( true );
    bool ok = false;
    string mess = value.get_str( ok );
    if( !ok ) {
        mess = "Unable to read error message.";
    }
    value.set_error( mess );
    return value;
}

SValueVec Script::get_args( SValue& value )
{
    SToken token = m_ts.next();
    SValueVec args;
    if( token.type() == SToken::Type::Lbracket ) {
        for( ;; ) {
            SValue arg = expr( true );
            args.push_back( arg );
            token = m_ts.current();
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
        token = m_ts.next();
    }
    return args;
}

SValue Script::function_call()
{
    SValue value;
    SToken token = m_ts.next();
    if( token.type() != SToken::Type::Name ) {
        value.set_error( "Function name expected." );
        return value;
    }
    string name = token.get_str();
    if( name == "if" ) {
        return at_if();
    }
    else if( name == "read" ) {
        return at_read();
    }

    SValueVec args = get_args( value );
    if( value.is_error() ) {
        return value;
    }
    Function* fun = m_db->get_function( name );
    if( fun == nullptr ) {
        value.set_error( "Function " + name + " not found." );
        return value;
    }

    STokenStream prev_ts = m_ts;
    m_ts.set_line( fun->get_line() );
    std::istringstream iss( fun->get_script() );
    m_ts.reset_in( &iss );
    m_db->push_store();

    store()->set( "result", SValue() );
    for( size_t i = 0; i < fun->get_arg_size(); i++ ) {
        if( i < args.size() ) {
            store()->set( fun->get_arg_name( i ), args[i] );
        }
        else {
            store()->set( fun->get_arg_name( i ), fun->get_default_value( i ) );
        }
    }

    m_ts.next();
    while( statement() ) {
        if( m_ts.next().type() == SToken::Type::End ) {
            break;
        }
    }

    store()->get( &value, "result" );
    m_db->pop_store();
    m_ts = prev_ts;

    return value;
}

SValue glich::Script::command_call()
{
    SValue value;
    SToken token = m_ts.next();
    if( token.type() != SToken::Type::Name ) {
        value.set_error( "Command name expected." );
        return value;
    }
    string name = token.get_str();

    SValueVec args = get_args( value );
    if( value.is_error() ) {
        return value;
    }
    Command* com = m_db->get_command( name );
    if( com == nullptr ) {
        value.set_error( "Command " + name + " not found." );
        return value;
    }

    STokenStream prev_ts = m_ts;
    m_ts.set_line( com->get_line() );
    std::istringstream iss( com->get_script() );
    m_ts.reset_in( &iss );
    m_db->push_store();

    for( size_t i = 0; i < com->get_arg_size(); i++ ) {
        if( i < args.size() ) {
            store()->set( com->get_arg_name( i ), args[i] );
        }
        else {
            store()->set( com->get_arg_name( i ), com->get_default_value( i ) );
        }
    }

    m_ts.next();
    while( statement() ) {
        if( m_ts.next().type() == SToken::Type::End ) {
            break;
        }
    }

    m_db->pop_store();
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
    SValueVec args = get_args( value );
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
    SValueVec args = get_args( value );
    if( value.is_error() ) {
        return value;
    }
    string prompt;
    if( args.size() > 0 && args[0].type() == SValue::Type::String ) {
        prompt = args[0].get_str();
    }
    return m_db->read_input( prompt );
}

SValue Script::get_value_var( const std::string& name )
{
    if( name == "true" ) {
        return SValue( true );
    }
    if( name == "false" ) {
        return SValue( false );
    }
    if( name == "null" ) {
        return SValue();
    }
    SValue value;
    if( store()->get( &value, name ) ) {
        return value;
    }
    value.set_error( "Variable \"" + name + "\" not found." );
    return value;
}

// End of src/glc/glcScript.cpp