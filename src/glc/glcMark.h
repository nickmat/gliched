/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/glcMark.h
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Mark class used by the scripts mark statement.
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

#ifndef SRC_GLC_GLCMARK_H_GUARD
#define SRC_GLC_GLCMARK_H_GUARD

#include "glc/glcDefs.h"

namespace glich {

    class Function;
    class Command;
    class Object;
    class File;
    class Lexicon;
    class Grammar;
    class Format;

    class Mark
    {
    public:
        Mark( const std::string& name, Mark* prev );
        ~Mark();

        std::string get_name() const { return m_name; }

        void add_function( Function* function ) { m_functions.push_back( function ); }
        void add_command( Command* command ) { m_commands.push_back( command ); }
        void add_object( Object* object ) { m_objects.push_back( object ); }
        void add_file( File* file ) { m_files.push_back( file ); }
        void add_lexicon( Lexicon* lex ) { m_lexicons.push_back( lex ); }
        void add_grammar( Grammar* gmr ) { m_grammars.push_back( gmr ); }
        void add_format( Format* fmt ) { m_formats.push_back( fmt ); }

        std::string remove_next_function();
        std::string remove_next_command();
        std::string remove_next_object();
        std::string remove_next_file();
        std::string remove_next_lexicon();
        std::string remove_next_grammar();
        std::string remove_next_format();

        void set_context( Context ct ) { m_context = ct; }

        Context get_context() const { return m_context; }

    private:
        std::string            m_name;
        std::vector<Function*> m_functions;
        std::vector<Command*>  m_commands;
        std::vector<Object*>   m_objects;
        std::vector<File*>     m_files;
        std::vector<Lexicon*>  m_lexicons;
        std::vector<Grammar*>  m_grammars;
        std::vector<Format*>   m_formats;
        Context m_context;
    };

}

#endif // SRC_GLC_GLCMARK_H_GUARD