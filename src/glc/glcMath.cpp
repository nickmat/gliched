/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/glcMath.cpp
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Miscellaneous math functions.
 * Author:      Nick Matthews
 * Website:     https://github.com/nickmat/glich
 * Created:     6th February 2023
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

#include "glcMath.h"

#include <cassert>
#include <cmath>

using namespace glich;

// Use our own value of pi for consistancy
double const glich::cal_pi = 3.141592653589793;

// Integer function to return floor( a / b )
Num glich::div_f( Num a, Num b )
{
    assert( b != 0 );
    Num q = a / b;
    if( ( a >= 0 && b > 0 ) || ( a < 0 && b < 0 ) || a % b == 0 ) {
        return q;
    }
    return q - 1;
}

// Integer function to return (a modulo b) that has the same sign as b.  
// Function based on Floored division.
Num glich::mod_f( Num a, Num b )
{
    assert( b != 0 );
    Num r = a % b;
    if( ( r > 0 && b < 0 ) || ( r < 0 && b > 0 ) ) {
        r += b;
    }
    return r;
}

// Integer function to return Euclidean division
Num glich::div_e( Num a, Num b )
{
    assert( b != 0 );
    Num q = a / b;
    if( a % b < 0 ) {
        if( b > 0 ) {
            q -= 1;
        } else {
            q += 1;
        }
    }
    return q;
}

// Integer function to return positive value for (a modulo b).
// Function based on Euclidean division.
Num glich::mod_e( Num a, Num b )
{
    assert( b != 0 );
    Num r = a % b;
    if( r < 0 ) {
        if( b > 0 ) { 
            r += b;
        } else {
            r -= b;
        }
    }
    return r;
}

Num glich::round_f( double r )
{
    return Num( r > 0.0 ? r + 0.5 : r - 0.5 );
}

Num glich::floor_f( double r )
{
    return Num( floor( r ) );
}


int glich::cal_signum( double n )
{
    if( n > 0 ) return 1;
    if( n < 0 ) return -1;
    return 0;
}

double glich::fmod_f( double x, double y )
{
    return x - y * floor( x / y ); 
} 

double glich::fmod_r( double x, double y )
{
    double f = floor( x / y );
    double r = x - y * f;
    if( r > y / 2 ) {
        return r - y;
    }
    return r;
} 


// End of src/glc/glcMath.cpp