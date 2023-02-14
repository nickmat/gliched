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

#include "glc/glcDefs.h"

#include <variant>

namespace glich {

    class SValue
    {
    public:
        enum class Type {
            Null, Error, String, Number, Bool, field, range, rlist
        };
        SValue() : m_type( Type::Null ) {}
        SValue( const SValue& value );
        SValue( const std::string& str ) : m_type( Type::String ), m_data( str ) {}
        SValue( const char* str ) : m_type( Type::String ), m_data( std::string( str ) ) {}
        SValue( Num num ) : m_type( Type::Number ), m_data( num ) {}
        SValue( int num ) : m_type( Type::Number ), m_data( static_cast<Num>(num) ) {}
        SValue( bool b ) : m_type( Type::Bool ), m_data( b ) {}
        SValue( CField f ) : m_type( Type::field ), m_data( f ) {}
        SValue( Range r ) : m_type( Type::range ), m_data( r ) {}
        SValue( const RList& rl ) : m_type( Type::rlist ), m_data( rl ) {}

        void set_str( const std::string& str ) { m_type = Type::String; m_data = str; }
        void set_bool( bool b ) { m_type = Type::Bool; m_data = b; }
        void set_number( Num num ) { m_type = Type::Number; m_data = num; }
        void set_field( Field fld ) { m_type = Type::field; m_data = Range( fld, fld ); }
        void set_range( Range rng ) { m_type = Type::range; m_data = rng; }
        void set_rlist( const RList& rlist ) { m_type = Type::rlist; m_data = rlist; }

        void set_range_demote( Range rng );
        void set_rlist_demote( const RList& rlist );

        void set_error( const std::string& str );

        std::string as_string() const;

        std::string get_str() const;
        Num get_number() const;
        bool get_bool() const;
        Field get_field() const;
        Range get_range() const;
        RList get_rlist() const;

        std::string get_str( bool& success ) const;
        Num get_number( bool& success ) const;
        bool get_bool( bool& success ) const;
        Field get_field( bool& success ) const; // Promote or demote if possible
        Range get_range( bool& success ) const; // Promote or demote if possible
        RList get_rlist( bool& success ) const;

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
        enum class di { di_bool, di_Num, di_string };
        std::variant<bool, Num, std::string, CField, Range, RList> m_data;
    };

    using SValueVec = std::vector<SValue>;
}

#endif // SRC_GLC_GLCVALUE_H_GUARD