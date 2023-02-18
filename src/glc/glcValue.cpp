/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/glcValue.cpp
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Class to hold all value types used by script.
 * Author:      Nick Matthews
 * Website:     https://github.com/nickmat/glich
 * Created:     5th February 2023
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

#include "glcValue.h"

#include "glcHelper.h"
#include "glcMath.h"
#include "glcScript.h"

#include <cassert>

using namespace glich;
using std::string;


SValue::SValue( const SValue& value )
{
    m_type = value.m_type;
    m_data = value.m_data;
}

string SValue::as_string() const
{
    switch( m_type )
    {
    case Type::Null:
        return "null";
    case Type::String:
    case Type::Error:
        return std::get<string>( m_data );
    case Type::Bool:
        return bool_to_string( std::get<bool>( m_data ) );
    case Type::Number:
        return std::to_string( std::get<Num>( m_data ) );
    case Type::field:
        return field_to_string( std::get<Num>( m_data ) );
    case Type::range:
        return range_to_string( std::get<Range>( m_data ) );
    case Type::rlist:
        return rlist_to_string( std::get<RList>( m_data ) );
    }
    return string();
}

void glich::SValue::set_range_demote( Range rng )
{
    if( rng.m_beg == rng.m_end ) {
        set_field( rng.m_beg );
    }
    else {
        set_range( rng );
    }
}

void glich::SValue::set_rlist_demote( const RList& rlist )
{
    if( rlist.size() == 1 ) {
        set_range_demote( rlist[0] );
    }
    else {
        set_rlist( rlist );
    }
}

void SValue::set_error( const std::string& str )
{
    STokenStream* ts = Script::get_current_ts();
    m_type = Type::Error;
    if( ts ) {
        m_data = "Error (" + std::to_string( ts->get_line() ) + "): " + str;
    }
    else {
        m_data = str;
    }
}

string SValue::get_str() const
{
    if( std::holds_alternative<string>( m_data ) ) {
        return std::get<string>( m_data );
    }
    assert( false ); // Should only be called for string types.
    return string();
}

Num SValue::get_number() const
{
    if( std::holds_alternative<Num>( m_data ) && m_type == Type::Number ) {
        return std::get<Num>( m_data );
    }
    assert( false ); // Should only be called for Number type.
    return 0;
}

bool SValue::get_bool() const
{
    if( std::holds_alternative<bool>( m_data ) ) {
        return std::get<bool>( m_data );
    }
    assert( false ); // Should only be called for bool type.
    return false;
}

Field SValue::get_field() const
{
    if( std::holds_alternative<Num>( m_data ) && m_type == Type::field ) {
        return std::get<Num>( m_data );
    }
    assert( false );
    return f_invalid;
}

Range SValue::get_range() const
{
    if( std::holds_alternative<Range>( m_data ) ) {
        return std::get<Range>( m_data );
    }
    assert( false );
    return Range( f_invalid, f_invalid );
}

RList SValue::get_rlist() const
{
    if( std::holds_alternative<RList>( m_data ) ) {
        return std::get<RList>( m_data );
    }
    assert( false );
    return RList( 0 );
}

Field glich::SValue::get_num_as_field() const
{
    if( std::holds_alternative<Num>( m_data ) && m_type == Type::Number ) {
        Num num = std::get<Num>( m_data );
        if( num >= f_maximum || num <= f_minimum ) {
            return f_invalid;
        }
        return num;
    }
    assert( false );
    return Field();
}

std::string glich::SValue::get_str( bool& success ) const
{
    if( std::holds_alternative<string>( m_data ) ) {
        success = true;
        return std::get<string>( m_data );
    }
    success = false;
    return string();
}

Num glich::SValue::get_number( bool& success ) const
{
    if( std::holds_alternative<Num>( m_data ) && m_type == Type::Number ) {
        success = true;
        return std::get<Num>( m_data );
    }
    Field fld = get_field( success );
    if( success ) {
        return FieldToNum( fld, success );
    }
    return 0;
}

bool SValue::get_bool( bool& success ) const
{
    if( std::holds_alternative<bool>( m_data ) ) {
        success = true;
        return std::get<bool>( m_data );
    }
    success = false;
    return false;
}

Field SValue::get_field( bool& success ) const
{
    success = true;
    if( std::holds_alternative<Num>( m_data ) && m_type == Type::field ) {
        return std::get<Num>( m_data );
    }
    if( std::holds_alternative<Range>( m_data ) ) {
        Range rng = std::get<Range>( m_data );
        if( rng.m_beg == rng.m_end ) {
            return rng.m_beg;
        }
    }
    if( std::holds_alternative<RList>( m_data ) ) {
        RList rl = std::get<RList>( m_data );
        if( rl.size() == 1 && rl[0].m_beg == rl[0].m_end ) {
            return rl[0].m_beg;
        }
    }
    if( std::holds_alternative<Num>( m_data ) && m_type == Type::Number ) {
        Num num = std::get<Num>( m_data );
        return FieldToNum( num, success );
    }
    success = false;
    return f_invalid;
}

Range SValue::get_range( bool& success ) const
{
    success = true;
    if( std::holds_alternative<Range>( m_data ) ) {
        return std::get<Range>( m_data );
    }
    if( std::holds_alternative<RList>( m_data ) ) {
        RList rl = std::get<RList>( m_data );
        if( rl.size() == 1 ) {
            return rl[0];
        }
    }
    success = false;
    return Range( f_invalid, f_invalid );
}

RList SValue::get_rlist( bool& success ) const
{
    success = true;
    if( std::holds_alternative<RList>( m_data ) ) {
        return std::get<RList>( m_data );
    }
    if( std::holds_alternative<Range>( m_data ) ) {
        Range rng = std::get<Range>( m_data );
        return { rng };
    }
    if( std::holds_alternative<Num>( m_data ) && m_type == Type::field ) {
        Field fld = std::get<Num>( m_data );
        return { Range( fld, fld ) };
    }
    success = false;
    return RList( 0 );
}

Num glich::SValue::get_integer( bool& success ) const
{
    success = true;
    if( std::holds_alternative<Num>( m_data ) ) {
        Num num = std::get<Num>( m_data );
        if( m_type == Type::field ) {
            num = NumToField( success );
        }
        return num;
    }
    success = false;
    return Num();
}

bool SValue::propagate_error( const SValue& value )
{
    if( is_error() ) {
        return true;
    }
    if( value.is_error() ) {
        m_type = Type::Error;
        m_data = value.as_string();
        return true;
    }
    return false;
}

void SValue::logical_or( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    set_bool( get_bool() || value.get_bool() );
}

void SValue::logical_and( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    set_bool( get_bool() && value.get_bool() );
}

void SValue::equal( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    if( m_type == value.type() ) {
        bool result = false;
        switch( m_type )
        {
        case Type::String:
            result = (get_str() == value.get_str());
            break;
        case Type::Bool:
            result = (get_bool() == value.get_bool());
            break;
        case Type::Number:
            result = (get_number() == value.get_number());
            break;
        case Type::Null:
            result = true;
            break;
        default:
            set_error( "Type cannot be compared." );
            return;
        }
        set_bool( result );
        return;
    }
    else if( is_integer() && value.is_integer() ) {
        bool success1, success2;
        Num num1 = get_integer( success1 );
        Num num2 = value.get_integer( success2 );
        if( !success1 || !success2 ) {
            set_bool( false );
            return;
        }
        bool result = false;
        if( num1 == num2 ) {
            result = true;
        }
        set_bool( result );
        return;
    }
    if( m_type == Type::Null || value.type() == Type::Null ) {
        set_bool( false );
        return;
    }
    set_error( "Must compare same types." );
}

void SValue::greater_than( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    if( m_type == value.type() ) {
        bool result = false;
        switch( m_type )
        {
        case Type::String:
            result = (get_str().compare( value.get_str() ) > 0);
            break;
        case Type::Bool:
            set_error( "Can only compare bool for equal or not equal." );
            return;
        case Type::Number:
            result = (get_number() > value.get_number());
            break;
        case Type::Null:
            set_error( "Can only compare a null value for equal or not equal." );
            return;
        default:
            set_error( "Type cannot be compared." );
            return;
        }
        set_bool( result );
        return;
    }
    else if( is_integer() && value.is_integer() ) {
        bool success1, success2;
        Num num1 = get_integer( success1 );
        Num num2 = value.get_integer( success2 );
        if( !success1 || !success2 ) {
            set_error( "Could not convert Field to Number." );
            return;
        }
        bool result = false;
        if( num1 > num2 ) {
            result = true;
        }
        set_bool( result );
        return;
    }
    set_error( "Must compare same types." );
}

void SValue::less_than( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    if( m_type == value.type() ) {
        bool result = false;
        switch( m_type )
        {
        case Type::String:
            result = (get_str().compare( value.get_str() ) < 0);
            break;
        case Type::Bool:
            set_error( "Can only compare bool for equal or not equal." );
            return;
        case Type::Number:
            result = (get_number() < value.get_number());
            break;
        case Type::Null:
            set_error( "Can only compare a null value for equal or not equal." );
            return;
        default:
            set_error( "Type cannot be compared." );
            return;
        }
        set_bool( result );
        return;
    }
    else if( is_integer() && value.is_integer() ) {
        bool success1, success2;
        Num num1 = get_integer( success1 );
        Num num2 = value.get_integer( success2 );
        if( !success1 || !success2 ) {
            set_error( "Could not convert Field to Number." );
            return;
        }
        bool result = false;
        if( num1 < num2 ) {
            result = true;
        }
        set_bool( result );
        return;
    }
    set_error( "Must compare same types." );
}

void SValue::plus( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    if( type() == Type::String || value.type() == Type::String ) {
        string str1 = as_string();
        string str2 = value.as_string();
        set_str( str1 + str2 );
        return;
    }
    Field fld = f_invalid;
    switch( type() )
    {
    case Type::Number:
        fld = get_num_as_field();
        switch( value.type() )
        {
        case Type::Number:
            set_number( get_number() + value.get_number() );
            return;
        case Type::field:
            set_field( add( fld, value.get_field() ) );
            return;
        case Type::range:
            set_range( add( value.get_range(), fld ) );
            return;
        case Type::rlist:
            set_rlist( add( value.get_rlist(), fld ) );
            return;
        }
        break;

    case Type::field:
        switch( value.type() )
        {
        case Type::Number:
            set_field( add( get_field(), value.get_num_as_field() ) );
            return;
        case Type::field:
            set_field( add( get_field(), value.get_field() ) );
            return;
        case Type::range:
            set_range( add( value.get_range(), get_field() ) );
            return;
        case Type::rlist:
            set_rlist( add( value.get_rlist(), get_field() ) );
            return;
        }
        break;
    case Type::range:
        switch( value.type() )
        {
        case Type::Number:
            set_range( add( get_range(), value.get_num_as_field() ) );
            return;
        case Type::field:
            set_range( add( get_range(), value.get_field() ) );
            return;
        case Type::range:
            set_range( add( value.get_range(), get_range() ) );
            return;
        case Type::rlist:
            set_rlist( add( value.get_rlist(), get_range() ) );
            return;
        }
        break;
    case Type::rlist:
        switch( value.type() )
        {
        case Type::Number:
            set_rlist( add( get_rlist(), value.get_num_as_field() ) );
            return;
        case Type::field:
            set_rlist( add( get_rlist(), value.get_field() ) );
            return;
        case Type::range:
            set_rlist( add( value.get_rlist(), get_range() ) );
            return;
        case Type::rlist:
            set_error( "Unable to add or subtract rlists." );
            return;
        }
        break;
    }
    set_error( "Unable to add or subtract types." );
}

void SValue::minus( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    if( ( m_type == Type::Number || m_type == Type::field ||
        m_type == Type::range || m_type == Type::rlist ) &&
        ( value.m_type == Type::Number || value.m_type == Type::field ||
            value.m_type == Type::range || value.m_type == Type::rlist )
        ) {
        SValue v( value );
        v.negate();
        plus( v );
        return;
    }
    set_error( "Unable to subtract types." );
}

void SValue::multiply( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    if( type() == Type::Number || value.type() == Type::Number ) {
        Num num1 = get_number();
        Num num2 = value.get_number();
        set_number( num1 * num2 );
        return;
    }
    set_error( "Can only multiply numbers." );
}

void SValue::divide( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    if( type() == Type::Number || value.type() == Type::Number ) {
        Num num1 = get_number();
        Num num2 = value.get_number();
        if( num2 == 0 ) {
            set_error( "Division by zero." );
            return;
        }
        set_number( div_e( num1, num2 ) );
        return;
    }
    set_error( "Can only divide numbers." );
}

void SValue::modulus( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    if( type() == Type::Number || value.type() == Type::Number ) {
        Num num1 = get_number();
        Num num2 = value.get_number();
        if( num2 == 0 ) {
            set_error( "Modulus by zero." );
            return;
        }
        set_number( mod_e( num1, num2 ) );
        return;
    }
    set_error( "Can only use modulus with numbers." );
}

void SValue::negate()
{
    switch( m_type )
    {
    case Type::Error:
        return;
    case Type::Number:
        set_number( -get_number() );
        return;
    case Type::field:
        if( get_field() != f_invalid ) {
            set_field( -get_field() );
        }
        return;
    case Type::range: {
        Range rng = get_range();
        if( rng.m_beg != f_invalid && rng.m_end != f_invalid ) {
            Field end = -rng.m_beg;
            rng.m_beg = -rng.m_end;
            rng.m_end = end;
        }
        return;
    }
    case Type::rlist: {
        RList rlist = get_rlist();
        for( Range rng : rlist ) {
            if( rng.m_beg != f_invalid && rng.m_end != f_invalid ) {
                Field end = -rng.m_beg;
                rng.m_beg = -rng.m_end;
                rng.m_end = end;
            }
        }
        std::reverse( rlist.begin(), rlist.end() );
        return;
    }
    }
    set_error( "Can only negate number types." );
}

void SValue::logical_not()
{
    switch( m_type )
    {
    case Type::Error:
        return;
    case Type::Bool:
        set_bool( !get_bool() );
        return;
    }
    set_error( "Logical 'not' only operates on bools" );
}

Field SValue::add( Field left, Field right ) const
{
    if( left == f_invalid || right == f_invalid ) {
        return f_invalid;
    }
    if( left == f_minimum || right == f_minimum ) {
        if( left == f_maximum || right == f_maximum ) {
            return f_invalid; // max + min = invalid
        }
        return f_minimum;
    }
    if( left == f_maximum || right == f_maximum ) {
        return f_maximum;
    }

    LongField lf = static_cast<LongField>(left) + static_cast<LongField>(right);
    if( lf < static_cast<LongField>(f_minimum) ) {
        return f_minimum;
    }
    if( lf > static_cast<LongField>(f_maximum) ) {
        return f_maximum;
    }
    return static_cast<Field>(lf);
}

Range SValue::add( Range range, Field field ) const
{
    return Range( add( range.m_beg, field ), add( range.m_end, field ) );
}

Range SValue::add( Range left, Range right ) const
{
    return Range(
        add( left.m_beg, right.m_beg ),
        add( left.m_end, right.m_end )
    );
}

RList SValue::add( RList rlist, Field field ) const
{
    RList rl;
    for( size_t i = 0; i < rlist.size(); i++ ) {
        Range range = add( rlist[i], field );
        rl.push_back( range );
    }
    return rl;
}

RList SValue::add( RList rlist, Range range ) const
{
    RList rl;
    for( size_t i = 0; i < rlist.size(); i++ ) {
        Range r = add( rlist[i], range );
        rl.push_back( r );
    }
    return rl;
}


// End of src/glc/glcValue.cpp