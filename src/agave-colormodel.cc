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
#include "agave-colormodel.h"

namespace agave
{
    ColorModel::ColorModel ()
    {
    }

    void ColorModel::set_color (const Color& c)
    {
        m_color = c;
        signal_color_changed ().emit ();
        m_color.signal_changed ().connect (sigc::mem_fun (this, &ColorModel::on_color_changed));
    }

    Color& ColorModel::get_color ()
    {
        return m_color;
    }

    void ColorModel::on_color_changed ()
    {
        signal_color_changed ().emit ();
    }
}
