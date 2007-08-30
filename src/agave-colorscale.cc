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
#include "agave-colorscale.h"
#include <gdkmm/general.h>  // cairo integration
#include <gdkmm/cursor.h>  // cairo integration
#include <cairomm/surface.h>

namespace agave
{
    const double border_width = 2.0;
    const double padding = 2.0;

    ColorScale::ColorScale (channel_t channel) :
        m_channel (channel),
        m_adj (0.0 /* initial value */,
                0.0 /* lower */,
                1.0 /* upper */,
                0.05 /* step increment */,
                0.1 /* page increment */)
    {
        init ();
        set_model (ColorModel::pointer (new ColorModel ()));
    }

    ColorScale::ColorScale (ColorModel::pointer model, channel_t channel) :
        m_channel (channel),
        m_adj (0.0 /* initial value */,
                0.0 /* lower */,
                1.0 /* upper */,
                0.05 /* step increment */,
                0.1 /* page increment */)
    {
        init ();
        set_model (model);
    }

    void ColorScale::init ()
    {
        m_adj.signal_value_changed ().connect (sigc::mem_fun (this,
                    &ColorScale::on_adjustment_value_changed));
        add_events (Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK |
                Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_MOTION_MASK |
                Gdk::ENTER_NOTIFY_MASK | Gdk::LEAVE_NOTIFY_MASK |
                Gdk::FOCUS_CHANGE_MASK);
    }

    double ColorScale::inside_x () const
    {
        return padding + border_width;
    }

    double ColorScale::inside_y () const
    {
        return padding + border_width;
    }

    double ColorScale::inside_width () const
    {
        return get_allocation ().get_width () - 2 * padding - 2 * border_width;
    }

    double ColorScale::inside_height () const
    {
        return get_allocation ().get_height () - 2 * padding - 2 * border_width;
    }

    double ColorScale::outside_x () const
    {
        return padding;
    }

    double ColorScale::outside_y () const
    {
        return padding;
    }

    double ColorScale::outside_width () const
    {
        return get_allocation ().get_width () - 2 * padding;
    }

    double ColorScale::outside_height () const
    {
        return get_allocation ().get_height () - 2 * padding;
    }

    void
    ColorScale::on_color_changed ()
    {
        if (m_model)
        {
            update_adjustment (m_model->get_color ());
        }
    }

    void
    ColorScale::on_adjustment_value_changed ()
    {
        g_return_if_fail (m_model);
        switch (m_channel)
        {
            case CHANNEL_HUE:
                // FIXME: implement
                break;
            case CHANNEL_SATURATION:
                // FIXME: implement
                break;
            case CHANNEL_VALUE:
                // FIXME: implement
                break;
            case CHANNEL_RED:
                {
                    Gdk::Color c;
                    c.set_rgb_p ( m_adj.get_value (),
                            m_model->get_color ().get_green_p (),
                            m_model->get_color ().get_blue_p ());
                    m_model->set_color (c);
                }
                break;
            case CHANNEL_GREEN:
                {
                    Gdk::Color c;
                    c.set_rgb_p (m_model->get_color ().get_red_p (),
                            m_adj.get_value (),
                            m_model->get_color ().get_blue_p ());
                    m_model->set_color (c);
                }
                break;
            case CHANNEL_BLUE:
                {
                    Gdk::Color c;
                    c.set_rgb_p (m_model->get_color ().get_red_p (),
                            m_model->get_color ().get_green_p (),
                            m_adj.get_value ());
                    m_model->set_color (c);
                }
                break;
            case CHANNEL_ALPHA:
                // FIXME: implement
                break;
            default:
                // NOTHING
                break;
        }
        queue_draw ();
    }

    std::ostream& operator<< (std::ostream& out, const Gdk::Rectangle& rect)
    {
        return out << "(" << rect.get_x () << "," << rect.get_y ()
            << ") " << rect.get_width () << "x" << rect.get_height ();
    }

    bool
    ColorScale::on_motion_notify_event (GdkEventMotion* event)
    {
        if (event && event->state & GDK_BUTTON1_MASK)
        {
            // dragging with mouse button pressed
            m_adj.set_value (get_value_from_coords (event->x, event->y));
        }
        return true;
    }

    bool
    ColorScale::on_button_press_event (GdkEventButton* event)
    {
        if (event && event->type == GDK_BUTTON_PRESS)
        {
            switch (event->button)
            {
                case 1:
                    m_adj.set_value (get_value_from_coords (event->x, event->y));
                    break;
                case 3:
                    {
                        double val = get_value_from_coords (event->x, event->y);
                        if (val < m_adj.get_value ())
                        {
                            m_adj.set_value (m_adj.get_value () - m_adj.get_page_increment ());
                        }
                        else if (val > m_adj.get_value ())
                        {
                            m_adj.set_value (m_adj.get_value () + m_adj.get_page_increment ());
                        }
                    }
                    break;
                default:
                    // do nothing
                    break;
            }
        }
        return true;
    }

    bool
    ColorScale::on_scroll_event (GdkEventScroll* event)
    {
        if (event)
        {
            switch (event->direction)
            {
                case GDK_SCROLL_UP:
                    m_adj.set_value (m_adj.get_value () + m_adj.get_page_increment ());
                    break;
                case GDK_SCROLL_DOWN:
                    m_adj.set_value (m_adj.get_value () - m_adj.get_page_increment ());
                    break;
                default:
                    // do nothing
                    break;
            }
        }
        return true;
    }

    double
    ColorScale::get_value_from_coords (double x, double y)
    {
            double new_val = (x - inside_x ()) / inside_width ();
            //CLAMP (new_val, 0.0, 1.0);
            return new_val;
    }

    bool ColorScale::on_enter_notify_event (GdkEventCrossing* event)
    {
        Glib::RefPtr<Gdk::Window> win = get_window();
        if (win)
        {
            Glib::RefPtr<Gdk::Display> dpy = Gdk::Display::get_default();
            Gdk::Cursor cursor(dpy, Gdk::HAND2);
            win->set_cursor(cursor);
        }
        return true;
    }

    bool
    ColorScale::on_focus_in_event (GdkEventFocus* event)
    {
        set_state (Gtk::STATE_ACTIVE);
        return true;
    }

    bool
    ColorScale::on_focus_out_event (GdkEventFocus* event)
    {
        set_state (Gtk::STATE_NORMAL);
        return true;
    }

    bool ColorScale::on_leave_notify_event (GdkEventCrossing* event)
    {
            Glib::RefPtr<Gdk::Window> win = get_window();
            if (win)
            {
                // return cursor to default
                win->set_cursor();
            }
            return true;
    }


    bool
    ColorScale::on_expose_event (GdkEventExpose* event)
    {
        Cairo::RefPtr<Cairo::Context> cr = get_window ()->create_cairo_context ();
        cr->rectangle (event->area.x, event->area.y,
                event->area.width, event->area.height);
        cr->clip ();
        cr->set_line_width (border_width);
        render_scale (cr);
        render_selectors (cr);
        return true;
    }

    void
    ColorScale::render_scale (Cairo::RefPtr<Cairo::Context> &cr)
    {
        g_return_if_fail (m_model);
        cr->save ();

        // print some check marks in the background so that if there is any
        // alpha opacity, the check marks will show through
        render_checks (cr, inside_x (), inside_y (),
                inside_width (), inside_height ());

        cr->rectangle (outside_x () + border_width / 2.0,
                outside_y () + border_width / 2.0,
                outside_width () - border_width,
                outside_height () - border_width);
        Cairo::RefPtr<Cairo::LinearGradient> gradient =
            Cairo::LinearGradient::create (outside_x () + border_width,
                    0.0,
                    inside_x () + inside_width (),
                    0.0);
        // fill with correct stuff
        switch (m_channel)
        {
            case CHANNEL_HUE:
                // FIXME
                gradient->add_color_stop_rgb (0.0, 1.0, 0.0, 0.0);
                gradient->add_color_stop_rgb (0.5, 0.0, 1.0, 0.0);
                gradient->add_color_stop_rgb (1.0, 0.0, 0.0, 1.0);
                break;
            case CHANNEL_SATURATION:
                // FIXME
                gradient->add_color_stop_rgb (0.0, 1.0, 0.0, 0.0);
                gradient->add_color_stop_rgb (0.5, 0.0, 1.0, 0.0);
                gradient->add_color_stop_rgb (1.0, 0.0, 0.0, 1.0);
                break;
            case CHANNEL_VALUE:
                // FIXME
                gradient->add_color_stop_rgb (0.0, 1.0, 0.0, 0.0);
                gradient->add_color_stop_rgb (0.5, 0.0, 1.0, 0.0);
                gradient->add_color_stop_rgb (1.0, 0.0, 0.0, 1.0);
                break;
            case CHANNEL_RED:
                gradient->add_color_stop_rgb (0.0, 0.0, 0.0, 0.0);
                gradient->add_color_stop_rgb (1.0, 1.0, 0.0, 0.0);
                break;
            case CHANNEL_GREEN:
                gradient->add_color_stop_rgb (0.0, 0.0, 0.0, 0.0);
                gradient->add_color_stop_rgb (1.0, 0.0, 1.0, 0.0);
                break;
            case CHANNEL_BLUE:
                gradient->add_color_stop_rgb (0.0, 0.0, 0.0, 0.0);
                gradient->add_color_stop_rgb (1.0, 0.0, 0.0, 1.0);
                break;
            case CHANNEL_ALPHA:
                {
                    Gdk::Color c = m_model->get_color ();
                    gradient->add_color_stop_rgba (0.0, c.get_red_p (), c.get_green_p (), c.get_blue_p (), 0.0);
                    gradient->add_color_stop_rgba (1.0, c.get_red_p (), c.get_green_p (), c.get_blue_p (), 1.0);
                }
                break;
        }
        cr->set_source (gradient);
        cr->fill_preserve ();

        Gdk::Cairo::set_source_color (cr, get_style ()->get_fg (get_state ()));
        cr->stroke ();

        cr->restore ();
    }

    void
    ColorScale::render_selectors (Cairo::RefPtr<Cairo::Context>& cr)
    {
        cr->save ();
        cr->rectangle (outside_x (), outside_y (), outside_width (), outside_height ());
        cr->clip ();
        const double side_length = inside_height () / 4.0;
        double value_x = inside_x () + m_adj.get_value () * inside_width ();

        cr->move_to (value_x, inside_y ());
        cr->line_to (value_x + side_length, inside_y () + inside_height () / 2.0);
        cr->line_to (value_x, inside_y () + inside_height ());
        cr->line_to (value_x - side_length, inside_y () + inside_height () / 2.0);
        cr->close_path ();
        Gdk::Cairo::set_source_color (cr, get_style ()->get_bg (get_state ()));
        cr->fill_preserve ();
        Gdk::Cairo::set_source_color (cr, get_style ()->get_fg (get_state ()));
        cr->set_line_width (1.0);
        cr->stroke ();

        cr->restore ();
    }

    void ColorScale::render_checks (Cairo::RefPtr<Cairo::Context>& cr, double x, double y, double w, double h)
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

    void
    ColorScale::set_model (const ColorModel::pointer& model)
    {
        m_model = model;
        if (m_model)
        {
            m_model->signal_color_changed ().connect (sigc::mem_fun (this, &ColorScale::on_color_changed));
            update_adjustment (m_model->get_color ());
        }
    }

    void ColorScale::update_adjustment (const Gdk::Color& c)
    {
        switch (m_channel)
        {
            case CHANNEL_HUE:
                // FIXME
                break;
            case CHANNEL_SATURATION:
                // FIXME
                break;
            case CHANNEL_VALUE:
                // FIXME
                break;
            case CHANNEL_RED:
                m_adj.set_value (c.get_red_p ());
                break;
            case CHANNEL_GREEN:
                m_adj.set_value (c.get_green_p ());
                break;
            case CHANNEL_BLUE:
                m_adj.set_value (c.get_blue_p ());
                break;
            case CHANNEL_ALPHA:
            default:
                // do nothing
                break;
        }
        queue_draw ();
    }

    ColorModel::pointer ColorScale::get_model ()
    {
        return m_model;
    }

    void
    ColorScale::set_alpha (double new_alpha)
    {
        switch (m_channel)
        {
            case CHANNEL_ALPHA:
                g_return_if_fail (new_alpha >= 0.0 && new_alpha <= 1.0);
                m_adj.set_value (new_alpha);
                break;
            default:
                //do nothing
                break;
        }
    }
}
