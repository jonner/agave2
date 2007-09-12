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
        generated_scheme[0] = Color (hsv1);
        generated_scheme[1] = Color (hsv2);

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
        hsv_t hsv, hsv1, hsv2, hsv3, hsv4;
        hsv = hsv1 = hsv2 = hsv3 = hsv4 = c.as_hsv ();
        hsv1.h = hsv.h + 1./6.;
        hsv3.h = hsv.h + 0.56;
        hsv4.h = hsv.h + 0.56;

        // set 'saturation'
        if (hsv.s > 0.1)
        {
            hsv1.s = hsv.s - 0.1;
            hsv2.s = hsv.s - 0.1;
            hsv3.s = hsv.s - 0.1;
        }
        else
        {
            hsv1.s = hsv.s + 0.1;
            hsv2.s = hsv.s + 0.1;
            hsv3.s = hsv.s + 0.1;
        }
        if (hsv.s > 0.95)
        {
            hsv4.s = hsv.s - 0.05;
        }
        else
        {
            hsv4.s = hsv.s + 0.05;
        }
        // set 'value'
        if (hsv.v > 0.5)
        {
            hsv2.v = hsv.v - 0.3;
            hsv3.v = hsv.v - 0.2;
            hsv4.v = hsv.v - 0.3;
        }
        else if (hsv.v > 0.4)
        {
            hsv2.v = hsv.v + 0.3;
            hsv3.v = hsv.v - 0.2;
            hsv4.v = hsv.v + 0.3;
        }
        else
        {
            hsv2.v = hsv.v + 0.3;
            hsv3.v = hsv.v + 0.2;
            hsv4.v = hsv.v + 0.3;
        }

        generated_scheme[0] = Color (hsv1);
        generated_scheme[1] = Color (hsv2);
        generated_scheme[2] = c;
        generated_scheme[3] = Color (hsv3);
        generated_scheme[4] = Color (hsv4);

        return generated_scheme.begin () + 2;
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
