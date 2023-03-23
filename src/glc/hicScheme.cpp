/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/hicScheme.cpp
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Scheme class implimentation.
 * Author:      Nick Matthews
 * Website:     https://github.com/nickmat/glich
 * Created:     17th March 2023
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

#include "hicScheme.h"

#include "hicJdn.h"
#include "hicJulian.h"
#include "hicRecord.h"

#include <cassert>
#include <cstdlib>

using namespace glich;
using std::string;

glich::Scheme::Scheme( const std::string& code, const Base* base )
    : m_style( SCH_STYLE_Default ), m_base( base ), Object( code )
{
    assert( m_base != nullptr );
    set_value_names( base->get_fieldnames() );
}

Scheme::~Scheme()
{
    delete m_base;
}

FieldVec Scheme::get_object_fields( const SValueVec& values ) const
{
    FieldVec fields( m_base->record_size(), f_invalid );
    if( values.size() <= 1 ) {
        return fields;
    }
    size_t size = std::min( m_base->record_size(), values.size() - 1 );
    bool success = false;
    for( size_t i = 0; i < size; i++ ) {
        Field fld = values[i + 1].get_field( success );
        if( success ) {
            fields[i] = fld;
        }
    }
    return fields;
}

Base* Scheme::create_base( BaseName bs, const std::string& data )
{
    switch( bs )
    {
    case BaseName::jdn:
        return new Jdn( data );
    case BaseName::julian:
        return new Julian( data );
    }
    return nullptr;
}

// End of src/cal/calscheme.cpp file
