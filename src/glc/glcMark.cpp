/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/glcMark.cpp
 * Project:     Glich: Extendable Script Language.
 * Purpose:     ScriptMark class used by the scripts mark statement.
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

#include "glcMark.h"

#include "glcFile.h"
#include "glcFunction.h"
#include "glcObject.h"
#include "hicFormat.h"
#include "hicGrammar.h"
#include "hicLexicon.h"
#include "hicScheme.h"

using namespace glich;
using std::string;
using std::vector;


Mark::Mark( const string& name, Mark* prev )
    : m_name( name ), m_context( Context::glich ), m_ischeme( nullptr ), m_oscheme( nullptr )
{
    if( prev != nullptr ) {
        m_context = prev->get_context();
        m_ischeme = prev->get_ischeme();
        m_oscheme = prev->get_oscheme();
    }
}

Mark::~Mark()
{
}

string Mark::remove_next_function()
{
    string code;
    if( !m_functions.empty() ) {
        Function* func = m_functions[m_functions.size() - 1];
        code = func->get_code();
        delete func;
        m_functions.pop_back();
    }
    return code;
}

std::string Mark::remove_next_command()
{
    string code;
    if( !m_commands.empty() ) {
        Command* com = m_commands[m_commands.size() - 1];
        code = com->get_code();
        delete com;
        m_commands.pop_back();
    }
    return code;
}

std::string Mark::remove_next_object()
{
    string code;
    if( !m_objects.empty() ) {
        Object* obj = m_objects[m_objects.size() - 1];
        code = obj->get_code();
        delete obj;
        m_objects.pop_back();
    }
    return code;
}

string Mark::remove_next_file()
{
    string code;
    if( m_files.size() ) {
        File* file = m_files[m_files.size() - 1];
        code = file->get_code();
        delete file;
        m_files.pop_back();
    }
    return code;
}

std::string Mark::remove_next_lexicon()
{
    string code;
    if( m_lexicons.size() ) {
        Lexicon* lex = m_lexicons[m_lexicons.size() - 1];
        code = lex->get_code();
        delete lex;
        m_lexicons.pop_back();
    }
    return code;
}

std::string Mark::remove_next_grammar()
{
    string code;
    if( m_grammars.size() ) {
        Grammar* gmr = m_grammars[m_grammars.size() - 1];
        code = gmr->get_code();
        delete gmr;
        m_grammars.pop_back();
    }
    return code;
}

std::string Mark::remove_next_format()
{
    string code;
    if( m_formats.size() ) {
        Format* fmt = m_formats[m_formats.size() - 1];
        code = fmt->get_code();
        Grammar* gmr = fmt->get_owner();
        if( gmr ) {
            gmr->remove_format( code );
        }
        delete fmt;
        m_formats.pop_back();
    }
    return code;
}

// End of src/glc/glcMark.cpp
