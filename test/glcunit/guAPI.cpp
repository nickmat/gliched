/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/gu/guAPI.cpp
 * Project:     glcUnit: Unit test program for the Glich Hics library.
 * Purpose:     Test general Glich API functions.
 * Author:      Nick Matthews
 * Website:     http://thefamilypack.org
 * Created:     17th August 2023
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

#include <catch2/catch.hpp>

#include "guMain.h"

using namespace glich;
using std::string;

TEST_CASE( "Test Version", "[Version]" )
{
    string ver = g_glc->version();
    string ver_num = ver.substr( 0, 5 );
    REQUIRE( ver_num == "0.3.0" );
}

// End of test/gu/guAPI.cpp file.
