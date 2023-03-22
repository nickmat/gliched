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

Base::Base( const string& data )
{
    string tail, word = get_first_word( data, &tail );
    while( !word.empty() ) {
        set_data( word );
        word = get_first_word( tail, &tail );
    }
}

Base::~Base()
{
}

void Base::set_data( const string& data )
{
    string code, tail;
    split_code( &code, &tail, data );
    if( code == "loc" ) {
        split_code( &code, &tail, tail );
        m_locale.lat = str_to_double( code );
        split_code( &code, &tail, tail );
        m_locale.lon = str_to_double( code );
    }
}


// End of src/cal/calbase.cpp file
