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

#include "swatch.h"
#include "util.h"

namespace agave
{
    struct Swatch::Priv
    {
        ColorModel::pointer m_model;
    };

    Swatch::Swatch () :
        m_priv (new Priv())
    {
        ColorModel::pointer model (new ColorModel());
        set_model (model);
    }

    Swatch::Swatch (const ColorModel::pointer& model) :
        m_priv (new Priv())
    {
        set_model (model);
    }

    Swatch::Swatch (const Color& c) :
        m_priv (new Priv())
    {
        ColorModel::pointer model (new ColorModel(c));
        set_model (model);
    }

    void Swatch::set_model (const ColorModel::pointer& model)
    {
        THROW_IF_FAIL (m_priv);
        m_priv->m_model = model;
        if (m_priv->m_model)
        {
            m_priv->m_model->signal_color_changed ().connect (sigc::mem_fun
                    (this, &Swatch::on_color_changed));
        }
    }

    void Swatch::set_color (const Color& c)
    {
        THROW_IF_FAIL (m_priv && m_priv->m_model);
        m_priv->m_model->set_color (c);
    }

    ColorModel::pointer Swatch::get_model ()
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_model;
    }

    bool Swatch::on_expose_event (GdkEventExpose* event)
    {
        THROW_IF_FAIL (m_priv);
        Glib::RefPtr<Gdk::Window> win = get_window ();
        Cairo::RefPtr<Cairo::Context> cr = win->create_cairo_context ();
        if (event)
        {
            cr->rectangle (event->area.x, event->area.y, event->area.width,
                    event->area.height);
            cr->clip ();
        }
        Color c = m_priv->m_model->get_color ();
        cr->set_source_rgba (c.get_red (), c.get_green (), c.get_blue (),
                c.get_alpha ());
        cr->paint ();
        return false;
    }

    void Swatch::on_color_changed ()
    {
        queue_draw ();
    }
}
