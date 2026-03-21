/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/gliched/geVersion.cpp
 * Project:     Gliched: Glich Script Language IDE.
 * Purpose:     Version constants and functions.
 * Author:      Nick Matthews
 * Created:     18th October 2023
 * Copyright:   Copyright (c) 2023, Nick Matthews.
 * Licence:     GNU GPLv3
 *
 *  Gliched is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Gliched is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Gliched.  If not, see <http://www.gnu.org/licenses/>.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

*/

#include "geVersion.h"


#define VERSION_STATUS   " alpha"

#if defined(NDEBUG) || !defined(_DEBUG)
#define VERSION_CONFIG   ""
#else
#define VERSION_CONFIG   " debug"
#endif

#define VERSION_NUM   "0.0.1"

#define VERSION   VERSION_NUM VERSION_STATUS VERSION_CONFIG

/*! A string containing the current full version number.
 */
const char* geVersion = VERSION;

/*! A string containing a long hand version name and copyright message.
 */
const char* geTitle = "Gliched: - V" VERSION "\n"
    "Copyright (c) 2023  Nick Matthews\n\n";

/***************************************************************************

 Source code revision history

 19sep23  v0.0.0   Work started on Gliched: An editor for Glich.

  active  v0.0.1   Create initial alpha version.

***************************************************************************/

// End of src/gliched/geVersion.cpp file
