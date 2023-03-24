/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/hicGrammar.h
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

#ifndef GLC_HICGRAMMAR_H_GUARD
#define GLC_HICGRAMMAR_H_GUARD

#include "hicHelper.h"

namespace glich {

    class Glich;
    class Base;

    class Grammar
    {
    public:
        Grammar( const std::string& code, Glich* glc );
        ~Grammar();

        bool is_ok() const { return m_ok; }
        bool constuct( const Base* base );

        void set_base_fieldnames( StdStrVec fieldnames ) { m_base_fieldnames = fieldnames; }

        std::string get_code() const { return m_code; }

        StdStrVec get_base_fieldnames() const { return m_base_fieldnames; }

        static Grammar* create_default_grammar( const Base* base, Glich* glc );

    private:
        Glich*      m_glc;
        std::string m_code;
        bool        m_ok;
        StdStrVec   m_base_fieldnames;
    };

}

#endif // GLC_HICGRAMMAR_H_GUARD
