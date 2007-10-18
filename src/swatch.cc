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
#include <gdkmm/general.h>  // cairo helpers
#include <glibmm-utils/exception.h>
#include "swatch.h"

namespace agave
{

    const double DEFAULT_BORDER_WIDTH = 1.0;
    const int MIN_SIZE = 10;

    struct Swatch::Priv
    {
        ColorModel::pointer m_model;
        double m_border_width;
        int m_padding;

        Priv () :
            m_border_width (DEFAULT_BORDER_WIDTH),
            m_padding (0)
        {}
    };

    Swatch::Swatch () :
        m_priv (new Priv())
    {
        ColorModel::pointer model (new ColorModel());
        init (model);
    }

    Swatch::Swatch (const ColorModel::pointer& model) :
        m_priv (new Priv())
    {
        init (model);
    }

    Swatch::Swatch (const Color& c) :
        m_priv (new Priv())
    {
        ColorModel::pointer model (new ColorModel(c));
        init (model);
    }

    void Swatch::init (const ColorModel::pointer& model)
    {
        request_size ();
        set_model (model);
    }

    void Swatch::request_size ()
    {
        THROW_IF_FAIL (m_priv);
        int sz = MIN_SIZE + 2 * m_priv->m_padding;
        if (m_priv->m_border_width > 0.0)
        {
            sz += static_cast<int>(2.0 * m_priv->m_border_width);
        }
        set_size_request (sz, sz);
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
        Gdk::Rectangle alloc = get_allocation ();
        double x, y, w, h;
        x = y = m_priv->m_padding;
        w = alloc.get_width () - 2 * m_priv->m_padding;
        h = alloc.get_height () - 2 * m_priv->m_padding;
        if (m_priv->m_border_width > 0.0)
        {
            x += m_priv->m_border_width / 2.0;
            y += m_priv->m_border_width / 2.0;
            w -= m_priv->m_border_width;
            h -= m_priv->m_border_width;
        }
        render_checks (cr, x, y, w, h);
        cr->rectangle (x, y, w, h);
        if (m_priv->m_border_width > 0.0)
        {
            cr->fill_preserve ();
            cr->set_line_width (m_priv->m_border_width);
            Gdk::Cairo::set_source_color (cr, get_style ()->get_fg (get_state ()));
            cr->stroke ();
        }
        else
        {
            cr->fill ();
        }
        return false;
    }

    void Swatch::on_color_changed ()
    {
        queue_draw ();
    }

    void Swatch::set_border_width (double width)
    {
        THROW_IF_FAIL (m_priv);
        m_priv->m_border_width = width;
        // redraw in case it changed
        queue_draw ();
    }

    void Swatch::set_padding (int padding)
    {
        THROW_IF_FAIL (m_priv);
        m_priv->m_padding = padding;
        // redraw in case it changed
        queue_draw ();
    }

    void Swatch::render_checks (Cairo::RefPtr<Cairo::Context>& cr, double x, double y, double w, double h)
    {
        const double CHECK_SIZE = 10.0;
        cr->save ();

        Cairo::RefPtr<Cairo::Surface> check = Cairo::Surface::create (cr->get_target (),
                Cairo::CONTENT_COLOR,
                static_cast<int>(2 * CHECK_SIZE), static_cast<int>(2 * CHECK_SIZE));

        /* Draw the check */
        {
            Cairo::RefPtr<Cairo::Context> cr2 = Cairo::Context::create (check);

            cr2->set_operator (Cairo::OPERATOR_SOURCE);

            cr2->set_source_rgb (0.4, 0.4, 0.4);

            cr2->rectangle (0, 0, 2 * CHECK_SIZE, 2 * CHECK_SIZE);
            cr2->fill ();

            cr2->set_source_rgb (0.7, 0.7, 0.7);

            cr2->rectangle (0.0, 0.0, CHECK_SIZE, CHECK_SIZE);
            cr2->fill ();
            cr2->rectangle (0.0 + CHECK_SIZE, 0.0 + CHECK_SIZE, CHECK_SIZE, CHECK_SIZE);
            cr2->fill ();
        }

        /* Fill the whole surface with the check */
        Cairo::RefPtr<Cairo::SurfacePattern> check_pattern = Cairo::SurfacePattern::create (check);
        check_pattern->set_extend (Cairo::EXTEND_REPEAT);
        cr->set_source (check_pattern);
        cr->rectangle (x, y, w, h);
        cr->fill ();

        cr->restore ();
    }
}
