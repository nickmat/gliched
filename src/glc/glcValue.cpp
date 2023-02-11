#include "glcValue.h"
#include "glcValue.h"
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
    m_data = value.m_data;
}

string SValue::as_string() const
{
    string str;
    if( m_type == Type::Null ) {
        return "null";
    }
    switch( static_cast<di>(m_data.index()) ) {
    case di::di_bool:
        return std::get<bool>( m_data ) ? "true" : "false";
    case di::di_Num:
        return std::to_string( std::get<Num>( m_data ) );
    case di::di_string:
        return std::get<string>( m_data );
    }
    assert( false ); // This should not be possible.
    return string();
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

std::string glich::SValue::get_str( bool& success ) const
{
    if( std::holds_alternative<string>( m_data ) ) {
        success = true;
        return std::get<string>( m_data );
    }
    success = false;
    return string();
}

Num SValue::get_number() const
{
    if( std::holds_alternative<Num>( m_data ) ) {
        return std::get<Num>( m_data );
    }
    assert( false ); // Should only be called for Number type.
    return 0;
}

Num glich::SValue::get_number( bool& success ) const
{
    if( std::holds_alternative<Num>( m_data ) ) {
        success = true;
        return std::get<Num>( m_data );
    }
    success = false;
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

bool glich::SValue::get_bool( bool& success ) const
{
    if( std::holds_alternative<bool>( m_data ) ) {
        success = true;
        return std::get<bool>( m_data );
    }
    success = false;
    return false;
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
        string str1 = as_string();
        string str2 = value.as_string();
        set_str( str1 + str2 );
        return;
    }
    if( type() == Type::Number || value.type() == Type::Number ) {
        Num num1 = get_number();
        Num num2 = value.get_number();
        set_number( num1 + num2 );
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
        Num num1 = get_number();
        Num num2 = value.get_number();
        set_number( num1 - num2 );
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


// End of src/glc/glcValue.cpp