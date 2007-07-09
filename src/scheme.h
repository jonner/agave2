/*******************************************************************************
 *  PROJECT: Agave
 *
 *  AUTHOR: Jonathon Jongsma
 *
 *  Copyright (c) 2007 Jonathon Jongsma
 *
 *  License:
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the
 *    Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 *    Boston, MA  02111-1307  USA
 *
 *******************************************************************************/
#ifndef __SCHEME_H
#define __SCHEME_H

#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "color.h"

namespace agave
{
    typedef enum
    {
        SCHEME_TYPE_ANALOGOUS,
        SCHEME_TYPE_MONOCHROMATIC,
        SCHEME_TYPE_TRIAD,
        SCHEME_TYPE_COMPLEMENTARY,
        SCHEME_TYPE_COMPOUND,
        SCHEME_TYPE_SHADES,
        SCHEME_TYPE_TETRAD, // not used by kuler
        SCHEME_TYPE_CUSTOM
    } scheme_type_t;

    class Scheme
    {
        public:
            Scheme ();
            Scheme (const Color& base_color, scheme_type_t scheme_type);

            std::vector<Color>& colors ();
            const std::vector<Color>& colors () const;
            void set (const Color& base_color, scheme_type_t scheme_type);
            void set_base_color (const Color& color);
            Color get_base_color () const;
            void set_scheme_type (scheme_type_t scheme_type);
            scheme_type_t get_scheme_type () const;

            friend std::ostream& operator<<(std::ostream& out, const Scheme& s);

        private:
            struct Priv;
            boost::shared_ptr<Priv> m_priv;
    };
}

#endif // __SCHEME_H
