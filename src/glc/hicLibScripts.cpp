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
 "mark \"__:hicslib:__\";\n"
 "set context hics;\n" },
    { "jdn",  /* jdn.glcs */
 "grammar d {\n"
 "fields day;\n"
 "alias pseudo { ddddd, day; }\n"
 "format d, \"{day}\";\n"
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
    { "julian",  /* julian.glcs */
 "lexicon m {\n"
 "name \"Month names\";\n"
 "fieldname month;\n"
 "lang en;\n"
 "pseudo Month, Mon;\n"
 "tokens {\n"
 "1, \"January\", \"Jan\";\n"
 "2, \"February\", \"Feb\";\n"
 "3, \"March\", \"Mar\";\n"
 "4, \"April\", \"Apr\";\n"
 "5, \"May\";\n"
 "6, \"June\", \"Jun\";\n"
 "7, \"July\", \"Jul\";\n"
 "8, \"August\", \"Aug\";\n"
 "9, \"September\", \"Sep\";\n"
 "10, \"October\", \"Oct\";\n"
 "11, \"November\", \"Nov\";\n"
 "12, \"December\", \"Dec\";\n"
 "}\n"
 "}\n"
 "grammar j {\n"
 "fields year month day;\n"
 "optional wday;\n"
 "lexicons m, w;\n"
 "alias pseudo { w wday; dd day; mm month; yyyy year; }\n"
 "alias unit { d day; m month; y year; }\n"
 "format dmy \"{day} |{month:m:a} |{year}\";\n"
 "format wdmy \"{wday:w:a} |{day} |{month:m:a} |{year}\";\n"
 "format out { output \"{day::os} |{month:m} |{year}\"; }\n"
 "format full { output \"{wday:w} |{day::os} |{month:m} |{year}\"; }\n"
 "format \"dmy+\" \"{day} |{month:m} |{year}\";\n"
 "format \"wdmy+\" \"{wday:w} |{day} |{month:m} |{year}\";\n"
 "format mdy \"{month:m:a} |{day}, |{year}\";\n"
 "format ymd \"{year}|:{month:m:a}|:{day}\";\n"
 "preferred dmy;\n"
 "}\n"
 "scheme j {\n"
 "name \"Julian\";\n"
 "base julian;\n"
 "grammar j;\n"
 "}\n" },
    { "gregorian",  /* gregorian.glcs */
 "scheme g {\n"
 "name \"Gregorian\";\n"
 "base gregorian;\n"
 "grammar j;\n"
 "}\n"
 "set inout g;\n" },
    { "jce",  /* jce.glcs */
 "lexicon ce {\n"
 "name \"Historic Era\";\n"
 "fieldname ce;\n"
 "lang en;\n"
 "pseudo CommonEra, CE;\n"
 "tokens {\n"
 "0, \"Before Common Era\", \"BCE\";\n"
 "1, \"Common Era\", \"CE\";\n"
 "}\n"
 "}\n"
 "grammar jce {\n"
 "fields year month day;\n"
 "calculated ce ceyear;\n"
 "optional wday;\n"
 "rank ce ceyear month day;\n"
 "calculate {\n"
 "output {: ,,,@if(year<1, 0, 1), @if(year<1, -year+1, year) };\n"
 "input {: @if(ce=1, ceyear, -ceyear+1) };\n"
 "}\n"
 "alias pseudo {\n"
 "w wday; dd day; mm month; yyyy ceyear; ce ce;\n"
 "}\n"
 "alias unit {\n"
 "d day; m month; y year;\n"
 "}\n"
 "lexicons m w ce;\n"
 "format dmye, \"{day} |{month:m:a} |{ceyear} |{ce:ce:a}\";\n"
 "format wdmye \"{wday:w:a} |{day} |{month:m:a} |{ceyear} |{ce:ce:a}\";\n"
 "format mdye \"{month:m:a} |{day} |{ceyear} |{ce:ce:a}\";\n"
 "format wmdye \"{wday:w:a} |{month:m:a} |{day} |{ceyear} |{ce:ce:a}\";\n"
 "format \"dmye+\" \"{day} |{month:m:a} |{ceyear} |{ce:ce}\";\n"
 "format \"wdmye+\" \"{wday:w} |{day} |{month:m} |{ceyear} |{ce:ce}\";\n"
 "format out {\n"
 "output \"{day::os} |{month:m} |{ceyear} |{ce:ce}\";\n"
 "}\n"
 "format full {\n"
 "output \"{wday:w} |{day::os} |{month:m} |{ceyear} |{ce:ce}\";\n"
 "}\n"
 "pref dmye;\n"
 "}\n"
 "scheme jce {\n"
 "name \"Julian Common Era\";\n"
 "base julian;\n"
 "grammar jce;\n"
 "}\n" },
};

size_t glich::hics_default_scripts_size =
    sizeof(glich::hics_default_scripts) / sizeof(glich::ScriptModule);

// End of lib/hics/hicScripts.f2c file
