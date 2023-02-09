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

#include "glcMath.h"
#include "glcScript.h"

#include <cassert>

using namespace glich;
using std::string;


SValue::SValue( const SValue& value )
{
    m_type = value.m_type;
    m_str = value.m_str;
    m_num = value.m_num;
}

void SValue::set_error( const std::string& str )
{
    STokenStream* ts = Script::get_current_ts();
    m_type = Type::Error;
    if( ts ) {
        m_str = "Error (" + std::to_string( ts->get_line() ) + "): " + str;
    }
    else {
        m_str = str;
    }
}

string SValue::get_str() const
{
    assert( m_type == Type::String || m_type == Type::Error );
    return m_str;
}

Num SValue::get_number() const
{
    assert( m_type == Type::Number );
    return m_num;
}

bool SValue::get_bool() const
{
    assert( m_type == Type::Bool );
    return bool( m_num );
}

bool SValue::get( std::string& str ) const
{
    str.clear();
    switch( m_type )
    {
    case Type::Error:
    case Type::String:
        str = m_str;
        return true;
    case Type::Bool:
        str = get_bool() ? "true" : "false";
        return true;
    case Type::Number:
        str = std::to_string( m_num );
        return true;
    case Type::Null:
        str = "null";
        return true;
    }
    return false;
}

bool SValue::get( bool& b ) const
{
    if( m_type == Type::Bool ) {
        b = m_num;
        return true;
    }
    return false;
}

bool SValue::propagate_error( const SValue& value )
{
    if( is_error() ) {
        return true;
    }
    if( value.is_error() ) {
        m_type = Type::Error;
        m_str = value.m_str;
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
    if( m_type == Type::Null || value.type() == Type::Null ) {
        return set_bool( false );
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
    set_error( "Must compare same types." );
}

void SValue::plus( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    if( type() == Type::String || value.type() == Type::String ) {
        string str1, str2;
        bool success = get( str1 );
        if( success ) {
            success = value.get( str2 );
        }
        if( success ) {
            set_str( str1 + str2 );
            return;
        }
        set_error( "Unable to combine strings." );
        return;
    }
    if( type() == Type::Number || value.type() == Type::Number ) {
        m_num += value.get_number();
        return;
    }
    set_error( "Unable to add or subtract types." );
}

void SValue::minus( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    if( type() == Type::Number || value.type() == Type::Number ) {
        m_num -= value.get_number();
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
        m_num *= value.get_number();
        return;
    }
    set_error( "Can only multiply numbers." );
}

void SValue::divide( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    if( value.get_number() == 0 ) {
        set_error( "Division by zero." );
        return;
    }
    if( type() == Type::Number || value.type() == Type::Number ) {
        m_num = div_e( m_num, value.get_number() );
        return;
    }
    set_error( "Can only divide numbers." );
}

void SValue::modulus( const SValue& value )
{
    if( propagate_error( value ) ) {
        return;
    }
    if( value.get_number() == 0 ) {
        set_error( "Modulus by zero." );
        return;
    }
    if( type() == Type::Number || value.type() == Type::Number ) {
        m_num = mod_e( m_num, value.get_number() );
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
        m_num = -m_num;
        return;
    }
    set_error( "Can only negate number types." );
}

void SValue::logical_not()
{
    if( is_error() ) {
        return;
    }
    bool b;
    if( get( b ) ) {
        set_bool( !b );
        return;
    }
    set_error( "Logical 'not' only operates on bools" );
}


// End of src/glc/glcValue.cpp