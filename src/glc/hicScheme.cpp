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

#include "glcFunction.h"
#include "glcScript.h"
#include "glcStore.h"
#include "hicFormat.h"
#include "hicGrammar.h"
#include "hicGregorian.h"
#include "hicJdn.h"
#include "hicJulian.h"
#include "hicOptional.h"

#include <cassert>
#include <cstdlib>

using namespace glich;
using std::string;

glich::Scheme::Scheme( const std::string& code, const Base& base )
    : m_style( SCH_STYLE_Default ), m_base( base ), Object( code )
{
    set_value_names( base.get_fieldnames() );
    string calc_output = base.get_calc_output();
    if( !calc_output.empty() ) {
        Function* out_fun = new Function( "output" );
        out_fun->set_script( "result=this.mask(" + calc_output + ");" );
        add_function( out_fun );
    }
    string calc_input = base.get_calc_input();
    if( !calc_input.empty() ) {
        Function* in_fun = new Function( "input" );
        in_fun->set_script( "result=this.mask(" + calc_input + ");" );
        add_function( in_fun );
    }
}

Scheme::~Scheme()
{
    delete &m_base;
}

SValue Scheme::complete_object( Field jdn ) const
{
    const Base& base = get_base();
    FieldVec fields = base.get_fields( jdn );
    SValueVec vals( fields.size() + 1 );
    vals[0].set_str( get_code() );
    for( size_t i = 0; i < base.required_size(); i++ ) {
        if( fields[i] != f_invalid ) {
            vals[i + 1].set_field( fields[i] );
        }
    }
    for( size_t i = base.required_size(); i < base.record_size(); i++ ) {
        string name = base.get_fieldname( i );
        vals[i + 1] = GetOptional( name, jdn );
    }
    SValue value( vals );
    string calc_output = base.get_grammar()->get_calc_output();
    if( !calc_output.empty() ) {
        Store* store = new Store;
        const NameIndexMap& namemap = get_vnames_map();
        for( const auto npair : namemap ) {
            store->create_local( npair.first );
            store->update_local( npair.first, vals[npair.second] );
        }
        Glich* glc = SValue::get_glc();
        SValue calc = glc->evaluate( calc_output, store );
        value.mask_op( calc );
    }
    return value;
}

Format* Scheme::get_output_format( const string& fcode ) const
{
    if( fcode.empty() ) {
        return m_base.get_format( m_base.get_output_fcode() );
    }
    return m_base.get_format( fcode );
}

Format* Scheme::get_input_format( const string& fcode ) const
{
    if( fcode.empty() ) {
        return m_base.get_format( m_base.get_input_fcode() );
    }
    return m_base.get_format( fcode );
}

FieldVec Scheme::get_object_fields( const SValueVec& values ) const
{
    FieldVec fields( m_base.record_size(), f_invalid );
    if( values.size() <= 1 ) {
        return fields;
    }
    size_t size = std::min( m_base.record_size(), values.size() - 1 );
    bool success = false;
    for( size_t i = 0; i < size; i++ ) {
        Field fld = values[i + 1].get_field( success );
        if( success ) {
            fields[i] = fld;
        }
    }
    return fields;
}

std::string Scheme::jdn_to_str( Field jdn, const string& fcode ) const
{
    Format* fmt = get_output_format( fcode );
    if( fmt == nullptr ) {
        return string();
    }
    return fmt->jdn_to_string( m_base, jdn );
}

std::string Scheme::range_to_str( Range rng, const std::string& fcode ) const
{
    Format* fmt = get_output_format( fcode );
    if( fmt == nullptr ) {
        return string();
    }
    return fmt->range_to_string( m_base, rng );
}

std::string Scheme::rlist_to_str( RList rlist, const std::string& fcode ) const
{
    Format* fmt = get_output_format( fcode );
    if( fmt == nullptr ) {
        return string();
    }
    return fmt->rlist_to_string( m_base, rlist );
}

RList Scheme::str_to_rlist( const std::string& input, const std::string& fcode ) const
{
    Format* fmt = get_input_format( fcode );
    if( fmt == nullptr ) {
        return RList();
    }
    return fmt->string_to_rlist( m_base, input );
}

/* static */
Base* Scheme::create_base( BaseName bs, const std::string& data )
{
    switch( bs )
    {
    case BaseName::jdn:
        return new Jdn( data );
    case BaseName::julian:
        return new Julian( data );
    case BaseName::gregorian:
        return new Gregorian( data );
    }
    return nullptr;
}

// End of src/cal/calscheme.cpp file
