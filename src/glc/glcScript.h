/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/glcScript.h
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

#ifndef SRC_GLC_GLCSCRIPT_H_GUARD
#define SRC_GLC_GLCSCRIPT_H_GUARD

#include <glc/glc.h>
#include "glcTokenStream.h"
#include "glcScriptStore.h"

namespace glich {

    class Script {
    public:
        Script( Glich* db, std::istream& in, std::ostream& out );

        bool run();

        static STokenStream* get_current_ts() { return s_current_ts; }

    private:
        bool error( const std::string& mess ) { return m_ts.error( mess ); }
        ScriptStore* store() const;
        bool statement();

        bool do_mark();
        bool do_clear();
        bool do_if();
        bool do_do();
        bool do_set();
        bool do_let();
        bool do_assign( const std::string& name );
        bool do_write( const std::string& term = "" );
        bool do_writeln() { return do_write( "\n" ); }
        bool do_function();
        bool do_command();
        bool do_call();
        bool do_file();

        SValue expr( bool get );
        SValue compare( bool get );
        SValue combine( bool get );
        SValue range( bool get );
        SValue sum( bool get );
        SValue term( bool get );
        SValue primary( bool get );
        std::string get_name_or_primary( bool get );

        SValue error_cast();
        SValueVec get_args( SValue& value );
        SValue function_call();
        SValue command_call();
        SValue at_if();
        SValue at_read();

        SValue get_value_var( const std::string& name );

        static STokenStream* s_current_ts;

        Glich* m_glc;
        STokenStream m_ts;
        std::ostream* m_out;
        std::ostream* m_err;
    };

} // namespace HistoryGen

#endif // SRC_GLC_GLCSCRIPT_H_GUARD
