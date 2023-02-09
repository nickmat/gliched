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
#include "glcScript.h"

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
    m_marks.push_back( new Mark( "" ) );
    if( !m_inout ) {
        m_inout = new InOut;
    }
}

string Glich::run_script( const string& script )
{
    std::istringstream iss( script );
    std::ostringstream oss;
    Script scr( this, iss, oss );
    scr.run();
    return oss.str();
}

Function* Glich::create_function( const std::string& code )
{
    Function* fun = new Function( code );
    assert( m_marks.size() > 0 );
    m_marks[m_marks.size() - 1]->add_function( fun );
    m_functions[code] = fun;
    return fun;
}

Function* Glich::get_function( const std::string& code ) const
{
    if( m_functions.count( code ) > 0 ) {
        return m_functions.find( code )->second;
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

void Glich::add_or_replace_mark( const std::string& name )
{
    clear_mark( name );
    Mark* mark = new Mark( name );
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

// End of src/hg/historygen.cpp
