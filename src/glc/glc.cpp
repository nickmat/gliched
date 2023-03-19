/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/glc.cpp
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Glich language class
 * Author:      Nick Matthews
 * Website:     https://github.com/nickmat/glich
 * Created:     8th February 2023
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

#include <glc/glc.h>

#include "glcFile.h"
#include "glcFunction.h"
#include "glcMark.h"
#include "glcObject.h"
#include "glcScript.h"
#include "glcValue.h"
#include "hicScheme.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace glich;
using std::string;
using std::vector;


string InOut::get_input( const string& prompt ) {
    if( !prompt.empty() ) {
        std::cout << prompt;
    }
    string result;
    std::getline( std::cin, result );
    return result;
}

Glich::Glich( InOut* inout ) : m_store( new ScriptStore ), m_inout( inout )
{
    m_marks.push_back( new Mark( "", nullptr ) );
    if( !m_inout ) {
        m_inout = new InOut;
    }
    STokenStream::init( this );
    SValue::init( this );
}

Glich::~Glich()
{
    for( size_t i = m_marks.size(); i > 0; --i ) {
        delete m_marks[i - 1];
    }
    while( pop_store() );
    delete m_store;
    delete m_inout;
}

string Glich::run_script( const string& script )
{
    std::istringstream iss( script );
    std::ostringstream oss;
    Script scr( this, iss, oss );
    scr.run();
    return oss.str();
}

bool Glich::add_function( Function* fun )
{
    assert( m_marks.size() > 0 );
    m_marks[m_marks.size() - 1]->add_function( fun );
    m_functions[fun->get_code()] = fun;
    return true;
}

Function* Glich::get_function( const std::string& code ) const
{
    if( m_functions.count( code ) > 0 ) {
        return m_functions.find( code )->second;
    }
    return nullptr;
}

Command* Glich::create_command( const std::string& code )
{
    Command* com = new Command( code );
    assert( m_marks.size() > 0 );
    m_marks[m_marks.size() - 1]->add_command( com );
    m_commands[code] = com;
    return com;
}

Command* Glich::get_command( const std::string& code ) const
{
    if( m_commands.count( code ) > 0 ) {
        return m_commands.find( code )->second;
    }
    return nullptr;
}

Object* glich::Glich::create_object( const std::string& code )
{
    Object* obj = new Object( code );
    assert( m_marks.size() > 0 );
    m_marks[m_marks.size() - 1]->add_object( obj );
    m_objects[code] = obj;
    return obj;
}

Object* Glich::get_object( const std::string& code ) const
{
    if( m_objects.count( code ) > 0 ) {
        return m_objects.find( code )->second;
    }
    return nullptr;
}

File* Glich::create_file( const std::string& code )
{
    File* file = new File( code );
    assert( m_marks.size() > 0 );
    m_marks[m_marks.size() - 1]->add_file( file );
    m_files[code] = file;
    return file;
}

File* Glich::get_file( const std::string& code ) const
{
    if( m_files.count( code ) > 0 ) {
        return m_files.find( code )->second;
    }
    return nullptr;
}

bool Glich::add_scheme( Scheme* sch, std::string& code )
{
    // Only add initialised schemes and that are not already there.
    if( sch == nullptr || sch->is_ok() == false || m_schemes.count( code ) ) {
        delete sch;
        return false;
    }
    assert( m_marks.size() > 0 );
    m_marks[m_marks.size() - 1]->add_scheme( sch );
    m_schemes[code] = sch;
    return true;
}

Scheme* Glich::get_scheme( const std::string& code )
{
    if( m_schemes.count( code ) > 0 ) {
        return m_schemes.find( code )->second;
    }
    return nullptr;
}

bool Glich::add_lexicon( Lexicon* lex, std::string& code )
{
    // Only add lexicons and that are not already there.
    if( lex == nullptr || m_lexicons.count( code ) ) {
        delete lex;
        return false;
    }
    assert( m_marks.size() > 0 );
    m_marks[m_marks.size() - 1]->add_lexicon( lex );
    m_lexicons[code] = lex;
    return true;
}

Lexicon* Glich::get_lexicon( const std::string& code )
{
    if( m_lexicons.count( code ) > 0 ) {
        return m_lexicons.find( code )->second;
    }
    return nullptr;
}

void Glich::add_or_replace_mark( const std::string& name )
{
    clear_mark( name );
    Mark* prev = nullptr;
    int i = int( m_marks.size() ) - 1;
    if( i >= 0 ) {
        prev = m_marks[i];
    }
    Mark* mark = new Mark( name, prev );
    m_marks.push_back( mark );
}

bool Glich::clear_mark( const std::string& name )
{
    assert( !m_marks.empty() );
    assert( !name.empty() );
    size_t end, pos;
    end = pos = m_marks.size() - 1;
    while( pos != 0 && name != m_marks[pos]->get_name() ) {
        --pos;
    }
    if( pos == 0 ) {
        return false; // Can't find mark name.
    }
    for( size_t i = end; i >= pos; --i ) {
        string code;
        for( ;;) {
            code = m_marks[i]->remove_next_function();
            if( code.empty() ) {
                break;
            }
            m_functions.erase( code );
        }
        for( ;;) {
            code = m_marks[i]->remove_next_command();
            if( code.empty() ) {
                break;
            }
            m_commands.erase( code );
        }
        for( ;;) {
            code = m_marks[i]->remove_next_object();
            if( code.empty() ) {
                break;
            }
            m_objects.erase( code );
        }
        for( ;;) {
            code = m_marks[i]->remove_next_file();
            if( code.empty() ) {
                break;
            }
            m_files.erase( code );
        }
        delete m_marks[i];
        m_marks.pop_back();
    }
    return true;
}

void Glich::push_store()
{
    m_store = new ScriptStore( m_store );
}

bool Glich::pop_store()
{
    ScriptStore* store = m_store->get_prev();
    if( store ) {
        delete m_store;
        m_store = store;
        return true;
    }
    return false;
}

std::string Glich::read_input( const std::string& prompt ) const
{
    return m_inout->get_input( prompt );
}

void Glich::set_context( Context ct )
{
    int i = int( m_marks.size() ) - 1;
    if( i >= 0 ) {
        m_marks[i]->set_context( ct );
    }
}

Context Glich::get_context() const
{
    int i = int( m_marks.size() ) - 1;
    if( i >= 0 ) {
       return m_marks[i]->get_context();
    }
    return Context::glich;
}

// End of src/hg/historygen.cpp
