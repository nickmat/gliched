/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/hicGrammar.h
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Grammar class to control date formatting.
 * Author:      Nick Matthews
 * Website:     https://github.com/nickmat/glich
 * Created:     24th March 2023
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

#ifndef GLC_HICGRAMMAR_H_GUARD
#define GLC_HICGRAMMAR_H_GUARD

#include "hicHelper.h"
#include <glc/hicDefs.h>

namespace glich {

    class Glich;
    class Base;
    class Grammar;
    class FormatText;
    class FormatUnit;

    class Grammar
    {
    public:
        Grammar( const std::string& code, Glich* glc );
        ~Grammar();

        bool is_ok() const { return m_ok; }
        bool constuct( const Base* base );

        void set_base_fieldnames( StdStrVec fieldnames ) { m_base_fieldnames = fieldnames; }
        FormatText* create_format_text( const std::string& code );
        FormatUnit* create_format_unit( const std::string& code );
        bool add_format( Format* fmt );

        std::string get_code() const { return m_code; }
        std::string resolve_field_alias( const std::string& alias );
        std::string resolve_unit_alias( const std::string& alias );
        Format* get_format( const std::string& code ) const;

        StdStrVec get_base_fieldnames() const { return m_base_fieldnames; }
        Glich& get_glich() const { return *m_glc; }
        void remove_format( const std::string& fcode ) { m_formats.erase( fcode ); }

        static Grammar* create_default_grammar( const Base* base, Glich* glc );

    private:
        Glich*      m_glc;
        std::string m_code;
        bool        m_ok;
        Grammar*    m_inherit;
        FormatMap   m_formats;
        StdStrVec   m_base_fieldnames;
        StdStrMap   m_field_alias;
        StdStrMap   m_unit_alias;
    };

}

#endif // GLC_HICGRAMMAR_H_GUARD
