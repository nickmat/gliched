/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/hicRecord.h
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Header for Record class to hold date values.
 * Author:      Nick Matthews
 * Website:     http://historycal.org
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

#ifndef SRC_GLC_HICRECORD_H_GUARD
#define SRC_GLC_HICRECORD_H_GUARD

#include <glc/glcDefs.h>
#include <glc/hicDefs.h>
#include "hicHelper.h"


namespace glich {

    class Base;

    class Record
    {
    public:
        Record( const Base& base );
        Record( const Base& base, Field jdn );

        void set_jdn( Field jdn );
        Field calc_jdn();

        Field complete_fields_as_beg();
        Field complete_fields_as_end();

        void set_field( Field value, size_t index );

        void clear_fields() { std::fill( m_f.begin(), m_f.end(), f_invalid ); }

        const Base& get_base() const { return m_base; }
        const FieldVec& get_field_vec() const { return m_f; }
        Field get_field( size_t index ) const { return m_f[index]; }
        Field get_jdn() const { return m_jdn; }

    private:
        const Base& m_base;
        FieldVec    m_f;
        Field       m_jdn;
    };
}

#endif // SRC_GLC_HICRECORD_H_GUARD
