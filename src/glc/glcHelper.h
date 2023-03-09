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

    Num GetNum( const std::string& str );
    double GetFloat( const std::string& str );
    Field GetField( const std::string& str );
    Field NumToField( Num num, bool& success );
    Num FieldToNum( Field fld, bool& success );

    std::string bool_to_string( bool b );
    std::string float_to_string( double real );
    std::string field_to_string( Field fld );
    std::string range_to_string( Range rng );
    std::string rlist_to_string( RList rlist );

    bool is_name( const std::string& str );
}

#endif // SRC_GLC_GLCHELPER_H
