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
#ifndef __SCHEME_GENERATORS_H
#define __SCHEME_GENERATORS_H

#include "i-scheme-generator.h"

namespace agave
{
    class AnalogousSchemeGenerator : public ISchemeGenerator
    {
        public:
            unsigned int num_colors () const { return 5; }
            Glib::ustring scheme_name () const { return "Analogous"; }
            std::vector<Color>::const_iterator generate (const Color& c,
                    std::vector<Color>& generated_scheme);
    };

    class MonochromaticSchemeGenerator : public ISchemeGenerator
    {
        public:
            unsigned int num_colors () const { return 5; }
            Glib::ustring scheme_name () const { return "Monochromatic"; }
            std::vector<Color>::const_iterator generate (const Color& c,
                    std::vector<Color>& generated_scheme);
    };

    class TriadSchemeGenerator : public ISchemeGenerator
    {
        public:
            unsigned int num_colors () const { return 5; }
            Glib::ustring scheme_name () const { return "Triads"; }
            std::vector<Color>::const_iterator generate (const Color& c,
                    std::vector<Color>& generated_scheme);
    };

    class ComplementarySchemeGenerator : public ISchemeGenerator
    {
        public:
            unsigned int num_colors () const { return 5; }
            Glib::ustring scheme_name () const { return "Complementary"; }
            std::vector<Color>::const_iterator generate (const Color& c,
                    std::vector<Color>& generated_scheme);
    };

    class CompoundSchemeGenerator : public ISchemeGenerator
    {
        public:
            unsigned int num_colors () const { return 5; }
            Glib::ustring scheme_name () const { return "Compound"; }
            std::vector<Color>::const_iterator generate (const Color& c,
                    std::vector<Color>& generated_scheme);
    };

    class ShadesSchemeGenerator : public ISchemeGenerator
    {
        public:
            unsigned int num_colors () const { return 5; }
            Glib::ustring scheme_name () const { return "Shades"; }
            std::vector<Color>::const_iterator generate (const Color& c,
                    std::vector<Color>& generated_scheme);
    };

    class TetradSchemeGenerator : public ISchemeGenerator
    {
        public:
            unsigned int num_colors () const { return 5; }
            Glib::ustring scheme_name () const { return "Tetrads"; }
            std::vector<Color>::const_iterator generate (const Color& c,
                    std::vector<Color>& generated_scheme);
    };

    class CustomSchemeGenerator : public ISchemeGenerator
    {
        public:
            unsigned int num_colors () const { return 5; }
            Glib::ustring scheme_name () const { return "Custom"; }
            std::vector<Color>::const_iterator generate (const Color& c,
                    std::vector<Color>& generated_scheme);
    };
}

#endif // __SCHEME_GENERATORS_H
