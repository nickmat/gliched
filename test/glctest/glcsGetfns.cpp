/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        test/glctest/glcGetfns.cpp
 * Project:     glctest: Command line Glich Test Script Program.
 * Purpose:     Read directory file listing.
 * Author:      Nick Matthews
 * Website:     https://github.com/nickmat/glich
 * Created:     7th February 2023
 * Copyright:   Copyright (c) 2023, Nick Matthews.
 * Licence:     GNU GPLv3
 *
 *  glctest is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  glctest is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with glctest.  If not, see <http://www.gnu.org/licenses/>.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

*/

#include "glcsTestmain.h"

#include <stdio.h>
#if defined(_MSC_VER)
#include <win/dirent.h>
#else
#include <dirent.h>
#endif

#include <cstring>
#include <iostream>

using std::vector;
using std::string;

CheckFile check_file( const std::string& name )
{
    DIR* dir = opendir( name.c_str() );
    if ( dir != nullptr ) {
        closedir( dir );
        return CheckFile::dir;
    }

    FILE* file = nullptr;
    fopen_s( &file, name.c_str(), "r" );
    if ( file != nullptr ) {
        fclose( file );
        return CheckFile::file;
    }
    return CheckFile::none;
}

void get_filenames( vector<string>& vec, const string& path )
{
    DIR* dir = opendir( path.c_str() );
    dirent* pdir = readdir( dir );
    while( pdir ) {
        if( pdir->d_name[0] == '.' ) { // Ignore hidden files/directories
            pdir = readdir( dir );
            continue;
        }
        string fname( pdir->d_name );
        if( pdir->d_type & DT_DIR ) {
            get_filenames( vec, path + "/" + fname );
        } else {
            size_t len = std::strlen( pdir->d_name );
            if( len > 4 && fname.substr( len - 5, 5 ) == ".glcs" ) {
                vec.push_back( path + "/" + fname );
            }
        }
        pdir = readdir( dir );
    }
    closedir( dir );
}


// End of test/glctest/glcGetfns.cpp