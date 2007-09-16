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
#include "color-relation.h"

namespace agave
{
    ColorRelation::ColorRelation(
            ColorModel::pointer src,
            ColorModel::pointer dest,
            color_generator_func func) :
        m_local_offset (0.0, 0.0, 0.0)
    {
        connect (src, dest, func);
    }

    void ColorRelation::connect (
            ColorModel::pointer src,
            ColorModel::pointer dest,
            color_generator_func func)
    {
        m_source = src;
        m_dest = dest;
        m_generator = func;
        if (m_source)
        {
            m_source->signal_color_changed ().connect (sigc::mem_fun (this,
                        &ColorRelation::on_source_color_changed));
        }
        if (m_dest)
        {
            m_dest->signal_color_changed ().connect (sigc::mem_fun (this,
                        &ColorRelation::on_dest_color_changed));
        }
    }

    void ColorRelation::on_source_color_changed ()
    {
        if (m_generator)
        {
            m_dest->set_color (m_generator (m_source->get_color () + m_local_offset));
        }
    }

    void ColorRelation::on_dest_color_changed ()
    {
        if (m_generator)
        {
            Color expected = m_generator (m_source->get_color ());
            m_local_offset = m_dest->get_color () - expected;
        }
    }
}
