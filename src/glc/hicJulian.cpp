/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/hicJulian.cpp
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Julian (j) calendar source.
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

#include "hicJulian.h"

#include "hicmath.h"
#include "hicrecord.h"

#include <cassert>

using namespace glich;
using std::string;

#define BASEDATE_Julian    1721058L

Field glich::julian_to_jdn( Field year, Field month, Field day )
{
    Field jdn =
        div_f( year, 4 ) * 1461 + mod_f( year, 4 ) * 365
        + latin_diy[month] + day + BASEDATE_Julian;

    // Adjust if in the 1st 2 months of 4 year cycle
    if ( month < 3 && ( year % 4 ) == 0 ) --( jdn );

    return jdn;
}

/*! Splits the given Julian Day Number date into the day, month and year
*  for the Julian Calendar.
*/
void glich::julian_from_jdn( Field* year, Field* month, Field* day, Field jdn )
{
    jdn -= BASEDATE_Julian;

    *year = div_f( jdn, 1461 ) * 4;
    jdn = mod_f( jdn, 1461 );

    if ( jdn < 60 )
    {
        if ( jdn < 31 )
        {
            *month = 1;
            *day = jdn + 1;
            return;
        }
        *month = 2;
        *day = jdn - 30;
        return;
    }
    --jdn; // remove the leap day
    *year += (int)jdn / 365;
    jdn %= 365;
    *month = 1;
    while ( jdn >= latin_diy[( *month ) + 1] ) ( *month )++;
    *day = jdn - latin_diy[*month] + 1;
    return;
}

/*! Return the jdn for Easter Sunday in the given year.
*/
Field glich::julian_easter( Field year )
{
    Field shifted_epact = ( 14 + 11 * ( year % 19 ) ) % 30;
    Field paschal_moon = julian_to_jdn( year, 4, 19 ) - shifted_epact;
    return kday_after( WDAY_Sunday, paschal_moon );
}

OptFieldID Julian::get_opt_field_id( const std::string& fieldname ) const
{
    return Base::get_opt_field_id( fieldname );
}

std::string Julian::get_opt_fieldname( OptFieldID field_id ) const
{
    return Base::get_opt_fieldname( field_id );
}

Field Julian::get_jdn( const Field* fields ) const
{
    if( fields[0] == f_invalid || fields[1] == f_invalid || fields[2] == f_invalid ) {
        return f_invalid;
    }
    return julian_to_jdn( fields[0], fields[1], fields[2] );
}

Field Julian::get_opt_field( const Field* fields, Field jdn, OptFieldID id ) const
{
    return Base::get_opt_field( fields, jdn, id );
}

bool Julian::set_fields_as_begin_first( Field* fields, const Field* mask ) const
{
    if( mask[0] == f_invalid ) {
        return false; // Must have at least year
    }
    if( mask[1] == f_invalid && mask[2] != f_invalid ) {
        return false; // Can't handle date lists
    }
    fields[0] = mask[0];
    fields[1] = ( mask[1] == f_invalid ) ? 1 : mask[1];
    fields[2] = ( mask[2] == f_invalid ) ? 1 : mask[2];
    return true;
}

bool Julian::set_fields_as_next_first( Field* fields, const Field* mask ) const
{
    return false;
}

bool Julian::set_fields_as_begin_last( Field* fields, const Field* mask ) const
{
    if( mask[0] == f_invalid ) {
        return false; // Must have at least year
    }
    if( mask[1] == f_invalid && mask[2] != f_invalid ) {
        return false; // Can't handle date lists
    }
    fields[0] = mask[0];
    fields[1] = ( mask[1] == f_invalid ) ? 12 : mask[1];
    if( mask[2] == f_invalid ) {
        fields[2] = last_day_in_month( fields[0], fields[1] );
        if( fields[2] == f_invalid ) {
            return false;
        }
    } else {
        fields[2] = mask[2];
    }
    return true;
}

bool Julian::set_fields_as_next_last( Field* fields, const Field* mask ) const
{
    return false;
}

void Julian::set_fields( Field* fields, Field jdn ) const
{
    julian_from_jdn( &fields[0], &fields[1], &fields[2], jdn );
}

Field Julian::get_rec_field_last( const Field* fields, size_t index ) const
{
    switch( index )
    {
    case 1: // Last month of year
        return 12;
    case 2: // Last day of month
        return last_day_in_month( fields[0], fields[1] );
    }
    return f_invalid;
}

/*! Returns the Julian Day Number for the given day, month and year
*  in the Julian Calendar.
*/
Field Julian::jdn( Field year, Field month, Field day ) const
{
    return julian_to_jdn( year, month, day );
}

/*! Return the jdn for Easter Sunday in the given year.
 */
Field Julian::easter( Field year ) const
{
    return julian_easter( year );
}

/*! Returns true if the year is a leap year in the Julian Calendar.
 */
bool Julian::is_leap_year( Field year ) const
{
    return ( year % 4 ) == 0;
}

/*! Returns the last day of the month for the given month and year.
 */
Field Julian::last_day_in_month( Field year, Field month ) const
{
    switch( month )
    {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
        return 31;
    case 4: case 6: case 9: case 11:
        return 30;
    case 2:
        return is_leap_year( year ) ? 29 : 28;
    }
    return f_invalid;
}

// End of src/glc/hicJulian.cpp file