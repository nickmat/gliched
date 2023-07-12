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
#include "hicIsoOrdinal.h"
#include "hicIsoWeek.h"
#include "hicJdn.h"
#include "hicJulian.h"
#include "hicOptional.h"
#include "hicRecord.h"

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
    Record record( base, jdn );
    string calc_output = base.get_calc_output();
    if( !calc_output.empty() ) {
        record.calculate_expression( calc_output );
    }
    return record.get_object( get_code() );
}

SValue Scheme::complete_object( const string& input, const string& fcode ) const
{
    Format* fmt = get_input_format( fcode );
    if( fmt == nullptr ) {
        return SValue();
    }
    return fmt->string_to_object( get_code(), m_base, input );
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

std::string Scheme::range_to_str( const Range& rng, const std::string& fcode ) const
{
    Format* fmt = get_output_format( fcode );
    if( fmt == nullptr ) {
        return string();
    }
    return fmt->range_to_string( m_base, rng );
}

std::string Scheme::rlist_to_str( const RList& rlist, const std::string& fcode ) const
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

string Scheme::object_to_str( const SValue& ovalue, const string& fcode ) const
{
    const Base& base = get_base();
    Record record( base, ovalue );
    Format* fmt = get_input_format( fcode );
    if( fmt == nullptr ) {
        return string();
    }
    return fmt->get_text_output( record );
}

// This currently expects the default fields to be complete on entry.
SValue Scheme::object_to_demoted_rlist( const SValue& ovalue ) const
{
    const Base& base = get_base();
    Record record( base, ovalue );
    record.calc_jdn();
    return SValue( record.get_jdn(), SValue::Type::field );
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
    case BaseName::isoweek:
        return new IsoWeek( data );
    case BaseName::isoordinal:
        return new IsoOrdinal( data );
    }
    return nullptr;
}

Base* Scheme::create_base_hybrid( const StdStrVec& fieldnames, const std::vector<HybridData>& data )
{
    Base* base = new Hybrid( fieldnames, data );
    if( base->is_ok() ) {
        return base;
    }
    delete base;
    return nullptr;
}

// End of src/glc/hicScheme.cpp file
