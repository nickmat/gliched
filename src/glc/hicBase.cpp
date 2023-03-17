/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/hicBase.cpp
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

#include "hicBase.h"

#include "glcHelper.h"
#include "hicMath.h"


using namespace glich;
using std::string;


const char* Base::s_ymd_fieldnames[] = { "year", "month", "day" };
size_t Base::s_sizeof_ymd_fieldnames = sizeof(s_ymd_fieldnames) / sizeof(const char*);


Base::Base()
{
}

Base::Base( const string& data ) 
{
    string tail, word = get_first_word( data, &tail );
    while ( !word.empty() ) {
        set_data( word );
        word = get_first_word( tail, &tail );
    }
}

Base::~Base()
{
}

void Base::set_data( const string & data )
{
    string code, tail;
    split_code( &code, &tail, data );
    if ( code == "loc" ) {
        split_code( &code, &tail, tail );
        m_locale.lat = str_to_double( code );
        split_code( &code, &tail, tail );
        m_locale.lon = str_to_double( code );
    }
}

int Base::get_fieldname_index( const string& fieldname ) const
{
    int index = get_std_fieldname_index( fieldname );
    if( index < 0 ) {
        for( size_t i = 0 ; i < m_opt_fields.size() ; i++ ) {
            if( get_opt_fieldname( m_opt_fields[i] ) == fieldname ) {
                return record_size() + i;
            }
        }
    }
    return index;
}

string Base::get_fieldname( size_t index ) const
{
    if( index < record_size() ) {
        return get_std_fieldname( index );
    }
    if( index < extended_size() ) {
        return get_opt_fieldname( opt_index_to_id( index ) );
    }
    return "";
}

OptFieldID Base::get_opt_field_id( const string& fieldname ) const
{
    if ( fieldname == "wday" ) {
        return OFID_wday;
    } 
    if ( fieldname == "wsday" ) {
        return OFID_wsday;
    }
    if ( fieldname == "jwn" ) {
        return OFID_jwn;
    }
    if ( fieldname == "dayinyear" ) {
        return OFID_dayinyear;
    }
    if ( fieldname == "unshift" ) {
        return OFID_unshift;
    }
    if ( fieldname == "repeated" ) {
        return OFID_repeated;
    }
    return OFID_NULL;
}

string Base::get_opt_fieldname( OptFieldID field_id ) const
{
    switch( field_id )
    {
    case OFID_wday:
        return "wday";
    case OFID_wsday:
        return "wsday";
    case OFID_jwn:
        return "jwn";
    case OFID_dayinyear:
        return "dayinyear";
    case OFID_unshift:
        return "unshift";
    case OFID_repeated:
        return "repeated";
    default:
        return "";
    }
}

Field Base::get_opt_field( const Field* fields, Field jdn, OptFieldID id ) const
{
    switch( id )
    {
    case OFID_wday:
        return day_of_week( jdn ) + 1; // Mon=1, Sun=7
    case OFID_wsday:
        return day_of_week( jdn + 1 ) + 1; // Sun=1, Sun=7
    case OFID_jwn: // Julian Week Number
        return jdn / 7;
    }
    return f_invalid;
}

Field Base::get_opt_field( const Field* fields, Field jdn, int index ) const
{
    if ( index >= int( record_size() ) && index < int( extended_size() ) ) {
        OptFieldID id = opt_index_to_id( index );
        return get_opt_field( fields, jdn, id );
    }
    return f_invalid;
}

Field Base::get_field( const Field* fields, Field jdn, size_t index ) const
{
    if ( index < record_size() ) {
        return fields[index];
    }
    if ( index < extended_size() ) {
        OptFieldID id = m_opt_fields[index - record_size()];
        return get_opt_field( fields, jdn, id );
    }
    return f_invalid;
}

Field Base::get_field_first( const Field* fields, Field jdn, size_t index ) const
{
    if ( index < record_size() ) {
        return get_rec_field_first( fields, index );
    }
    if ( index < extended_size() ) {
        OptFieldID id = m_opt_fields[index - record_size()];
        return get_opt_field_first( fields, jdn, id );
    }
    return f_invalid;
}

Field Base::get_field_last( const Field* fields, Field jdn, size_t index ) const
{
    if ( index < record_size() ) {
        return get_rec_field_last( fields, index );
    }
    if ( index < extended_size() ) {
        OptFieldID id = m_opt_fields[index - record_size()];
        return get_opt_field_last( fields, jdn, id );
    }
    return f_invalid;
}

Field Base::get_rec_field_first( const Field* fields, size_t index ) const
{
    return 1;
}

Field Base::get_rec_field_last( const Field* fields, size_t index ) const
{
    return f_invalid;
}

Field Base::get_opt_field_first( const Field* fields, Field jdn, OptFieldID id ) const
{
    return 1;
}

Field Base::get_opt_field_last( const Field* fields, Field jdn, OptFieldID id ) const
{
    switch ( id )
    {
    case OFID_wday:
    case OFID_wsday:
        return 7;
    }
    return f_invalid;
}

int Base::get_ymd_fieldname_index( const string& fieldname ) const
{
    for( size_t i = 0; i < s_sizeof_ymd_fieldnames; i++ ) {
        if( fieldname == s_ymd_fieldnames[i] ) {
            return i;
        }
    }
    return -1;
}

string Base::get_ymd_fieldname( size_t index ) const
{
    if( index < s_sizeof_ymd_fieldnames ) {
        return s_ymd_fieldnames[index];
    }
    return "";
}


// End of src/cal/calbase.cpp file
