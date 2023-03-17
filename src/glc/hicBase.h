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

    class FormatText;

    typedef std::map<int,XRefVec> XRefSet;
    typedef std::map<std::string,XRefSet> XRefMap;

    enum {
        YMD_year, YMD_month, YMD_day
    };
    enum OptFieldID {
        OFID_NULL,
        OFID_wday,      // 7 Day week Mon=1 (1 to 7)
        OFID_wsday,     // 7 Day week Sun=1 (1 to 7)
        OFID_jwn,       // Julian Week Number, week count from jdn 0.
        OFID_dayinyear, // Day in year (1 to about 366, dep. on scheme)
        OFID_unshift,   // Value before being shifted, (year or day)
        OFID_repeated,  // Where dates can occasionally be repeated ie Easter. (0 or 1).
        //
        OFID_MAX
    };

    class Base
    {
    public:
        Base();
        Base( const std::string& data );
        virtual ~Base();

        // Set initial data from string.
        virtual void set_data( const std::string& data );

        // Return true if in a usable state.
        virtual bool is_ok() const { return true; }
        // Return the maximum number of Fields required by the Record.
        virtual size_t record_size() const = 0;
        // Return the number of extended (read-only) Fields available.
        virtual size_t extended_size() const { return record_size() + m_opt_fields.size(); }

        // Returns the index to the named Record field, or -1 if not found.
        virtual int get_fieldname_index( const std::string& fieldname ) const;
        virtual std::string get_fieldname( size_t index ) const;
        virtual OptFieldID get_opt_field_id( const std::string& fieldname ) const;
        virtual std::string get_opt_fieldname( OptFieldID field_id ) const;

        // Converts the Field's into a jdn and returns it.
        virtual Field get_jdn( const Field* fields ) const = 0;

        // Get an optional field value.
        virtual Field get_opt_field( const Field* fields, Field jdn, OptFieldID id ) const;
        virtual Field get_opt_field( const Field* fields, Field jdn, int index ) const;

        virtual bool set_fields_as_begin_first( Field* fields, const Field* mask ) const = 0;
        virtual bool set_fields_as_next_first( Field* fields, const Field* mask ) const = 0;
        virtual bool set_fields_as_begin_last( Field* fields, const Field* mask ) const = 0;
        virtual bool set_fields_as_next_last( Field* fields, const Field* mask ) const = 0;

        Field get_field( const Field* fields, Field jdn, size_t index ) const;
        Field get_field_first( const Field* fields, Field jdn, size_t index ) const;
        Field get_field_last( const Field* fields, Field jdn, size_t index ) const;
        virtual Field get_rec_field_first( const Field* fields, size_t index ) const;
        virtual Field get_rec_field_last( const Field* fields, size_t index ) const;
        virtual Field get_opt_field_first( const Field* fields, Field jdn, OptFieldID id ) const;
        virtual Field get_opt_field_last( const Field* fields, Field jdn, OptFieldID id ) const;

        // Converts the given jdn into the Records' Fields.
        virtual void set_fields( Field* fields, Field jdn ) const = 0;

        OptFieldID opt_index_to_id( size_t index ) const { return m_opt_fields[index-record_size()]; }
        int opt_id_to_index( OptFieldID id ) const;

    protected:
        virtual int get_std_fieldname_index( const std::string& fieldname ) const { return get_ymd_fieldname_index( fieldname ); }
        virtual std::string get_std_fieldname( size_t index ) const { return get_ymd_fieldname( index ); }

        int get_ymd_fieldname_index( const std::string& fieldname ) const;
        std::string get_ymd_fieldname( size_t index ) const;

        size_t opt_fields_size() const { return m_opt_fields.size(); }

    private:
        static const char* s_ymd_fieldnames[];
        static size_t s_sizeof_ymd_fieldnames;

        std::vector<OptFieldID> m_opt_fields;
        LocaleData m_locale;
    };

}

#endif // GLC_HICBASE_H_GUARD
