/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        src/glc/glcScriptStore.h
 * Project:     Glich: Extendable Script Language.
 * Purpose:     ScriptStore class definition header.
 * Author:      Nick Matthews
 * Website:     https://github.com/nickmat/glich
 * Created:     5th February 2023
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

#ifndef SRC_GLC_GLCSCRIPTSTORE_H_GUARD
#define SRC_GLC_GLCSCRIPTSTORE_H_GUARD

#include "glcValue.h"

namespace glich {

    class Scheme;

    typedef std::map<std::string, SValue> SValueMap;

    class ScriptStore {
    public:
        ScriptStore()
            : m_prev( nullptr ) {}
        ScriptStore( ScriptStore* ss )
            : m_prev( ss ) {}

        ScriptStore* get_prev() const { return m_prev; }

        bool exists( const std::string& name ) const { return m_table.count( name ) == 1; }
        void set( const std::string& name, const SValue& value ) { m_table[name] = value; }
        bool get( SValue* value, const std::string& name ) const {
            if( exists( name ) ) {
                *value = m_table.at( name );
                return true;
            }
            return false;
        }
        void clear() {
            m_table.clear();
        }

    private:
        SValueMap m_table;
        ScriptStore* m_prev;
    };

}

#endif // SRC_GLC_GLCSCRIPTSTORE_H_GUARD
