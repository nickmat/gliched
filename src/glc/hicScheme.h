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
#include "glcObject.h"

namespace glich {

    class Calendars;
    class Grammar;
    class Base;

    class Scheme : public Object
    {
    public:
        enum class BaseName { null, jdn, julian, gregorian };

        Scheme( const std::string& code, const Base& base );
        virtual ~Scheme();

        void set_output_format( const std::string& fcode ) { m_output_fcode = fcode; }
        void set_input_format( const std::string& fcode ) { m_input_fcode = fcode; }

        SValue complete_object( Field jdn ) const;
        SValue complete_object( const std::string& input, const std::string& fcode ) const;

        std::string get_name() const { return m_name; }
        Scheme_style get_style() const { return m_style; }
        const Base& get_base() const { return m_base; }
        Format* get_output_format( const std::string& fcode ) const;
        Format* get_input_format( const std::string& fcode ) const;

        FieldVec get_object_fields( const SValueVec& values ) const;

        void set_name( const std::string& name ) { m_name = name; }
        void set_style( Scheme_style style ) { m_style = style; }

        std::string jdn_to_str( Field jdn, const std::string& fcode ) const;
        std::string range_to_str( Range rng, const std::string& fcode ) const;
        std::string rlist_to_str( RList rlist, const std::string& fcode ) const;
        RList str_to_rlist( const std::string& str, const std::string& fcode ) const;

        std::string object_to_str( const SValue& ovalue, const std::string& fcode ) const;

        static Base* create_base( BaseName bs, const std::string& data );

    private:
        std::string  m_name;
        Scheme_style  m_style;
        const Base&  m_base;
        std::string  m_input_fcode;
        std::string  m_output_fcode;
    };

}

#endif // SRC_GLC_HICSCHEME_H_GUARD
