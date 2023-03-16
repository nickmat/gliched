/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        include/utf8/utf8api.h
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Wrapper for the 3rd party utf8proc library.
 * Author:      Nick Matthews
 * Website:     https://github.com/nickmat/glich
 * Created:     13th October 2013
 * Copyright:   Copyright (c) 2013, Nick Matthews.
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


#ifndef UTF8API_H_GUARD
#define UTF8API_H_GUARD


#include <string>

namespace Utf8api {

    const char* version();
    std::string normal( const std::string& utf8 );

}

#endif // UTF8API_H_GUARD
