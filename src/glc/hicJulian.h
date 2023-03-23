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
        Julian( const std::string& data = std::string() ) : Base( data ) {
            m_fieldnames = { "year", "month", "day" };
        }
        ~Julian() {}

        size_t required_size() const override { return 3; }
    
        Field get_jdn( const FieldVec& fields ) const override;

        FieldVec get_fields( Field jdn ) const override;

    };

}

#endif // GLC_HICJULIAN_H_GUARD