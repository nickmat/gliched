/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/hicBase.h
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Hics Base class, used as base for all calendar calulations.
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

#ifndef GLC_HICBASE_H_GUARD
#define GLC_HICBASE_H_GUARD

#include <glc/hicDefs.h>
#include "hicHelper.h"

namespace glich {


    class Base
    {
    public:
        Base( const std::string& data );
        virtual ~Base();

        // Set initial data from string.
        virtual void set_data( const std::string& data );
        virtual bool set_epoch( Field epoch ) { return false; }

        bool attach_grammar( Grammar* gmr );
        // Return true if in a usable state.
        virtual bool is_ok() const { return true; }

        // Return the number of Required Fields.
        virtual size_t required_size() const = 0;
        // Return the number of of all Fields including optional and calculated.
        size_t record_size() const { return m_fieldnames.size(); }
        Grammar* get_grammar() const { return m_grammar; }
        // Get list of fieldnames in default order.
        StdStrVec get_fieldnames() const { return m_fieldnames; }
        std::string get_fieldname( size_t index ) const { return m_fieldnames[index]; }
        int get_fieldname_index( const std::string& fieldname ) const;
        int get_alias_fieldname_index( const std::string& alias ) const;

        std::string get_calc_input() const { return m_calculate_input; }
        std::string get_calc_output() const { return m_calculate_output; }
        bool has_calc_input() const { return !m_calculate_input.empty(); }
        bool has_calc_output() const { return !m_calculate_output.empty(); }
        Format* get_format( const std::string& fcode ) const;
        std::string get_input_fcode() const;
        std::string get_output_fcode() const;

        // Converts the Field's into a jdn and returns it.
        virtual Field get_jdn( const FieldVec& fields ) const = 0;

        virtual Field get_beg_field_value( const FieldVec& fields, size_t index ) const;
        virtual Field get_end_field_value( const FieldVec& fields, size_t index ) const = 0;

        // Complete any redundant fields.
        virtual void update_input( FieldVec& fields ) const {}
        // Complete any redundant fields.
        virtual void update_output( FieldVec& fields ) const {}
        // Converts the given jdn into the Records' Fields.
        virtual FieldVec get_fields( Field jdn ) const = 0;

    protected:
        StdStrVec m_fieldnames; // This is both required, calculated and optional.
        LocaleData m_locale;
        Grammar* m_grammar;

        std::string m_calculate_input;
        std::string m_calculate_output;
    };

}

#endif // GLC_HICBASE_H_GUARD
