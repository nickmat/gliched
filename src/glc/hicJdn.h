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
        Jdn( const std::string& data = std::string() ) : Base( data ) {
            m_fieldnames = { "day" };
        }

        size_t required_size() const override { return 1; }

        Field get_jdn( const FieldVec& fields ) const override { return fields[0]; }

        void set_fields( FieldVec& fields, Field jdn ) const override { fields[0] = jdn; }

    };

}

#endif // GLC_HICJDN_H_GUARD
