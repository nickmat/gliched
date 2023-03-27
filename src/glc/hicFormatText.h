/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Name:        glc/glc/hicFormatText.h
 * Project:     Glich: Extendable Script Language.
 * Purpose:     FormatText class to control formatting and parsing of dates.
 * Author:      Nick Matthews
 * Website:     https://github.com/nickmat/glich
 * Created:     21st March 2023
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

#ifndef SRC_GLC_HICFORMATTEXT_H_GUARD
#define SRC_GLC_HICFORMATTEXT_H_GUARD

#include "hicFormat.h"

namespace glich {

    class FormatText : public Format
    {
    public:
        FormatText( const std::string& code, Grammar& gmr );

        void set_separators( const std::string& sep ) { m_separators = sep; }
        void set_rank_fieldnames( StdStrVec fieldnames ) { m_rank_fieldnames = fieldnames; }
        void set_rankout_fieldnames( StdStrVec fieldnames ) { m_rankout_fieldnames = fieldnames; }
        void set_control_out( const std::string& format ) { m_control_out = format; }
        void set_control_in( const std::string& format ) { m_control_in = format; }

        virtual std::string get_text_output( const Record& rec ) const override;
        virtual RList string_to_rlist( const Base& base, const std::string& input ) const override;

    private:
        Field get_field( const Record& record, const std::string& fname ) const;

        std::string m_control_in;
        std::string m_control_out;
        std::string m_separators;
        StdStrVec   m_rank_fieldnames;
        StdStrVec   m_rankout_fieldnames;
    };

}

#endif // SRC_GLC_HICFORMATTEXT_H_GUARD