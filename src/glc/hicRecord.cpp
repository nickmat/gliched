/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/hicRecord.cpp
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Source for Record class to hold date values.
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

#include "hicRecord.h"

#include "glcHelper.h"
#include "hicBase.h"

#include <algorithm>
#include <cassert>

using namespace glich;
using std::string;

Record::Record( const Base& base )
    : m_base( base ), m_jdn( f_invalid ), m_f( base.record_size(), f_invalid )
{
}

Record::Record( const Base& base, Field jdn )
    : m_base(base), m_jdn(jdn)
{
    if( jdn == f_minimum || jdn == f_maximum || jdn == f_invalid ) {
        m_f = FieldVec( base.record_size(), f_invalid );
        m_f[0] = jdn;
        return;
    }
    m_f = m_base.get_fields( jdn );
}

void Record::set_jdn( Field jdn )
{
    m_jdn = jdn;
    if( jdn == f_minimum || jdn == f_maximum || jdn == f_invalid ) {
        clear_fields();
        m_f[0] = jdn;
        return;
    }
    m_f = m_base.get_fields( jdn );
}

Field glich::Record::calc_jdn()
{
    if( m_f[0] == f_minimum || m_f[0] == f_maximum ) {
        m_jdn = m_f[0];
    }
    else {
        // Base should check or correct for valid content
        m_jdn = m_base.get_jdn( m_f );
    }
    return m_jdn;
}

// Complete all invalid required fields by setting them to the first valid value.
// If the first field is invalid, set it to past.
Field Record::complete_fields_as_beg()
{
    bool begining = true;
    for( size_t i = 0; i < m_base.required_size(); i++ ) {
        if( begining && m_f[i] != f_invalid ) {
            continue;
        }
        m_f[i] = m_base.get_beg_field_value( m_f, i );
        begining = false;
    }
    return calc_jdn();
}


// Complete all invalid required fields by setting them to the last valid value.
// If the first field is invalid, set it to future.
Field glich::Record::complete_fields_as_end()
{
    bool begining = true;
    for( size_t i = 0; i < m_base.required_size(); i++ ) {
        if( begining && m_f[i] != f_invalid ) {
            continue;
        }
        m_f[i] = m_base.get_end_field_value( m_f, i );
        begining = false;
    }
    return calc_jdn();
}

void Record::set_field( Field value, size_t index )
{
    assert( index < m_f.size() );
    if( index < m_f.size() ) {
        m_f[index] = value;
    }
}


// End of src/glc/hicRecord.cpp file
