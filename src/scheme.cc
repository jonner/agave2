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

#include "scheme.h"

namespace agave
{

    struct Scheme::Priv
    {
    };

    Scheme Scheme::create (scheme_type_t scheme_type)
    {
        switch (scheme_type)
        {
            case SCHEME_TYPE_ANALOGOUS:
                break;
            case SCHEME_TYPE_MONOCHROMATIC:
                break;
            case SCHEME_TYPE_TRIAD:
                break;
            case SCHEME_TYPE_COMPLEMENTARY:
                break;
            case SCHEME_TYPE_COMPOUND:
                break;
            case SCHEME_TYPE_SHADES:
                break;
            case SCHEME_TYPE_TETRAD:
                break;
            case SCHEME_TYPE_CUSTOM:
                break;
            default:
                break;
        }
    }

    Scheme::Scheme () :
        m_priv (new Priv())
    {
    }
}
