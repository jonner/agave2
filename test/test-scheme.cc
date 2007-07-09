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
#include <iostream>
#include "scheme.h"

using namespace agave;

int main ()
{
    hsv_t hsv = {0.5, 1.0, 1.0, 1.0};
    Color c (hsv);
    Scheme s(c, SCHEME_TYPE_ANALOGOUS);
    std::cout << s << std::endl;
    s.set_scheme_type (SCHEME_TYPE_MONOCHROMATIC);
    std::cout << s << std::endl;
    s.set_scheme_type (SCHEME_TYPE_TRIAD);
    std::cout << s << std::endl;
    s.set_scheme_type (SCHEME_TYPE_COMPLEMENTARY);
    std::cout << s << std::endl;
    s.set_scheme_type (SCHEME_TYPE_COMPOUND);
    std::cout << s << std::endl;
    s.set_scheme_type (SCHEME_TYPE_SHADES);
    std::cout << s << std::endl;
    s.set_scheme_type (SCHEME_TYPE_TETRAD);
    std::cout << s << std::endl;
    s.set_scheme_type (SCHEME_TYPE_CUSTOM);
    std::cout << s << std::endl;
}
