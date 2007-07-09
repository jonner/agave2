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

#include "scheme-generators.h"

namespace agave
{

    std::vector<Color>::const_iterator
    AnalogousSchemeGenerator::generate (const Color& c,
            std::vector<Color>& generated_scheme)
    {
        generated_scheme.resize (num_colors (), c);
        // FIXME: algorithm needs some work
        std::vector<Color>::iterator base_iter;

        // add some shifted colors to the left of the base color
        Color c1, c2, c3, c4;
        c1 = c2 = c3 = c4 = c;
        c1.shift_hue (0.1666);    // -60 degrees
        generated_scheme[0] = c1;
        c2.shift_hue (0.0833);    // -30 degrees
        generated_scheme[1] = c2;

        // add the base color to the scheme
        generated_scheme[2] = c;
        base_iter = generated_scheme.begin () + 2;

        // add some shifted colors to the right of the base color
        c3.shift_hue (-0.0833);    // +30 degrees
        generated_scheme[3] = c3;
        c4.shift_hue (-0.1666);    // +60 degrees
        generated_scheme[4] = c4;

        return base_iter;
    }

    std::vector<Color>::const_iterator
    MonochromaticSchemeGenerator::generate (const Color& c,
            std::vector<Color>& generated_scheme)
    {
        generated_scheme.resize (num_colors (), c);
        // FIXME: algorithm needs some work
        std::vector<Color>::const_iterator base_iter;

        // add colors to the left of the base color
        hsv_t hsv_base = c.as_hsv ();
        hsv_t hsv1, hsv2, hsv3, hsv4;
        hsv1 = hsv2 = hsv3 = hsv4 = hsv_base;
        if (hsv_base.s < 0.40)
        {
            hsv1.s += 0.30;
            hsv3.s += 0.30;
        }
        else
        {
            hsv1.s -= 0.30;
            hsv3.s -= 0.30;
        }
        if (hsv_base.v > 0.70)
        {
            hsv2.v -= 0.50;
            hsv3.v -= 0.50;
            hsv4.v -= 0.20;
        }
        else if (hsv_base.v > 0.40)
        {
            hsv2.v += 0.30;
            hsv3.v += 0.30;
            hsv4.v -= 0.20;
        }
        else
        {
            hsv2.v += 0.30;
            hsv3.v += 0.30;
            hsv4.v += 0.60;
        }
        generated_scheme[0] = Color (hsv2);
        generated_scheme[1] = Color (hsv1);

        // add the base color to the scheme
        generated_scheme[2] = c;
        base_iter = generated_scheme.begin () + 2;

        generated_scheme[3] = Color (hsv3);
        generated_scheme[4] = Color (hsv4);

        return base_iter;
    }

    std::vector<Color>::const_iterator
    TriadSchemeGenerator::generate (const Color& c,
            std::vector<Color>& generated_scheme)
    {
        generated_scheme.resize (num_colors (), c);
        // FIXME: algorithm needs some work
        return generated_scheme.begin ();
    }

    std::vector<Color>::const_iterator
    ComplementarySchemeGenerator::generate (const Color& c,
            std::vector<Color>& generated_scheme)
    {
        generated_scheme.resize (num_colors (), c);
        // FIXME: algorithm needs some work
        return generated_scheme.begin ();
    }

    std::vector<Color>::const_iterator
    CompoundSchemeGenerator::generate (const Color& c,
            std::vector<Color>& generated_scheme)
    {
        generated_scheme.resize (num_colors (), c);
        // FIXME: algorithm needs some work
        return generated_scheme.begin ();
    }

    std::vector<Color>::const_iterator
    ShadesSchemeGenerator::generate (const Color& c,
            std::vector<Color>& generated_scheme)
    {
        generated_scheme.resize (num_colors (), c);
        // FIXME: algorithm needs some work
        return generated_scheme.begin ();
    }

    std::vector<Color>::const_iterator
    TetradSchemeGenerator::generate (const Color& c,
            std::vector<Color>& generated_scheme)
    {
        generated_scheme.resize (num_colors (), c);
        // FIXME: algorithm needs some work
        return generated_scheme.begin ();
    }

    std::vector<Color>::const_iterator
    CustomSchemeGenerator::generate (const Color& c,
            std::vector<Color>& generated_scheme)
    {
        generated_scheme.resize (num_colors (), c);
        // if there were any colors already in the theme, just leave them alone
        // since they're as good as any other colors.  Just make sure that the
        // 'base' color is set.
        generated_scheme[2] = c;
        return generated_scheme.begin () + 2;
    }
}
