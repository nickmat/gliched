/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/hicJulian.h
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Julian (j) calendar header.
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

#ifndef GLC_HICJULIAN_H_GUARD
#define GLC_HICJULIAN_H_GUARD

#include "hicBase.h"

namespace glich {

    Field julian_to_jdn( Field year, Field month, Field day );
    void julian_from_jdn( Field* year, Field* month, Field* day, Field jdn );
    Field julian_easter( Field year );

    class Julian : public Base
    {
    public:
        Julian() {}
        Julian( const std::string& data ) : Base( data ) {}

        size_t record_size() const override { return 3; }

        OptFieldID get_opt_field_id( const std::string& fieldname ) const override;
        std::string get_opt_fieldname( OptFieldID field_id ) const override;

        Field get_jdn( const Field* fields ) const override;

        Field get_opt_field( const Field* fields, Field jdn, OptFieldID id ) const override;

        bool set_fields_as_begin_first( Field* fields, const Field* mask ) const override;
        bool set_fields_as_next_first( Field* fields, const Field* mask ) const override;
        bool set_fields_as_begin_last( Field* fields, const Field* mask ) const override;
        bool set_fields_as_next_last( Field* fields, const Field* mask ) const override;

        void set_fields( Field* fields, Field jdn ) const override;

        Field get_rec_field_last( const Field* fields, size_t index ) const override;

        // Virtual members shared with Gregorian class.
        virtual Field jdn( Field year, Field month, Field day ) const;
        virtual Field easter( Field year ) const;

    protected:
        virtual bool is_leap_year( Field year ) const;
        Field last_day_in_month( Field year, Field month ) const;

    };

}

#endif // GLC_HICJULIAN_H_GUARD