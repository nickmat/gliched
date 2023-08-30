/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/gu/guSch_g.cpp
 * Project:     glcUnit: Unit test program for the Glich Hics library.
 * Purpose:     Test Scheme "g" Gregorian conversions.
 * Author:      Nick Matthews
 * Website:     https://github.com/nickmat/glich
 * Created:     29th August 2023
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


const char* test_strs[guTT_size][2] = {
    //   dmy           mdy
    { "24 Jul -586", "Jul 24, -586" },
    { "5 Dec -168",  "Dec 5, -168"  },
    { "24 Sep 70",   "Sep 24, 70"   },
    { "2 Oct 135",   "Oct 2, 135"   },
    { "8 Jan 470",   "Jan 8, 470"   },
    { "20 May 576",  "May 20, 576"  },
    { "10 Nov 694",  "Nov 10, 694"  },
    { "25 Apr 1013", "Apr 25, 1013" },
    { "24 May 1096", "May 24, 1096" },
    { "23 Mar 1190", "Mar 23, 1190" },
    { "10 Mar 1240", "Mar 10, 1240" },
    { "2 Apr 1288",  "Apr 2, 1288"  },
    { "27 Apr 1298", "Apr 27, 1298" },
    { "12 Jun 1391", "Jun 12, 1391" },
    { "3 Feb 1436",  "Feb 3, 1436"  },
    { "9 Apr 1492",  "Apr 9, 1492"  },
    { "19 Sep 1553", "Sep 19, 1553" },
    { "5 Mar 1560",  "Mar 5, 1560"  },
    { "10 Jun 1648", "Jun 10, 1648" },
    { "30 Jun 1680", "Jun 30, 1680" },
    { "24 Jul 1716", "Jul 24, 1716" },
    { "19 Jun 1768", "Jun 19, 1768" },
    { "2 Aug 1819",  "Aug 2, 1819"  },
    { "27 Mar 1839", "Mar 27, 1839" },
    { "19 Apr 1903", "Apr 19, 1903" },
    { "25 Aug 1929", "Aug 25, 1929" },
    { "29 Sep 1941", "Sep 29, 1941" },
    { "19 Apr 1943", "Apr 19, 1943" },
    { "7 Oct 1943",  "Oct 7, 1943"  },
    { "17 Mar 1992", "Mar 17, 1992" },
    { "25 Feb 1996", "Feb 25, 1996" },
    { "10 Nov 2038", "Nov 10, 2038" },
    { "18 Jul 2094", "Jul 18, 2094" }
};


TEST_CASE( "Libray Scheme g", "[g]" )
{
    Scheme_info info;
    g_glc->get_scheme_info( &info, "g" );
    REQUIRE( info.name == "Gregorian" );
    REQUIRE( info.code == "g" );
    REQUIRE( info.grammar_code == "g" );
    REQUIRE( info.lexicon_codes[0] == "m" );
    REQUIRE( info.lexicon_codes[1] == "w" );
    REQUIRE( info.lexicon_names[0] == "Month names" );
    REQUIRE( info.lexicon_names[1] == "Weekday names Monday start" );
}

TEST_CASE( "Scheme g Test Table", "[g TestTable]" )
{
    for( size_t i = 0; i < guTT_size; i++ ) {
        Field jdn = g_glc->text_to_field( test_strs[i][0], "g:dmy" );
        REQUIRE( jdn == guTT_jdn[i] );
        string str = g_glc->field_to_text( jdn, "g:dmy" );
        REQUIRE( str == test_strs[i][0] );

        jdn = g_glc->text_to_field( test_strs[i][1], "g:mdy" );
        REQUIRE( jdn == guTT_jdn[i] );
        str = g_glc->field_to_text( jdn, "g:mdy" );
        REQUIRE( str == test_strs[i][1] );
    }
}

TEST_CASE( "Scheme g Test Range", "[g TestRange]" )
{
    struct data { string in; Range rng; string out; } t[] = {
        { "19sep1948", { 2432814, 2432814 }, "19 Sep 1948" },
        { "sep1948", { 2432796, 2432825 },"Sep 1948" },
        { "1948", { 2432552, 2432917 }, "1948" },
        { "1feb1948..29feb1948", { 2432583, 2432611 }, "Feb 1948" },
        { "1jan1948..31dec1948", { 2432552, 2432917 }, "1948" },
        { "1948..sep1948", { 2432552, 2432825 }, "Jan 1948..Sep 1948" },
        { "1948..19sep1948", { 2432552, 2432814 }, "1 Jan 1948..19 Sep 1948" },
        { "19sep1948..1948", { 2432814, 2432917 }, "19 Sep 1948..31 Dec 1948" }
    };
    size_t count = sizeof( t ) / sizeof( data );

    for( size_t i = 0; i < count; i++ ) {
        Range range = g_glc->text_to_range( t[i].in, "g:dmy" );
        REQUIRE( range == t[i].rng );
        string str = g_glc->range_to_text( range, "g:dmy" );
        REQUIRE( str == t[i].out );
    }
}

TEST_CASE( "Scheme g Test RList", "[g TestRList]" )
{
    string str = "30aug2023 | 1800..1837 | may2030..future | past..1756";
    RList expect = { { f_minimum, 2362791 }, { 2378497, 2392375 }, { 2460187, 2460187 }, { 2462623, f_maximum } };
    RList rlist = g_glc->text_to_rlist( str, "g:dmy" );
    REQUIRE( rlist.size() == expect.size() );
    for( size_t i = 0; i < expect.size(); i++ ) {
        REQUIRE( rlist[i] == expect[i] );
    }
    string result = g_glc->rlist_to_text( rlist, "g:dmy" );
    REQUIRE( result == "past..1756 | 1800..1837 | 30 Aug 2023 | May 2030..future" );
}

// End of test/gu/guSch_g.cpp file.
