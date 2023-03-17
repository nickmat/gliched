/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/hicScheme.h
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Scheme class implimentation.
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

#ifndef SRC_GLC_HICSCHEME_H_GUARD
#define SRC_GLC_HICSCHEME_H_GUARD

#include <glc/glc.h>
#include <glc/hicDefs.h>

namespace glich {

    class Calendars;
    class Grammar;
    class Base;

    class Scheme
    {
    public:
        enum class BaseName { null, jdn };
        Scheme( const std::string& name, const Base* base );
        ~Scheme();

        bool is_ok() const;
        std::string get_code() const { return m_code; }
        std::string get_name() const { return m_name; }
        Scheme_style get_style() const { return m_style; }

//        std::string get_pref_input_format() const;
//        void get_info( Scheme_info* info ) const; 
//        std::string get_input_fcode() const;
//        std::string get_output_fcode() const;
        const Base* get_base() const { return m_base; }
//        Format* get_output_format( const std::string& fcode ) const;

        void set_code( const std::string& code ) { m_code = code; }
        void set_style( Scheme_style style ) { m_style = style; }
        void set_input_format( const std::string& code ) { m_input_fcode = code; }
        void set_output_format( const std::string& code ) { m_output_fcode = code; }

        Field fieldvec_to_jdn( const FieldVec& fieldv );
        FieldVec jdn_to_fieldvec( Field jdn );
        RList fieldvec_to_rlist( const FieldVec& fieldv );
        Field jdn_fieldname_to_field( Field jdn, const std::string& fieldname ) const;

        Field str_to_jdn( const std::string& str, const std::string& fmt );
//        std::string jdn_to_str( Field jdn, const std::string& fcode );
//        std::string range_to_str( Range range, const std::string& fcode );
//        std::string rlist_to_str( const RList& ranges, const std::string& fcode );

//        Field add_to_jdn( Field jdn, Field value, Unit unit, Norm norm );
//        bool str_to_rel_info( const std::string& str, Rel_info* info ) const;
//        RList rel_rlist( const RList& ranges, Rel_info* info );

        static Base* create_base( BaseName bs, const std::string& data );

    private:
        Unit str_to_unit( const std::string& str ) const;

        std::string   m_name;
        std::string   m_code;
        Scheme_style  m_style;
        const Base*   m_base;
        std::string   m_input_fcode;
        std::string   m_output_fcode;
    };

}


#endif // SRC_GLC_HICSCHEME_H_GUARD