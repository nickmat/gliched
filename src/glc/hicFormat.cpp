/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/hicFormat.cpp
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Format class to control the formatting and parsing of dates.
 * Author:      Nick Matthews
 * Website:     https://github.com/nickmat/glich
 * Created:     24th March 2023
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

#include "hicFormat.h"
#include "hicRecord.h"

#include <cassert>

using namespace glich;
using std::string;

Format::Format( const std::string& code, Grammar& gmr )
    : m_code( code ), m_owner( gmr ), m_ok( false ), m_style( FormatStyle::Default )
{
}

Format::~Format()
{
}

string Format::jdn_to_string( const Base& base, Field jdn ) const
{
    if( jdn == f_minimum ) {
        return "past";
    }
    if( jdn == f_maximum ) {
        return "future";
    }
    Record rec( base, jdn );
    return get_text_output( rec );
}

string Format::range_to_string( const Base& base, Range rng ) const
{
    string output = jdn_to_string( base, rng.m_beg );
    output += "..";
    output += jdn_to_string( base, rng.m_end );
    return output;
}

string Format::rlist_to_string( const Base& base, RList rlist ) const
{
    string output;
    for( Range& rng : rlist ) {
        if( !output.empty() ) {
            output += " | ";
        }
        output += range_to_string( base, rng );
    }
    return output;
}

// End of src/cal/calformat.cpp file
