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
        Color m_color;
        sigc::signal<void, const Color&> m_signal_color_changed;
    };

    Swatch::Swatch (const Color& c) :
        m_priv (new Priv())
    {
        set_color (c);
    }

    void Swatch::set_color (const Color& c)
    {
        THROW_IF_FAIL (m_priv);
        m_priv->m_color = c;
        m_priv->m_signal_color_changed.emit (c);
    }

    sigc::signal<void, const Color&> Swatch::signal_color_changed ()
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_signal_color_changed;
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
        rgb_t rgb = m_priv->m_color.as_rgb ();
        cr->set_source_rgba (rgb.r, rgb.g, rgb.b, rgb.a);
        cr->paint ();
        return false;
    }
}
