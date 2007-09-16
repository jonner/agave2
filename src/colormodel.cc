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
#include "colormodel.h"

namespace agave
{
    ColorModel::ColorModel (const Color& c)
    {
        set_color (c);
        m_color.signal_changed ().connect (sigc::mem_fun (this, &ColorModel::on_color_changed));
    }

    void ColorModel::set_color (const Color& c)
    {
        m_color = c;
        signal_color_changed ().emit ();
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
