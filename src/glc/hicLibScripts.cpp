/* ../../src/glc/hicLibScripts.cpp - File created by file2cpp 1.0.0 */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        lib/hics/hicScripts.f2c
 * Project:     Glich: Extendable Script Language.
 * Purpose:     file2cpp template file for hics library scripts.
 * Author:      Nick Matthews
 * Website:     https://github.com/nickmat/glich
 * Created:     1st April 2023
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

 This template is used to create the src/glc/hicLibScripts.cpp file.

*/

#include "hicLibScripts.h"

glich::ScriptModule glich::hics_default_scripts[] = {
    { "hicLib",  /* hicLib.glcs */
 "mark \"__:hicslib:__\";\n" },
    { "jdn",  /* jdn.glcs */
 "grammar d {\n"
 "fields day;\n"
 "alias pseudo { ddddd, day; }\n"
 "format d, \"(day)\";\n"
 "preferred d;\n"
 "}\n"
 "scheme jdn {\n"
 "name \"Julian Day Number\";\n"
 "base jdn;\n"
 "grammar d;\n"
 "}\n" },
    { "week",  /* week.glcs */
 "lexicon w {\n"
 "name \"Weekday names\";\n"
 "fieldname wday;\n"
 "lang en;\n"
 "pseudo Weekday, WDay;\n"
 "tokens {\n"
 "1, \"Monday\", \"Mon\";\n"
 "2, \"Tuesday\", \"Tue\";\n"
 "3, \"Wednesday\", \"Wed\";\n"
 "4, \"Thursday\", \"Thur\";\n"
 "5, \"Friday\", \"Fri\";\n"
 "6, \"Saturday\", \"Sat\";\n"
 "7, \"Sunday\", \"Sun\";\n"
 "}\n"
 "}\n"
 "lexicon ws {\n"
 "name \"Weekday names\";\n"
 "fieldname wsday;\n"
 "lang en;\n"
 "pseudo Weekday, WDay;\n"
 "tokens {\n"
 "1, \"Sunday\", \"Sun\";\n"
 "2, \"Monday\", \"Mon\";\n"
 "3, \"Tuesday\", \"Tue\";\n"
 "4, \"Wednesday\", \"Wed\";\n"
 "5, \"Thursday\", \"Thur\";\n"
 "6, \"Friday\", \"Fri\";\n"
 "7, \"Saturday\", \"Sat\";\n"
 "}\n"
 "}\n" },
};

size_t glich::hics_default_scripts_size =
    sizeof(glich::hics_default_scripts) / sizeof(glich::ScriptModule);

// End of lib/hics/hicScripts.f2c file
