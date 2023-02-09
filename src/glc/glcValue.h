/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/glcValue.h
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Class to hold all value types used by script.
 * Author:      Nick Matthews
 * Website:     https://github.com/nickmat/glich
 * Created:     8th February 2023
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

#ifndef SRC_GLC_GLCVALUE_H_GUARD
#define SRC_GLC_GLCVALUE_H_GUARD

#include <glc/glcDefs.h>

namespace glich {

    class SValue
    {
    public:
        enum class Type {
            Null, Error, String, Number, Bool
        };
        SValue() : m_type( Type::Null ), m_num( 0 ) {}
        SValue( const SValue& value );
        SValue( const std::string& str ) : m_type(Type::String), m_str(str), m_num(0) {}
        SValue( Num num ) : m_type( Type::Number ), m_num( num ) {}
        SValue( bool b ) : m_type( Type::Bool ), m_num( b ) {}

        void set_str( const std::string& str ) { m_type = Type::String; m_str = str; }
        void set_bool( bool b ) { m_type = Type::Bool; m_num = b ? 1 : 0; }
        void set_number( Num num ) { m_type = Type::Number; m_num = num; }
        void set_error( const std::string& str );

        std::string get_str() const;
        Num get_number() const;
        bool get_bool() const;

        bool get( std::string& str ) const;
        bool get( bool& b ) const;

        bool is_error() const { return m_type == Type::Error; }
        bool propagate_error( const SValue& value );

        void logical_or( const SValue& value );
        void logical_and( const SValue& value );
        void equal( const SValue& value );
        void greater_than( const SValue& value );
        void less_than( const SValue& value );
        void plus( const SValue& value );
        void minus( const SValue& value );
        void multiply( const SValue& value );
        void divide( const SValue& value );
        void modulus( const SValue& value );

        void negate(); // Unitary minus
        void logical_not();

        Type type() const { return m_type; }

    private:
        Type        m_type;
        std::string m_str;
        Num         m_num;
    };

    using SValueVec = std::vector<SValue>;
}

#endif // SRC_GLC_GLCVALUE_H_GUARD