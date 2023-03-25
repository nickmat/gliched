/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/hicFormat.cpp
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Format class to control the formatting and parsing of dates.
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

#include "hicFormat.h"

//#include "calbase.h"
//#include "calgrammar.h"
//#include "calparse.h"
//#include "hicRecord.h"
//#include "caltext.h"
//#include "calvocab.h"

#include <cassert>

using namespace glich;
using std::string;

Format::Format( const std::string& code, Grammar* gmr )
    : m_code( code ), m_owner( gmr ), m_ok( false ), m_style( FormatStyle::Default )
{
}

Format::~Format()
{
}

// End of src/cal/calformat.cpp file
