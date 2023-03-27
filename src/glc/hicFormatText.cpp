/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        glc/glc/hicFormatText.cpp
 * Project:     Glich: Extendable Script Language.
 * Purpose:     FormatText class to control formatting and parsing of dates.
 * Author:      Nick Matthews
 * Website:     https://github.com/nickmat/glich
 * Created:     21st March 2023
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

#include "hicFormatText.h"

#include "glcHelper.h"
#include "hicBase.h"
#include "hicElement.h"
#include "hicRecord.h"

#include <algorithm>
#include <cassert>

using namespace glich;
using std::string;


FormatText::FormatText( const string& code, Grammar& gmr )
: Format( code, gmr )//, m_separators(":,"), m_sig_rank_size(0), m_shorthand(true)
{
}

string FormatText::get_text_output( const Record& record ) const
{
    Element ele;
    string output, fieldout, value;
    enum State { ignore, dooutput, doelement };
    State state = dooutput;
    for( auto it = m_control_out.begin(); it != m_control_out.end(); it++ ) {
        switch( state )
        {
        case ignore:
            if( *it == '|' ) {
                state = dooutput;
            }
            break;
        case dooutput:
            if( *it == '|' ) {
                output += fieldout;
                fieldout.clear();
            }
            else if( *it == '{' ) {
                state = doelement;
            }
            else {
                fieldout += *it;
            }
            break;
        case doelement:
            if( *it == '}' ) {
                Field f = get_field( record, ele.get_field_name() );
                if( ele.has_dual_field_name() ) {
                    Field d = get_field( record, ele.get_dual_field_name() );
                    value = dual_fields_to_str( f, d );
                }
                else {
                    value = ele.get_formatted_element( get_glich(), f );
                }
                if( value.empty() ) {
                    fieldout.clear();
                    state = ignore;
                }
                else {
                    fieldout += value;
                    state = dooutput;
                }
                ele.clear();
            }
            else {
                ele.add_char( *it );
            }
            break;
        }
    }
    return output + fieldout;
}

RList FormatText::string_to_rlist( const Base& base, const string& input ) const
{
    return RList();
}

Field FormatText::get_field( const Record& record, const string& fname ) const
{
    int index = record.get_field_index( fname );
    if( index < 0 ) {
        return f_invalid;
    }
    return record.get_field( index );
}


// End of src/cal/calformattext.cpp file
