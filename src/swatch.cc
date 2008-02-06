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
#include <list>
#include <gdkmm/general.h>  // cairo helpers
#include <glibmm-utils/exception.h>
#include <gtkmm/drawingarea.h>
#include "swatch.h"
#include "colormodel.h"

namespace agave
{

    const double DEFAULT_BORDER_WIDTH = 1.0;
    const int MIN_SIZE = 10;
    static const unsigned int RED_BYTE_POS = 3;
    static const unsigned int GREEN_BYTE_POS = 2;
    static const unsigned int BLUE_BYTE_POS = 1;

    struct Swatch::Priv :
        public Gtk::DrawingArea
    {
        boost::shared_ptr<ColorModel> m_model;
        double m_border_width;
        int m_padding;

        Priv (const boost::shared_ptr<ColorModel>& model) :
            m_border_width (DEFAULT_BORDER_WIDTH),
            m_padding (0)
        {
            set_model (model);
            request_size ();

            // Targets for drag and drop:
            std::list<Gtk::TargetEntry> drag_targets;
            drag_targets.push_back(Gtk::TargetEntry("application/x-color"));
            drag_targets.push_back(Gtk::TargetEntry("UTF8_STRING"));
            drag_targets.push_back(Gtk::TargetEntry("text/plain"));

            // set the swatch as a drag source
            drag_source_set(drag_targets);
            signal_drag_data_get().connect(sigc::mem_fun(*this, &Priv::on_drag_data_get));
            signal_drag_begin().connect(sigc::mem_fun(*this, &Priv::set_color_icon));
        }

        void set_model (const boost::shared_ptr<ColorModel>& model)
        {
            m_model = model;
            m_model->signal_color_changed ().connect (sigc::mem_fun
                    (this, &Priv::on_color_changed));
        }

        virtual bool on_expose_event (GdkEventExpose* event)
        {
            Glib::RefPtr<Gdk::Window> win = get_window ();
            Cairo::RefPtr<Cairo::Context> cr = win->create_cairo_context ();
            if (event)
            {
                cr->rectangle (event->area.x, event->area.y, event->area.width,
                        event->area.height);
                cr->clip ();
            }
            Gdk::Rectangle alloc = get_allocation ();
            render_swatch (cr, alloc.get_width(), alloc.get_height ());
            return false;
        }

        void on_color_changed ()
        {
            queue_draw ();
        }

        void render_swatch (Cairo::RefPtr<Cairo::Context>& cr, double w, double h)
        {
            double x, y;
            x = y = m_padding;
            w -= 2 * m_padding;
            h -= 2 * m_padding;
            if (m_border_width > 0.0)
            {
                x += m_border_width / 2.0;
                y += m_border_width / 2.0;
                w -= m_border_width;
                h -= m_border_width;
            }
            render_checks (cr, x, y, w, h);
            cr->rectangle (x, y, w, h);
            if (m_border_width > 0.0)
            {
                Color c = m_model->get_color ();
                cr->set_source_rgba (c.get_red (), c.get_green (), c.get_blue (),
                        c.get_alpha ());
                cr->fill_preserve ();
                cr->set_line_width (m_border_width);
                Gdk::Cairo::set_source_color (cr, get_style ()->get_fg (get_state ()));
                cr->stroke ();
            }
            else
            {
                cr->fill ();
            }
        }

        void render_checks (Cairo::RefPtr<Cairo::Context>& cr, double x, double y, double w, double h)
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

        void on_drag_data_get(const Glib::RefPtr<Gdk::DragContext>& context,
                Gtk::SelectionData& selection_data, guint info, guint time)
        {
            Color c = m_model->get_color ();
            if (selection_data.get_target () == "application/x-color")
            {
                /* type: application/x-color
                 * format::16
                 * data[0]: red
                 * data[1]: green
                 * data[2]: blue
                 * data[3]: opacity
                 */
                guint16 color[4];
                guint16 max = std::numeric_limits<guint16>::max ();
                color[0] = static_cast<guint16>(c.get_red() *
                        static_cast<double>(max));
                color[1] = static_cast<guint16>(c.get_green()
                        * static_cast<double>(max));
                color[2] = static_cast<guint16>(c.get_blue()
                        * static_cast<double>(max));
                color[3] = max;
                selection_data.set(selection_data.get_target (), 16,
                        reinterpret_cast<const guchar*>(&color), sizeof(color));
            }
            else if (selection_data.targets_include_text ())
            {
                selection_data.set_text(c.as_hexstring ());
            }
        }

        void set_color_icon(const Glib::RefPtr<Gdk::DragContext>& context)
        {
            using std::numeric_limits;

            const int bits_per_sample = 8;
            const int w = 32;
            const int h = 32;
            Glib::RefPtr<Gdk::Pixbuf> pixbuf =
                Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, bits_per_sample,
                        w, h);
            Color c = m_model->get_color ();
            guint32 pixel = 0;

            // populate the 'pixel' value from the red, green, and blue color
            // values
            *(reinterpret_cast<guchar*>(&pixel) + RED_BYTE_POS) =
                (guchar) (c.get_red () * static_cast<double>(numeric_limits<guchar>::max()));

            *(reinterpret_cast<guchar*>(&pixel) + GREEN_BYTE_POS) =
                (guchar) (c.get_green () * static_cast<double>(numeric_limits<guchar>::max()));

            *(reinterpret_cast<guchar*>(&pixel) + BLUE_BYTE_POS) =
                (guchar) (c.get_blue () * static_cast<double>(numeric_limits<guchar>::max()));

            pixbuf->fill(pixel);
            drag_source_set_icon(pixbuf);
        }

        void request_size ()
        {
            int sz = MIN_SIZE + 2 * m_padding;
            if (m_border_width > 0.0)
            {
                sz += static_cast<int>(2.0 * m_border_width);
            }
            set_size_request (sz, sz);
        }

        void set_border_width (double width)
        {
            m_border_width = width;
            // redraw in case it changed
            queue_draw ();
        }

        void set_padding (int padding)
        {
            m_padding = padding;
            // redraw in case it changed
            queue_draw ();
        }

    };

    Swatch::Swatch (const boost::shared_ptr<ColorModel>& model) :
        m_priv (new Priv(model))
    {
    }

    void Swatch::set_model (const boost::shared_ptr<ColorModel>& model)
    {
        THROW_IF_FAIL (m_priv);
        m_priv->set_model (model);
    }

    boost::shared_ptr<ColorModel> Swatch::get_model ()
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_model;
    }

    void Swatch::set_border_width (double width)
    {
        THROW_IF_FAIL (m_priv);
        m_priv->set_border_width (width);
    }

    void Swatch::set_padding (int padding)
    {
        THROW_IF_FAIL (m_priv);
        m_priv->set_padding (padding);
    }

    Gtk::Widget& Swatch::get_widget () const
    {
        THROW_IF_FAIL (m_priv);
        return *m_priv;
    }
}
