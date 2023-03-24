/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/hicGrammar.cpp
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Grammar class to control date formatting.
 * Author:      Nick Matthews
 * Website:     https://github.com/nickmat/glich
 * Created:     24th March 2023
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

#include "hicGrammar.h"

#include <glc/glc.h>
#include "hicBase.h"
//#include "hicFormat.h"
//#include "hicFormatText.h"
#include "hicLexicon.h"

#include <cassert>

using namespace glich;
using std::string;


Grammar::Grammar( const string& code, Glich* glc )
    : m_code( code ), m_glc( glc ), m_ok( false )
{
}

Grammar::~Grammar()
{
}

bool Grammar::constuct( const Base* base )
{
    if( m_ok ) {
        return false; // Only run construct once.
    }
    // TODO:
    m_ok = true;
    return true;
}

Grammar* Grammar::create_default_grammar( const Base* base, Glich* glc )
{
    Grammar* gmr = new Grammar( "", glc );
    gmr->set_base_fieldnames( base->get_fieldnames() );
    // TODO: Add default formats.
    return gmr;
}


// End of src/cal/calgrammar.cpp file
