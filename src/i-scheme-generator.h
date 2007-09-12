/*******************************************************************************
 *
 *  Copyright (c) 2007 Jonathon Jongsma
 *
 *  This file is part of Agave
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>
 *
 *******************************************************************************/
#ifndef __I_SCHEME_GENERATOR_H
#define __I_SCHEME_GENERATOR_H

#include <vector>
#include "color.h"

namespace agave
{
    class ISchemeGenerator
    {
        public:
            /** Gets the number of colors that will be generated for this scheme
             */
            virtual unsigned int num_colors () const = 0;

            /** Gets the display name for this scheme type (e.g. "Triads")
             */
            virtual Glib::ustring scheme_name () const = 0;

            /** Generates a scheme from the provided color and returns an
             * iterator that points to the base color element in the generated
             * scheme
             *
             * @param c The base color for the color scheme
             * @param generated_scheme  Output parameter for the generated
             * scheme
             * @returns pointer to base color in the generated scheme
             */
            virtual std::vector<Color>::const_iterator generate (const Color& c,
                    std::vector<Color>& generated_scheme) = 0;

            virtual ~ISchemeGenerator () {}
    };
}

#endif // __I_SCHEME_GENERATOR_H
