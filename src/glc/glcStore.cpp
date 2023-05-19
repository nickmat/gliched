/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/glcStore.cpp
 * Project:     Glich: Extendable Script Language.
 * Purpose:     ScriptStore class definition header.
 * Author:      Nick Matthews
 * Website:     https://github.com/nickmat/glich
 * Created:     29th April 2023
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

#include "glcStore.h"

#include <cassert>

using namespace glich;
using std::string;

bool Store::create_local( const string& name )
{
    if( exists( name ) ) {
        return false;
    }
    m_variables[name] = SValue();
    return true;
}

SValue Store::get_local( const string& name )
{
    if( exists( name ) ) {
        return m_variables[name];
    }
    return SValue();
}

SValue* glich::Store::get_local_ptr( const std::string& name )
{
    if( exists( name ) ) {
        return &m_variables.at( name );
    }
    return nullptr;
}

bool Store::update_local( const string& name, SValue& value )
{
    if( exists( name ) ) {
        m_variables[name] = value;
        return true;
    }
    return false;
}



bool Store::add_local( const string& name, SValue& value )
{
    if( exists( name ) ) {
        return false;
    }
    m_variables[name] = value;
    return true;
}


// End of src/glc/glcStore.cpp source file.
