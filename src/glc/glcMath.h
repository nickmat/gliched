/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        include/glc/glcMath.h
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Miscellaneous math functions header.
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

#ifndef SRC_GLC_GLCMATH_H_GUARD
#define SRC_GLC_GLCMATH_H_GUARD

#include <glc/glcDefs.h>
#include <cmath>

namespace glich {

    //! We use our own value of pi for consistancy.
    extern const double cal_pi;

    //! Integer function to return floor( a / b )
    extern Num div_f( Num a, Num b );
    //! Integer function to return (a modulo b) that has the same sign as b.  
    extern Num mod_f( Num a, Num b );
    //! Modulus of a multiple of divisor equals the divisor (not zero)
    inline Num amod_f( Num a, Num b ) { return mod_f( a - 1, b ) + 1; }

    //! Integer function to return Euclidean division
    extern Num div_e( Num a, Num b );
    //! Integer function to return positive value for (a modulo b).
    extern Num mod_e( Num a, Num b );

    //! Convert double to Number by rounding.
    extern Num round_f( double r );
    //! Convert double to Number with floor.
    extern Num floor_f( double r );

    extern int cal_signum( double n );

    //! Modulus with return the same sign as y (divisor).
    extern double fmod_f( double x, double y );
    //! Modulus rounded towards zero.
    extern double fmod_r( double x, double y );

    // Inline functions
    double inline deg_to_rad( double deg ) { return deg * cal_pi / 180; }

    double inline rad_to_deg( double rad ) { return rad * 180 / cal_pi; }

    double inline sin_d( double deg ) { return sin( deg_to_rad( deg ) ); }
    double inline cos_d( double deg ) { return cos( deg_to_rad( deg ) ); }
    double inline tan_d( double deg ) { return tan( deg_to_rad( deg ) ); }
    double inline asin_d( double n ) { return rad_to_deg( asin( n ) ); }
    double inline acos_d( double n ) { return rad_to_deg( acos( n ) ); }
    double inline atan_d( double n ) { return rad_to_deg( atan( n ) ); }

}

#endif // SRC_GLC_GLCMATH_H_GUARD