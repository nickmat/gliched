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
        Record( const Base* base );
        Record( const Base* base, Field jdn );
        Record( const Base* base, const Field* fields, size_t size );
        Record( const Base* base, const std::string& str, const std::string& fcode, Boundary rb );
        Record( const Record& rec );

        void clear_fields();

        void set_jdn( Field jdn );
        void set_fields( const Field* fields, size_t size );
        void set_str( const std::string& str, const std::string& fcode, Boundary rb );

        bool set_fields_as_next_first( const Field* mask );
        bool set_fields_as_next_last( const Field* mask );

        Field get_jdn() const;
        Field jdn() const { return m_jdn; }
        FieldVec get_fieldvec() const { return m_f; };

    private:
        const Base* m_base;
        FieldVec    m_f;
        Field       m_jdn;
    };

}

#endif // SRC_GLC_HICRECORD_H_GUARD
