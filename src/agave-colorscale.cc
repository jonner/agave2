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
#include <gdk/gdkkeysyms.h>

namespace agave
{
    const double border_width = 1.0;
    const double selector_size = 6.0;
    const double x_padding = 3.0 + selector_size;
    const double y_padding = 2.0;
    const double min_width = 4.0 * selector_size;
    const double min_height = 4.0 * selector_size;

    ColorScale::ColorScale (channel_t channel) :
        m_channel (channel)
    {
        init ();
        set_model (ColorModel::pointer (new ColorModel ()));
    }

    ColorScale::ColorScale (ColorModel::pointer model, channel_t channel) :
        m_channel (channel)
    {
        init ();
        set_model (model);
    }

    void ColorScale::init ()
    {
        add_events (Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK |
                Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_MOTION_MASK |
                Gdk::ENTER_NOTIFY_MASK | Gdk::LEAVE_NOTIFY_MASK |
                Gdk::KEY_PRESS_MASK | Gdk::FOCUS_CHANGE_MASK);
        set_size_request (static_cast<int>(2.0 * x_padding + min_width),
                static_cast<int>(2.0 * y_padding + min_height));
        m_adj.reset (new Gtk::Adjustment (0.0 /* initial value */,
                    0.0 /* lower */,
                    1.0 /* upper */,
                    0.05 /* step increment */,
                    0.1 /* page increment */));
        m_adj->signal_value_changed ().connect (sigc::mem_fun (this,
                    &ColorScale::on_adjustment_value_changed));
    }

    double ColorScale::inside_x () const
    {
        return x_padding + border_width;
    }

    double ColorScale::inside_y () const
    {
        return y_padding + border_width;
    }

    double ColorScale::inside_width () const
    {
        return get_allocation ().get_width () - 2 * x_padding - 2 * border_width;
    }

    double ColorScale::inside_height () const
    {
        return get_allocation ().get_height () - 2 * y_padding - 2 * border_width;
    }

    double ColorScale::outside_x () const
    {
        return x_padding;
    }

    double ColorScale::outside_y () const
    {
        return y_padding;
    }

    double ColorScale::outside_width () const
    {
        return get_allocation ().get_width () - 2 * x_padding;
    }

    double ColorScale::outside_height () const
    {
        return get_allocation ().get_height () - 2 * y_padding;
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
                m_model->get_color ().set_hue (m_adj->get_value ());
                break;
            case CHANNEL_SATURATION:
                m_model->get_color ().set_saturation (m_adj->get_value ());
                break;
            case CHANNEL_VALUE:
                m_model->get_color ().set_value (m_adj->get_value ());
                break;
            case CHANNEL_RED:
                m_model->get_color ().set_red (m_adj->get_value ());
                break;
            case CHANNEL_GREEN:
                m_model->get_color ().set_green (m_adj->get_value ());
                break;
            case CHANNEL_BLUE:
                m_model->get_color ().set_blue (m_adj->get_value ());
                break;
            case CHANNEL_ALPHA:
                m_model->get_color ().set_alpha (m_adj->get_value ());
                break;
            default:
                // NOTHING
                break;
        }
        queue_draw ();
    }

    bool
    ColorScale::on_motion_notify_event (GdkEventMotion* event)
    {
        if (get_state () != Gtk::STATE_INSENSITIVE)
        {
            if (event && event->state & GDK_BUTTON1_MASK)
            {
                // dragging with mouse button pressed
                m_adj->set_value (get_value_from_coords (event->x, event->y));
            }
        }
        return true;
    }

    bool
    ColorScale::on_button_press_event (GdkEventButton* event)
    {
        if (get_state () != Gtk::STATE_INSENSITIVE)
        {
            if (event && event->type == GDK_BUTTON_PRESS)
            {
                switch (event->button)
                {
                    case 1:
                        m_adj->set_value (get_value_from_coords (event->x, event->y));
                        break;
                    case 3:
                        {
                            double val = get_value_from_coords (event->x, event->y);
                            if (val < m_adj->get_value ())
                            {
                                decrement_page ();
                            }
                            else if (val > m_adj->get_value ())
                            {
                                increment_page ();
                            }
                        }
                        break;
                    default:
                        // do nothing
                        break;
                }
            }
        }
        return true;
    }
    void ColorScale::increment_page ()
    {
        m_adj->set_value (m_adj->get_value () + m_adj->get_page_increment ());
    }

    void ColorScale::decrement_page ()
    {
        m_adj->set_value (m_adj->get_value () - m_adj->get_page_increment ());
    }

    bool
    ColorScale::on_scroll_event (GdkEventScroll* event)
    {
        if (get_state () != Gtk::STATE_INSENSITIVE)
        {
            if (event)
            {
                switch (event->direction)
                {
                    case GDK_SCROLL_UP:
                        m_adj->set_value (m_adj->get_value () + m_adj->get_page_increment ());
                        break;
                    case GDK_SCROLL_DOWN:
                        m_adj->set_value (m_adj->get_value () - m_adj->get_page_increment ());
                        break;
                    default:
                        // do nothing
                        break;
                }
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
        if (get_state () != Gtk::STATE_INSENSITIVE)
        {
            Glib::RefPtr<Gdk::Window> win = get_window();
            if (win)
            {
                Glib::RefPtr<Gdk::Display> dpy = Gdk::Display::get_default();
                Gdk::Cursor cursor(dpy, Gdk::HAND2);
                win->set_cursor(cursor);
            }
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
        if (get_state () != Gtk::STATE_INSENSITIVE)
        {
            Glib::RefPtr<Gdk::Window> win = get_window();
            if (win)
            {
                // return cursor to default
                win->set_cursor();
            }
        }
        return true;
    }

    bool ColorScale::on_key_press_event (GdkEventKey* event)
    {
        if (event)
        {
            switch (event->keyval)
            {
                case GDK_Up:
                case GDK_Left:
                    increment_page ();
                    break;
                case GDK_Down:
                case GDK_Right:
                    decrement_page ();
                default:
                    // nothing
                    break;
            }
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
        if (m_draw_value)
        {
            // FIXME: draw the current value on the widget
        }
        return true;
    }

    void
    ColorScale::render_scale (Cairo::RefPtr<Cairo::Context> &cr)
    {
        g_return_if_fail (m_model);
        cr->save ();
        double x, y, w, h;
        x = outside_x () + border_width / 2.0;
        y = outside_y () + border_width / 2.0;
        w = outside_width () - border_width;
        h = outside_height () - border_width;

        // print some check marks in the background so that if there is any
        // alpha opacity, the check marks will show through
        render_checks (cr, inside_x (), inside_y (),
                inside_width (), inside_height ());

        Cairo::RefPtr<Cairo::Pattern> pattern;
        // fill with correct stuff
        switch (m_channel)
        {
            case CHANNEL_HUE:
                {
                    Cairo::RefPtr<Cairo::ImageSurface> surface =
                        Cairo::ImageSurface::create (Cairo::FORMAT_ARGB32,
                                static_cast<int>(w), static_cast<int>(h));
                    unsigned char *data = surface->get_data ();
                    g_return_if_fail (data);

                    for (int row = 0; row < surface->get_height (); ++row)
                    {
                        for (int px = 0; px < surface->get_width (); ++px)
                        {
                            hsv_t hsv;
                            hsv.h = static_cast<double>(px) /
                                static_cast<double>(surface->get_width ());
                            hsv.s = 1.0;
                            hsv.v = 1.0;
                            Color c (hsv);
                            *data++ =
                                static_cast<unsigned char>(
                                        c.get_blue () * static_cast<double>(std::numeric_limits<unsigned char>::max ()));
                            *data++ =
                                static_cast<unsigned char>(
                                        c.get_green () * static_cast<double>(std::numeric_limits<unsigned char>::max ()));
                            *data++ =
                                static_cast<unsigned char>(
                                        c.get_red () * static_cast<double>(std::numeric_limits<unsigned char>::max ()));
                            *data++ = std::numeric_limits<unsigned char>::max ();
                        }
                    }
                    surface->flush ();
                    pattern = Cairo::SurfacePattern::create (surface);
                }
                break;
            case CHANNEL_SATURATION:
                {
                    Cairo::RefPtr<Cairo::LinearGradient> gradient =
                        Cairo::LinearGradient::create (0.0, 0.0, w, 0.0);
                    Color low = m_model->get_color ();
                    low.set_saturation (0.0);     // min
                    gradient->add_color_stop_rgb (0.0, low.get_red (), low.get_green (), low.get_blue ());
                    Color high = m_model->get_color ();
                    high.set_saturation (1.0);     // max
                    gradient->add_color_stop_rgb (1.0, high.get_red (), high.get_green (), high.get_blue ());
                    pattern = gradient;
                }
                break;
            case CHANNEL_VALUE:
                {
                    Cairo::RefPtr<Cairo::LinearGradient> gradient =
                        Cairo::LinearGradient::create (0.0, 0.0, w, 0.0);
                    Color low = m_model->get_color ();
                    low.set_value (0.0);  // min
                    gradient->add_color_stop_rgb (0.0, low.get_red (), low.get_green (), low.get_blue ());
                    Color high = m_model->get_color ();
                    high.set_value (1.0);  // max
                    gradient->add_color_stop_rgb (1.0, high.get_red (), high.get_green (), high.get_blue ());
                    pattern = gradient;
                }
                break;
            case CHANNEL_RED:
                {
                    Cairo::RefPtr<Cairo::LinearGradient> gradient =
                        Cairo::LinearGradient::create (0.0, 0.0, w, 0.0);
                    gradient->add_color_stop_rgb (0.0, 0.0, 0.0, 0.0);
                    gradient->add_color_stop_rgb (1.0, 1.0, 0.0, 0.0);
                    pattern = gradient;
                }
                break;
            case CHANNEL_GREEN:
                {
                    Cairo::RefPtr<Cairo::LinearGradient> gradient =
                        Cairo::LinearGradient::create (0.0, 0.0, w, 0.0);
                    gradient->add_color_stop_rgb (0.0, 0.0, 0.0, 0.0);
                    gradient->add_color_stop_rgb (1.0, 0.0, 1.0, 0.0);
                    pattern = gradient;
                }
                break;
            case CHANNEL_BLUE:
                {
                    Cairo::RefPtr<Cairo::LinearGradient> gradient =
                        Cairo::LinearGradient::create (0.0, 0.0, w, 0.0);
                    gradient->add_color_stop_rgb (0.0, 0.0, 0.0, 0.0);
                    gradient->add_color_stop_rgb (1.0, 0.0, 0.0, 1.0);
                    pattern = gradient;
                }
                break;
            case CHANNEL_ALPHA:
                {
                    Cairo::RefPtr<Cairo::LinearGradient> gradient =
                        Cairo::LinearGradient::create (0.0, 0.0, w, 0.0);
                    Color c = m_model->get_color ();
                    rgb_t rgb = c.as_rgb ();
                    gradient->add_color_stop_rgba (0.0, rgb.r, rgb.g, rgb.b, 0.0);
                    gradient->add_color_stop_rgba (1.0, rgb.r, rgb.g, rgb.b, 1.0);
                    pattern = gradient;
                }
                break;
        }
        cr->rectangle (x, y, w, h);

        // this translation is necessary to get the surface pattern (for the hue
        // selector) to paint inside the borders correctly
        cr->save ();
        cr->translate (x, y);
        cr->set_source (pattern);
        cr->fill();
        cr->restore ();

        if (get_state () == Gtk::STATE_INSENSITIVE)
        {
            render_stipple (cr, inside_x (), inside_y (),
                    inside_width (), inside_height ());
        }

        Gdk::Cairo::set_source_color (cr, get_style ()->get_fg (get_state ()));
        cr->rectangle (x, y, w, h);
        cr->stroke ();

        cr->restore ();
    }

    void
    ColorScale::render_selectors (Cairo::RefPtr<Cairo::Context>& cr)
    {
        cr->save ();
        double value_x = inside_x () + m_adj->get_value () * inside_width ();
        double mid_y = get_allocation ().get_height () / 2.0;

        cr->move_to (value_x, mid_y + 2.0 * selector_size);
        cr->line_to (value_x + selector_size, mid_y);
        cr->line_to (value_x, mid_y - 2.0 * selector_size);
        cr->line_to (value_x - selector_size, mid_y);
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

    void ColorScale::render_stipple (Cairo::RefPtr<Cairo::Context>& cr, double x, double y, double w, double h)
    {
        const double CHECK_SIZE = 1.0;
        cr->save ();

        Cairo::RefPtr<Cairo::Surface> stipple = Cairo::Surface::create (cr->get_target (),
                Cairo::CONTENT_COLOR_ALPHA,
                static_cast<int>(2 * CHECK_SIZE), static_cast<int>(2 * CHECK_SIZE));

        /* Draw a translucent stipple overlay */
        {
            Cairo::RefPtr<Cairo::Context> cr2 = Cairo::Context::create (stipple);

            cr2->set_operator (Cairo::OPERATOR_SOURCE);

            Gdk::Cairo::set_source_color (cr2, get_style ()->get_bg (Gtk::STATE_INSENSITIVE));

            Gdk::Cairo::set_source_color (cr2, get_style ()->get_bg (Gtk::STATE_INSENSITIVE));

            // draw on the top-right and bottom-left
            cr2->rectangle (CHECK_SIZE, 0.0, CHECK_SIZE, CHECK_SIZE);
            cr2->rectangle (0.0, CHECK_SIZE, CHECK_SIZE, CHECK_SIZE);
            cr2->fill ();
        }

        /* Fill the whole surface with the stipple */
        Cairo::RefPtr<Cairo::SurfacePattern> check_pattern = Cairo::SurfacePattern::create (stipple);
        check_pattern->set_extend (Cairo::EXTEND_REPEAT);
        cr->set_source (check_pattern);
        cr->rectangle (x, y, w, h);
        cr->paint_with_alpha (0.5);

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

    void ColorScale::update_adjustment (const Color& c)
    {
        switch (m_channel)
        {
            case CHANNEL_HUE:
                m_adj->set_value (c.as_hsv ().h);
                break;
            case CHANNEL_SATURATION:
                m_adj->set_value (c.as_hsv ().s);
                break;
            case CHANNEL_VALUE:
                m_adj->set_value (c.as_hsv ().v);
                break;
            case CHANNEL_RED:
                m_adj->set_value (c.as_rgb ().r);
                break;
            case CHANNEL_GREEN:
                m_adj->set_value (c.as_rgb ().g);
                break;
            case CHANNEL_BLUE:
                m_adj->set_value (c.as_rgb ().b);
                break;
            case CHANNEL_ALPHA:
                m_adj->set_value (c.as_rgb ().a);
                break;
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
                m_adj->set_value (new_alpha);
                break;
            default:
                //do nothing
                break;
        }
    }

    void ColorScale::set_draw_value (bool enable)
    {
        m_draw_value = enable;
        queue_draw ();
    }

    bool ColorScale::get_draw_value ()
    {
        return m_draw_value;
    }

}
