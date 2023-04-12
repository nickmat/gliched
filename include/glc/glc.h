/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        include/glc/glc.h
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Glich language class header
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

#ifndef INCLUDE_GLC_GLC_H_GUARD
#define INCLUDE_GLC_GLC_H_GUARD

#include "glcDefs.h"
#include "hicDefs.h"

namespace glich {

    class Command;
    using CommandMap = std::map<std::string, Command*>;
    class Object;
    using ObjectMap = std::map<std::string, Object*>;
    class File;
    using FileMap = std::map<std::string, File*>;
    class Mark;
    using MarkVec = std::vector<Mark*>;
    class ScriptStore;

    class InOut {
    public:
        InOut() {}
        virtual ~InOut() {}

        virtual std::string get_input( const std::string& prompt );
    };

    enum class InitLibrary { None, Hics };

    class Glich {
    public:
        Glich( InitLibrary lib = InitLibrary::None, InOut* inout = nullptr );
        ~Glich();

        void load_hics_library();

        std::string run_script( const std::string& cmnd );

        std::string get_init_error() const { return m_init_error; }

        bool add_function( Function* fun );
        Function* get_function( const std::string& code ) const;
        Command* create_command( const std::string& code );
        Command* get_command( const std::string& code ) const;
        Object* create_object( const std::string & code );
        bool add_object( Object* obj, const std::string& code );
        Object* get_object( const std::string& code ) const;
        File* create_file( const std::string& code );
        File* get_file( const std::string& code ) const;
        bool add_lexicon( Lexicon* lex, const std::string& code );
        Lexicon* get_lexicon( const std::string& code ) const;
        bool add_grammar( Grammar* gmr, const std::string& code );
        Grammar* get_grammar( const std::string& code ) const;
        bool add_format( Format* fmt, const std::string& code );

        void add_or_replace_mark( const std::string& name );
        bool clear_mark( const std::string& name );
        ScriptStore* get_store() const { return m_store; }
        void push_store();
        bool pop_store();
        std::string read_input( const std::string& prompt ) const;

        void set_context( Context ct );
        void set_ischeme( Scheme* sch );
        void set_oscheme( Scheme* sch );
        Context get_context() const;
        Scheme* get_ischeme() const;
        Scheme* get_oscheme() const;

    private:
        FunctionMap m_functions;
        CommandMap m_commands;
        ObjectMap m_objects;
        FileMap m_files;
        LexiconMap m_lexicons;
        GrammarMap m_grammars;
        MarkVec m_marks;
        ScriptStore* m_store;
        std::string m_init_error;
        InOut* m_inout;
    };

} // namespace glich

#endif // INCLUDE_GLC_GLC_H_GUARD
