/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/hicJdn.h
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Jdn (Julian Day Number) calendar header.
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

#ifndef GLC_HICJDN_H_GUARD
#define GLC_HICJDN_H_GUARD

#include "hicBase.h"

namespace glich {

    class Jdn : public Base
    {
    public:
        Jdn() {}
        Jdn( const std::string& data ) : Base( data ) {}

        size_t record_size() const override { return 1; }

        bool set_fields_as_begin_first( Field* fields, const Field* mask ) const override;
        bool set_fields_as_next_first( Field* fields, const Field* mask ) const override;
        bool set_fields_as_begin_last( Field* fields, const Field* mask ) const override;
        bool set_fields_as_next_last( Field* fields, const Field* mask ) const override;

        Field get_jdn( const Field* fields ) const override { return fields[0]; }

        void set_fields( Field* fields, Field jdn ) const override { fields[0] = jdn; }

    protected:
        int get_std_fieldname_index( const std::string& fieldname ) const override;
        std::string get_std_fieldname( size_t index ) const override;
    };

}

#endif // GLC_HICJDN_H_GUARD
