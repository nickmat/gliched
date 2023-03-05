/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        glc/glc/glcHelper.cpp
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Helpful functions and strucs
 * Author:      Nick Matthews
 * Website:     https://github.com/nickmat/glich
 * Created:     4th March 2023
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


#include "glcHelper.h"

#include "glcMath.h"

#include <sstream>

using namespace glich;
using std::string;


Num glich::GetNum( const string& str )
{
    if( str.empty() ) {
        return 0;
    }
    return strtoll( str.c_str(), nullptr, 10 );
}

double glich::GetReal( const string& str ) {
    if( str.empty() ) {
        return 0.0;
    }
    return std::stod( str );
}

Field glich::GetField( const string& str )
{
    Num num = GetNum( str );
    if( num <= f_maximum && num >= f_minimum ) {
        return static_cast<Field>(num);
    }
    return f_invalid;
}

Field glich::NumToField( Num num, bool& success )
{
    if( num < f_maximum && num > f_minimum ) {
        success = true;
        return static_cast<Field>(num);
    }
    success = false;
    return f_invalid;
}

Num glich::FieldToNum( Field fld, bool& success )
{
    success = true;
    if( fld < f_maximum && fld > f_minimum ) {
        return static_cast<Num>(fld);
    }
    success = false;
    return 0;
}

string glich::bool_to_string( bool b ) {
    return b ? "true" : "false";
}

string glich::real_to_string( double real ) {
    if( isnan( real ) ) {
        return "nan";
    }
    if( is_zero( real ) ) {
        return "0.0";
    }
    std::stringstream ss;
    ss.precision( 6 );
    ss << std::fixed;
    ss << real;
    string res = ss.str();
    if( res.find( '.' ) != std::string::npos ) {
        size_t pos = res.length();
        while( res[pos - 1] == '0' ) {
            --pos;
        }
        if( res[pos - 1] == '.' && pos < res.length() ) {
            pos++;
        }
        res = res.substr( 0, pos );
    }
    return res;
}

string glich::field_to_string( Field fld ) {
    switch( fld )
    {
    case f_invalid:
        return "?";
    case f_maximum:
        return "+infinity";
    case f_minimum:
        return "-infinity";
    }
    return std::to_string( fld );
}

string glich::range_to_string( Range rng ){
    if( rng.m_beg == rng.m_end ) {
        return field_to_string( rng.m_beg );
    }
    return 
        field_to_string( rng.m_beg ) + ".." +
        field_to_string( rng.m_end );
}

string glich::rlist_to_string( RList rlist ) {
    if( rlist.empty() ) {
        return "empty";
    }
    std::string str;
    bool first = true;
    for( Range rng : rlist ) {
        if( !first ) {
            str += " | ";
        }
        str += range_to_string( rng );
        first = false;
    }
    return str;
}

bool glich::is_name( const std::string& str )
{
    if( str.empty() ) return false;
    bool first = true;
    for( auto ch : str ) {
        if( first ) {
            if( isdigit( ch ) ) return false;
            first = false;
        }
        if( isalnum( ch ) || ch == '_' || ch == ':' ) {
            continue;
        }
        return false;
    }
    // check for keywords
    const char* keywords[] =
    {
        "or", "and", "not", "div", "mod", "error"
    };
    for( auto word : keywords ) {
        if( strcmp( str.c_str(), word) == 0 ) {
            return false;
        }
    }
    return true;
}


// End of src/glc/glcHelper.cpp
