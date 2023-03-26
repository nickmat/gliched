/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/hicFormatUnit.cpp
 * Project:     Glich: Extendable Script Language.
 * Purpose:     FormatUnit class to read and write as units.
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

#include "hicFormatUnit.h"

#include "hicBase.h"
#include "hicRecord.h"

using namespace glich;
using std::string;


FormatUnit::FormatUnit( const std::string& code, Grammar& gmr )
    : Format( code, gmr )
{
    set_user_input_str( "Value Unit ..." );
    set_user_output_str( "Value Unit ..." );
}

std::string glich::FormatUnit::get_text_output( const Record& record ) const
{
    string output;
    const Base& base = record.get_base();

    for( size_t i = 0; i < base.record_size(); i++ ) {
        Field fld = record.get_field( i );
        if( fld != f_invalid ) {
            string fname = base.get_fieldname( i );
            string uname = fname;// get_grammar().get_unitname( fname );
            if( !output.empty() ) {
                output += " ";
            }
            output += std::to_string( fld ) + uname;
        }
    }
    return output;
}


// End of src/cal/calformatunit.cpp file
