/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/glcMark.h
 * Project:     Glich: Extendable Script Language.
 * Purpose:     Mark class used by the scripts mark statement.
 * Author:      Nick Matthews
 * Website:     https://github.com/nickmat/glich
 * Created:     8th February 2023
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

#ifndef SRC_GLC_GLCMARK_H_GUARD
#define SRC_GLC_GLCMARK_H_GUARD

#include "glc/glcDefs.h"

namespace glich {

    class Function;
    class File;

    class Mark
    {
    public:
        Mark( const std::string& name );
        ~Mark();

        std::string get_name() const { return m_name; }

        void add_function( Function* function ) { m_functions.push_back( function ); }
        void add_file( File* file ) { m_files.push_back( file ); }

        std::string remove_next_function();
        std::string remove_next_file();

    private:
        std::string            m_name;
        std::vector<Function*> m_functions;
        std::vector<File*>     m_files;
    };

}

#endif // SRC_GLC_GLCMARK_H_GUARD