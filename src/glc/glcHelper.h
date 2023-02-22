/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        glc/glc/glcHelper.h
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Helpful functions and strucs
 * Author:      Nick Matthews
 * Website:     https://github.com/nickmat/glich
 * Created:     13th February 2023
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

#ifndef SRC_GLC_GLCHELPER_H
#define SRC_GLC_GLCHELPER_H

#include <glc/glcDefs.h>

namespace glich {

    inline Num GetNum( const std::string& str )
    {
        if( str.empty() ) {
            return 0;
        }
        return strtoll( str.c_str(), nullptr, 10 );
    }

    inline Field NumToField( Num num )
    {
        if( num <= f_maximum && num >= f_minimum ) {
            return static_cast<Field>(num);
        }
        return f_invalid;
    }

    inline Field NumToField( Num num, bool& success )
    {
        if( num < f_maximum && num > f_minimum ) {
            success = true;
            return static_cast<Field>(num);
        }
        success = false;
        return f_invalid;
    }

    inline Num FieldToNum( Field fld, bool& success )
    {
        success = true;
        if( fld < f_maximum && fld > f_minimum ) {
            return static_cast<Num>(fld);
        }
        success = false;
        return 0;
    }

    inline std::string bool_to_string( bool b ) {
        return b ? "true" : "false";
    }

    inline std::string field_to_string( Field fld ) {
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

    inline std::string range_to_string( Range rng ){
        if( rng.m_beg == rng.m_end ) {
            return field_to_string( rng.m_beg );
        }
        return 
            field_to_string( rng.m_beg ) + ".." +
            field_to_string( rng.m_end );
    }

    inline std::string rlist_to_string( RList rlist ) {
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
}

#endif // SRC_GLC_GLCHELPER_H
