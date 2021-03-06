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
#include "color-scale.h"
#include <gdkmm/general.h>  // cairo integration
#include <cairomm/surface.h>
#include <gdk/gdkkeysyms.h>
#include <gtkmm/tooltip.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/adjustment.h>
#include <glibmm-utils/log-stream-utils.h>
#include <boost/format.hpp>
#include <glibmm-utils/exception.h>
#include "color-model.h"

namespace agave
{
    const double border_width = 1.0;
    const double selector_size = 6.0;
    const double x_padding = 3.0 + selector_size;
    const double y_padding = 2.0;
    const double min_width = 8.0 * selector_size;
    const double min_height = 4.0 * selector_size;

    struct ColorScale::Priv : public Gtk::DrawingArea
    {
        boost::shared_ptr<ColorModel> m_model;
        const channel_t m_channel;
        boost::shared_ptr<Gtk::Adjustment> m_adj;
        Color last_color;
        bool m_draw_value;
        bool m_drag_started;
        Cairo::RefPtr<Cairo::ImageSurface> m_hue_surface;
        Glib::RefPtr<Pango::Layout> m_text_layout;
        mutable sigc::connection m_color_signal_connection;
        mutable sigc::connection m_adjustment_signal_connection;

        Priv (const boost::shared_ptr<ColorModel>& model, channel_t channel) :
            m_channel (channel)
        {
            init ();
            set_model (model);
            property_can_focus () = true;
        }

        void set_model (const boost::shared_ptr<ColorModel>& model)
        {
            m_model = model;
            if (m_model)
            {
                m_color_signal_connection = m_model->signal_color_changed ().connect
                    (sigc::mem_fun (this, &Priv::on_color_changed));
                update_adjustment (m_model->get_color ());
            }
        }

        void set_draw_label (bool enable)
        {
            if (enable)
            {
                switch (m_channel)
                {
                    case CHANNEL_HUE:
                        m_text_layout = create_pango_layout ("H");
                        break;
                    case CHANNEL_SATURATION:
                        m_text_layout = create_pango_layout ("S");
                        break;
                    case CHANNEL_VALUE:
                        m_text_layout = create_pango_layout ("V");
                        break;
                    case CHANNEL_RED:
                        m_text_layout = create_pango_layout ("R");
                        break;
                    case CHANNEL_GREEN:
                        m_text_layout = create_pango_layout ("G");
                        break;
                    case CHANNEL_BLUE:
                        m_text_layout = create_pango_layout ("B");
                        break;
                    case CHANNEL_ALPHA:
                        m_text_layout = create_pango_layout ("A");
                        break;
                    default:
                        LOG_ERROR ("Invalid Channel type");
                        break;
                }
            }
            else
            {
                m_text_layout.clear ();
            }
            queue_draw ();
        }


        virtual bool on_expose_event (GdkEventExpose* event)
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

            if (has_focus ())
            {
                // FIXME: this doesn't paint quite right...
                gtk_paint_focus (get_style ()->gobj (),
                                 get_window ()->gobj (),
                                 static_cast<GtkStateType> (get_state ()),
                                 0, /* no clip */
                                 GTK_WIDGET (gobj ()),  /* this widget */
                                 0, /* no style detail */
                                 1,
                                 1,
                                 get_width () - 2,
                                 get_height () - 2);
            }
            return true;
        }

        virtual bool on_motion_notify_event (GdkEventMotion* event)
        {
            if (get_state () != Gtk::STATE_INSENSITIVE)
            {
                if (event
                        && (event->state & GDK_BUTTON1_MASK)
                        && m_drag_started)
                {
                    // dragging with mouse button pressed
                    m_adj->set_value (get_value_from_coords (event->x, event->y));
                }
            }
            return true;
        }

        virtual bool on_button_press_event (GdkEventButton* event)
        {
            grab_focus ();
            if (get_state () != Gtk::STATE_INSENSITIVE)
            {
                if (event
                        && is_inside_scale (event->x, event->y)
                        && event->type == GDK_BUTTON_PRESS)
                {
                    switch (event->button)
                    {
                        case 1:
                            m_drag_started = true;
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

        virtual bool on_button_release_event (GdkEventButton* event)
        {
            m_drag_started = false;
            return true;
        }

        virtual bool on_scroll_event (GdkEventScroll* event)
        {
            grab_focus ();
            if (get_state () != Gtk::STATE_INSENSITIVE)
            {
                if (event)
                {
                    switch (event->direction)
                    {
                        case GDK_SCROLL_UP:
                            increment_page ();
                            break;
                        case GDK_SCROLL_DOWN:
                            decrement_page ();
                            break;
                        default:
                            // do nothing
                            break;
                    }
                }
            }
            return true;
        }

        virtual bool on_key_press_event (GdkEventKey* event)
        {
            if (event)
            {
                switch (event->keyval)
                {
                    case GDK_Down:
                    case GDK_Left:
                        decrement_page ();
                        break;
                    case GDK_Up:
                    case GDK_Right:
                        increment_page ();
                        break;
                    default:
                        return false;
                }
            }
            return true;
        }

        bool on_query_tooltip (int x, int y, bool keyboard_tooltip,
                const Glib::RefPtr<Gtk::Tooltip>& tooltip)
        {
            Gdk::Rectangle area (static_cast<int>(outside_x ()),
                    static_cast<int>(outside_y ()),
                    static_cast<int>(outside_width ()),
                    static_cast<int>(outside_height ()));
            tooltip->set_tip_area (area);
            boost::format tip_format("%1$.2f");
            tip_format % m_adj->get_value ();
            Glib::ustring tooltip_text = tip_format.str ();
            tooltip->set_text (tooltip_text);
            return true;
        }

        void render_scale (Cairo::RefPtr<Cairo::Context>& cr)
        {
            g_return_if_fail (m_model);
            double x, y, w, h;
            cr->save ();
            if (m_text_layout)
            {
                // draw the layout
                Pango::Rectangle extents = m_text_layout->get_pixel_logical_extents ();
                cr->move_to (0.0,
                        (static_cast<float>(get_allocation ().get_height ()) / 2.0) - (extents.get_height () / 2.0));
                m_text_layout->add_to_cairo_context (cr);
                cr->fill ();

                LOG_D("Layout width: " << extents.get_width (), "pango");
            }
            y = outside_y () + border_width / 2.0;
            x = outside_x () + border_width / 2.0;
            w = outside_width () - border_width;
            h = outside_height () - border_width;


            // print some check marks in the background so that if there is any
            // alpha opacity, the check marks will show through
            render_checks (cr, x, y,
                    w, h);

            Cairo::RefPtr<Cairo::Pattern> pattern;
            // fill with correct stuff
            switch (m_channel)
            {
                case CHANNEL_HUE:
                    {
                        LOG_DD ("rendering HUE scale");
                        // first check if we already have a cached hue surface of
                        // the right size
                        if (!(m_hue_surface
                                    && m_hue_surface->get_width () == w
                                    && m_hue_surface->get_height () == h))
                        {
                            LOG_DD ("Creating HUE surface -- no valid cache");
                            m_hue_surface =
                                Cairo::ImageSurface::create (Cairo::FORMAT_ARGB32,
                                        static_cast<int>(w), static_cast<int>(h));
                            unsigned char *data = m_hue_surface->get_data ();
                            g_return_if_fail (data);

                            for (int row = 0; row < m_hue_surface->get_height (); ++row)
                            {
                                for (int px = 0; px < m_hue_surface->get_width (); ++px)
                                {
                                    hsv_t hsv;
                                    hsv.h = static_cast<double>(px) /
                                        static_cast<double>(m_hue_surface->get_width ());
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
                            m_hue_surface->flush ();
                        }
                        pattern = Cairo::SurfacePattern::create (m_hue_surface);
                    }
                    break;
                case CHANNEL_SATURATION:
                    {
                        LOG_DD ("rendering SATURATION scale");
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
                        LOG_DD ("rendering VALUE scale");
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
                        LOG_DD ("rendering RED scale");
                        Cairo::RefPtr<Cairo::ImageSurface> red_surface =
                            Cairo::ImageSurface::create (Cairo::FORMAT_ARGB32,
                                    static_cast<int>(w), static_cast<int>(h));
                        Cairo::RefPtr<Cairo::LinearGradient> gradient =
                            Cairo::LinearGradient::create (0.0, 0.0, w, 0.0);
                        Color c = m_model->get_color ();
                        gradient->add_color_stop_rgb (0.0, 0.0, c.get_green (), c.get_blue ());
                        gradient->add_color_stop_rgb (1.0, 1.0, c.get_green (), c.get_blue ());
                        Cairo::RefPtr<Cairo::Context> cr2 =
                            Cairo::Context::create (red_surface);
                        cr2->set_source (gradient);
                        cr2->paint ();
                        gradient = Cairo::LinearGradient::create (0.0, 0.0, w, 0.0);
                        gradient->add_color_stop_rgb (0.0, 0.0, 0.0, 0.0);
                        gradient->add_color_stop_rgb (1.0, 1.0, 0.0, 0.0);
                        cr2->rectangle (0.0, 0.0, w, h / 4.0);
                        cr2->set_source (gradient);
                        cr2->fill ();
                        pattern = Cairo::SurfacePattern::create (red_surface);
                    }
                    break;
                case CHANNEL_GREEN:
                    {
                        LOG_DD ("rendering GREEN scale");
                        Cairo::RefPtr<Cairo::ImageSurface> green_surface =
                            Cairo::ImageSurface::create (Cairo::FORMAT_ARGB32,
                                    static_cast<int>(w), static_cast<int>(h));
                        Cairo::RefPtr<Cairo::LinearGradient> gradient =
                            Cairo::LinearGradient::create (0.0, 0.0, w, 0.0);
                        Color c = m_model->get_color ();
                        gradient->add_color_stop_rgb (0.0, c.get_red (), 0.0, c.get_blue ());
                        gradient->add_color_stop_rgb (1.0, c.get_red (), 1.0, c.get_blue ());
                        Cairo::RefPtr<Cairo::Context> cr2 =
                            Cairo::Context::create (green_surface);
                        cr2->set_source (gradient);
                        cr2->paint ();
                        gradient = Cairo::LinearGradient::create (0.0, 0.0, w, 0.0);
                        gradient->add_color_stop_rgb (0.0, 0.0, 0.0, 0.0);
                        gradient->add_color_stop_rgb (1.0, 0.0, 1.0, 0.0);
                        cr2->rectangle (0.0, 0.0, w, h / 4.0);
                        cr2->set_source (gradient);
                        cr2->fill ();
                        pattern = Cairo::SurfacePattern::create (green_surface);
                    }
                    break;
                case CHANNEL_BLUE:
                    {
                        LOG_DD ("rendering BLUE scale");
                        Cairo::RefPtr<Cairo::ImageSurface> blue_surface =
                            Cairo::ImageSurface::create (Cairo::FORMAT_ARGB32,
                                    static_cast<int>(w), static_cast<int>(h));
                        Cairo::RefPtr<Cairo::LinearGradient> gradient =
                            Cairo::LinearGradient::create (0.0, 0.0, w, 0.0);
                        Color c = m_model->get_color ();
                        gradient->add_color_stop_rgb (0.0, c.get_red (), c.get_green (), 0.0);
                        gradient->add_color_stop_rgb (1.0, c.get_red (), c.get_green (), 1.0);
                        Cairo::RefPtr<Cairo::Context> cr2 =
                            Cairo::Context::create (blue_surface);
                        cr2->set_source (gradient);
                        cr2->paint ();
                        gradient = Cairo::LinearGradient::create (0.0, 0.0, w, 0.0);
                        gradient->add_color_stop_rgb (0.0, 0.0, 0.0, 0.0);
                        gradient->add_color_stop_rgb (1.0, 0.0, 0.0, 1.0);
                        cr2->rectangle (0.0, 0.0, w, h / 4.0);
                        cr2->set_source (gradient);
                        cr2->fill ();
                        pattern = Cairo::SurfacePattern::create (blue_surface);
                    }
                    break;
                case CHANNEL_ALPHA:
                    {
                        LOG_DD ("rendering ALPHA scale");
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

        void render_selectors (Cairo::RefPtr<Cairo::Context>& cr)
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

        void render_checks (Cairo::RefPtr<Cairo::Context>& cr,
                double x, double y, double w, double h)
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

        void render_stipple (Cairo::RefPtr<Cairo::Context>& cr,
                double x, double y, double w, double h)
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

        void on_adjustment_value_changed ()
        {
            g_return_if_fail (m_model);
            m_color_signal_connection.block ();
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
            m_color_signal_connection.unblock ();
        }

        void on_color_changed ()
        {
            if (m_model)
            {
                m_adjustment_signal_connection.block ();
                update_adjustment (m_model->get_color ());
                m_adjustment_signal_connection.unblock ();

                // only queue a redraw if the color has been changed since last
                // update so that we don't do any unnecessary redraws
                if (m_model->get_color () != last_color)
                {
                    queue_draw ();
                    last_color = m_model->get_color ();
                }
            }
        }

        void update_adjustment (const Color& c)
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
        }

        double inside_x () const
        {
            return outside_x () + border_width;
        }

        double inside_y () const
        {
            return outside_y () + border_width;
        }

        double inside_width () const
        {
            return outside_width () - 2.0 * border_width;
        }

        double inside_height () const
        {
            return outside_height () - 2.0 * border_width;
        }

        double outside_x () const
        {
            double x = x_padding;
            if (m_text_layout)
            {
                x += m_text_layout->get_pixel_logical_extents ().get_width ();
            }
            return x;
        }

        double outside_y () const
        {
            return y_padding;
        }

        double outside_width () const
        {
            double x = get_allocation ().get_width () - 2.0 * x_padding;
            if (m_text_layout)
            {
                x -= m_text_layout->get_pixel_logical_extents ().get_width ();
            }
            return x;
        }

        double outside_height () const
        {
            return get_allocation ().get_height () - 2.0 * y_padding;
        }

        bool is_inside_scale (double x, double y) const
        {
            return (x > outside_x () && x < (outside_x () + outside_width ()) &&
                    y > outside_y () && y < (outside_y () + outside_height ()));
        }

        double get_value_from_coords (double x, double y)
        {
            double new_val = (x - inside_x ()) / inside_width ();
            //CLAMP (new_val, 0.0, 1.0);
            return new_val;
        }

        void increment_page ()
        {
            m_adj->set_value (m_adj->get_value () + m_adj->get_page_increment ());
        }

        void decrement_page ()
        {
            m_adj->set_value (m_adj->get_value () - m_adj->get_page_increment ());
        }

        void init ()
        {
            add_events (Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK |
                    Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_MOTION_MASK |
                    Gdk::KEY_PRESS_MASK | Gdk::FOCUS_CHANGE_MASK);
            set_size_request (static_cast<int>(2.0 * x_padding + min_width),
                    static_cast<int>(2.0 * y_padding + min_height));
            m_adj.reset (new Gtk::Adjustment (0.0 /* initial value */,
                        0.0 /* lower */,
                        1.0 /* upper */,
                        0.025 /* step increment */,
                        0.05 /* page increment */));
            m_adjustment_signal_connection = m_adj->signal_value_changed ().connect
                (sigc::mem_fun (this, &Priv::on_adjustment_value_changed));
            m_drag_started = false;
            set_has_tooltip ();
            signal_query_tooltip ().connect (sigc::mem_fun (this,
                        &Priv::on_query_tooltip));
        }

    };

    ColorScale::ColorScale (const boost::shared_ptr<ColorModel>& model, channel_t channel) :
        m_priv (new Priv (model, channel))
    {}


    void
    ColorScale::set_model (const boost::shared_ptr<ColorModel>& model)
    {
        THROW_IF_FAIL (m_priv);
        m_priv->set_model (model);
    }


    boost::shared_ptr<ColorModel> ColorScale::get_model ()
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_model;
    }

    void ColorScale::set_draw_value (bool enable)
    {
        THROW_IF_FAIL (m_priv);
        m_priv->m_draw_value = enable;
        m_priv->queue_draw ();
    }

    bool ColorScale::get_draw_value ()
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_draw_value;
    }

    void ColorScale::set_draw_label (bool enable)
    {
        THROW_IF_FAIL (m_priv);
        m_priv->set_draw_label (enable);
    }

    bool ColorScale::get_draw_label ()
    {
        THROW_IF_FAIL (m_priv);
        return static_cast<bool>(m_priv->m_text_layout);
    }


    Gtk::Widget& ColorScale::get_widget ()
    {
        THROW_IF_FAIL (m_priv);
        return *m_priv;
    }
}
