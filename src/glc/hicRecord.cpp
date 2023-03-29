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
#include "hicFormat.h"

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

Record::Record( const Base& base, const string& str, const Format& fmt, Boundary rb )
    : m_base( base ), m_jdn( f_invalid ), m_f( base.record_size() )
{
    set_str( str, fmt, rb );
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

void Record::set_str( const string& str, const Format& fmt, Boundary rb )
{
    clear_fields();
    string in = full_trim( str );
    if( in == "past" ) {
        m_jdn = m_f[0] = f_minimum;
        return;
    }
    if( in == "future" ) {
        m_jdn = m_f[0] = f_maximum;
        return;
    }
#if 0
    if( in == "today" ) {
        set_jdn( Gregorian::today() );
        return;
    }
#endif

    if( fmt.set_input( *this, in, rb ) ) {
        m_jdn = get_jdn();
    }
    else {
        clear_fields();
    }
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

// Treat the record as a mask and generate rlist
// Mask is of the form "val, ..., ?, ..."
RList Record::get_rlist_from_mask() const
{
    RList rlist;
    if( m_f[0] == f_invalid ) {
        return rlist;
    }
    Record beg( *this ), end(*this);
    Range range;
    range.m_beg = beg.complete_fields_as_beg();
    range.m_end = end.complete_fields_as_end();
    rlist.push_back( range );
    return rlist;
}

// Sets range and returns true if the masks can create a valid range.
// Sets up range but returns false if the masks can create a valid range only
// by ignoring optional fields.
// Sets range to invalid and returns false if the masks cannot create a valid
// range.
bool Record::set_range_as_begin( Range& range ) const
{
    Record rec1( m_base );
    Record rec2( m_base );
    bool ret1 = rec1.complete_fields_as_beg();
    bool ret2 = rec2.complete_fields_as_end();
    if( !ret1 || !ret2 ) {
        range.m_beg = f_invalid;
        return false;
    }
    range.m_beg = rec1.get_jdn();
    range.m_end = rec2.get_jdn();
    if( range.m_beg == f_invalid || range.m_end == f_invalid ) {
        range.m_beg = f_invalid;
        return false;
    }
    // Adjust for extended fields restricting the range.
    ret1 = rec1.complete_fields_as_beg();
    ret2 = rec2.complete_fields_as_end();
    if( ret1 && ret2 ) {
        Range r;
        r.m_beg = rec1.get_jdn();
        r.m_end = rec2.get_jdn();
        if( r.m_beg != range.m_beg || r.m_end != range.m_end ) {
            if( r.m_beg > r.m_end ) {
                return false;
            }
            range = r;
            return true;
        }
    }
    return ret1 && ret2;
}

// Using the initial value of range, attempts to adjust the value of range
// to the next value.
// If it fails, it sets the range invalid and returns false.
// If it succeeds, it will attempt to correct for optional fields - if this
// fails, the uncorrected range is set and the the function returns false.
bool Record::set_range_as_next( Range& range ) const
{
    range.m_beg = f_invalid;
    return false;
}


// End of src/glc/hicRecord.cpp file
