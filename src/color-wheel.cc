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
#include <algorithm>    // for std::find, std::for_each
#include <cmath>    // for floor()
#include <gdk/gdkkeysyms.h>
#include "color-wheel.h"
#include "color-model.h"
#include <goocanvasmm.h>
#include <goocanvas.h>
#include <glibmm-utils/exception.h>

namespace agave
{
    const int WHEEL_DEFAULT_SIZE = 200;
    const int MARKER_DEFAULT_SIZE = 10;
    const float STROKE_DEFAULT_WIDTH = 2.0;

    typedef sigc::slot<bool, double, double> SlotValidateDrop;
    typedef sigc::slot<std::pair<double, double>, const Color&> SlotDeterminePosition;
    typedef sigc::slot<Color, double, double> SlotDetermineColor;
    static bool drop_anywhere (double x, double y)
    {
        return true;
    }

    class MarkerItem :
        public Goocanvas::Ellipse
    {
        public:
            static Glib::RefPtr<MarkerItem> create (const boost::shared_ptr<ColorModel>& model, double radius)
            { return Glib::RefPtr<MarkerItem> (new MarkerItem (model, radius)); }

            void set_validate_drop_func (const SlotValidateDrop& func)
            { m_validate_func = func; }

            void set_color_position_funcs (const SlotDetermineColor& color_func,
                    const SlotDeterminePosition& position_func)
            {
                m_position_func = position_func;
                m_color_func = color_func;
                update_position_from_color ();
            }

            boost::shared_ptr<ColorModel> get_model () { return m_model;}
            void set_model (const boost::shared_ptr<ColorModel>& model) { m_model = model;}

        protected:
            bool on_focus_event (const Glib::RefPtr<Goocanvas::Item>& target,
                    GdkEventFocus* event)
            {
                g_return_val_if_fail (event, false);

                if (event->in)
                {
                    property_stroke_color_rgba () = get_stroke_color (FOCUSED);
                    raise ();
                }
                else
                {
                    property_stroke_color_rgba () = get_stroke_color (UNFOCUSED);
                }
                return false;
            }

            bool xon_button_press_event (const Glib::RefPtr<Goocanvas::Item>& target,
                    GdkEventButton* event)
            {
                m_dragging = true;
                m_drag_origin_x = event->x;
                m_drag_origin_y = event->y;
                get_canvas ()->pointer_grab(target,
                        Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_RELEASE_MASK,
                        Gdk::Cursor(Gdk::HAND1),
                        event->time);
                return false;
            }

            bool xon_button_release_event (const Glib::RefPtr<Goocanvas::Item>& target,
                    GdkEventButton* event)
            {
                m_dragging = false;
                m_drag_origin_x = 0.0;
                m_drag_origin_y = 0.0;
                get_canvas ()->pointer_ungrab(target, event->time);
                return false;
            }

            bool xon_motion_notify_event (const Glib::RefPtr<Goocanvas::Item>& target,
                    GdkEventMotion* event)
            {
                if (m_dragging && m_validate_func (event->x, event->y))
                {
                    move_to (event->x, event->y);
                    update_color_from_position ();
                }
                return false;
            }

            void move_to (double x, double y)
            {
                property_center_x () = x;
                property_center_y () = y;
            }

            void set_background_color (const Color& color)
            {
                // FIXME: this is kind of a nasty hack because we end up losing a
                // lot of precision -- isn't there a better way to do this?
                unsigned char pixel[4];
                pixel[3] = static_cast<unsigned char>(color.get_red () *
                        std::numeric_limits<unsigned char>::max ());
                pixel[2] = static_cast<unsigned char>(color.get_green () *
                        std::numeric_limits<unsigned char>::max ());
                pixel[1] = static_cast<unsigned char>(color.get_blue () *
                        std::numeric_limits<unsigned char>::max ());
                pixel[0] = std::numeric_limits<unsigned char>::max ();
                property_fill_color_rgba () = *reinterpret_cast<uint32_t*>(pixel);
            }

            void on_color_changed ()
            {
                set_background_color (m_model->get_color ());

                // when we drag the marker, it changes the color model, so we
                // don't want to modify the position due to that color model
                // change
                if (!m_dragging && m_position_func)
                {
                    std::pair<double, double> new_position =
                        m_position_func (m_model->get_color ());
                    move_to (new_position.first, new_position.second);
                }
            }

            bool xon_key_press_event (const Glib::RefPtr<Goocanvas::Item>& target, GdkEventKey* event)
            {
                const guint ADJUSTMENT = 2;
                g_return_val_if_fail (event, false);
                double newx = property_center_x (), newy = property_center_y ();
                switch (event->keyval)
                {
                    case GDK_Left:
                        newx = property_center_x () - ADJUSTMENT;
                        break;
                    case GDK_Right:
                        newx = property_center_x () + ADJUSTMENT;
                        break;
                    case GDK_Up:
                        newy = property_center_y () - ADJUSTMENT;
                        break;
                    case GDK_Down:
                        newy = property_center_y () + ADJUSTMENT;
                        break;
                    default:
                        // let any other keys be handled by the default handlers
                        return false;
                }
                if (m_validate_func (newx, newy))
                {
                    move_to (newx, newy);
                    update_color_from_position ();
                }
                return true;
            }


        private:
            MarkerItem (const boost::shared_ptr<ColorModel>& model, double radius) :
                Goocanvas::Ellipse (0.0, 0.0, radius, radius),
                m_dragging (false),
                m_drag_origin_x (0),
                m_drag_origin_y (0),
                m_validate_func (sigc::ptr_fun (drop_anywhere)),
                m_model (model)
            {
                property_pointer_events () = Goocanvas::CANVAS_EVENTS_ALL;
                property_line_width () = STROKE_DEFAULT_WIDTH;
                property_can_focus () = true;
                signal_focus_out_event ().connect (sigc::mem_fun (this,
                            &MarkerItem::on_focus_event));
                signal_focus_in_event ().connect (sigc::mem_fun (this,
                            &MarkerItem::on_focus_event));
                signal_button_press_event ().connect (sigc::mem_fun (this,
                            &MarkerItem::xon_button_press_event));
                signal_button_release_event ().connect (sigc::mem_fun (this,
                            &MarkerItem::xon_button_release_event));
                signal_motion_notify_event ().connect (sigc::mem_fun (this,
                            &MarkerItem::xon_motion_notify_event));
                signal_key_press_event ().connect (sigc::mem_fun (this,
                            &MarkerItem::xon_key_press_event));
                if (m_model) {
                    m_model->signal_color_changed ().connect (sigc::mem_fun
                            (this, &MarkerItem::on_color_changed));
                }

                property_stroke_color_rgba () = get_stroke_color (UNFOCUSED);

                // initialize the bacground color and position of the marker
                on_color_changed ();
            }

            void update_position_from_color ()
            {
                if (m_position_func)
                {
                    std::pair<double, double> new_position =
                        m_position_func (m_model->get_color ());
                    move_to (new_position.first, new_position.second);
                }
            }


            void update_color_from_position ()
            {
                if (m_color_func) {
                    Color new_color = m_color_func (property_center_x (),
                            property_center_y ());
                    m_model->set_color (new_color);
                }
            }

            enum FocusState {
                FOCUSED,
                UNFOCUSED
            };

            inline uint32_t get_stroke_color (FocusState focus)
            {
                Gtk::StateType gtk_state;
                if (focus == FOCUSED)
                    gtk_state = Gtk::STATE_SELECTED;
                else
                    gtk_state = Gtk::STATE_NORMAL;

                Glib::RefPtr<Gtk::Style> style;
                if (get_canvas ())
                { style = get_canvas ()->get_style (); }
                else
                { style = Gtk::Widget::get_default_style (); }
                // Gdk::Color only returns rgb without alpha, so we have to
                // shift over and add in a full alpha component
                return (style->get_fg(gtk_state).get_pixel () << 8 | 0xff);
            }

            bool m_dragging;
            double m_drag_origin_x, m_drag_origin_y;
            SlotValidateDrop m_validate_func;
            SlotDeterminePosition m_position_func;
            SlotDetermineColor m_color_func;
            boost::shared_ptr<ColorModel> m_model;
            static const uint32_t COLOR_FOCUSED;
            static const uint32_t COLOR_UNFOCUSED;
    };


    class WheelItem :
        public Goocanvas::Ellipse
    {
        public:
            static Glib::RefPtr<WheelItem> create (double xc, double yc, double radius)
            { return Glib::RefPtr<WheelItem> (new WheelItem (xc, yc, radius)); }

            std::pair<double, double> position_for_color (const Color& color)
            {
                double x = 0.0, y = 0.0;
                hsv_t hsv = color.as_hsv ();
                x = hsv.s * cos (hsv.h * 2.0 * G_PI) * property_radius_x () +
                    property_center_x ();
                y = hsv.s * -sin (hsv.h * 2.0 * G_PI) * property_radius_y () +
                    property_center_y ();
                return std::make_pair (x, y);
            }

            Color color_at_position (double x, double y)
            {
                double dy = - (y - property_center_y ());
                double dx = x - property_center_x ();
                double angle = atan2 (dy, dx);
                if (angle < 0.0)
                {
                    angle += 2.0 * G_PI;
                }
                double dist = sqrt (dx * dx + dy * dy);

                hsv_t hsv;
                hsv.h = angle / (2.0 * G_PI);
                hsv.s = std::min (dist / (property_radius_x ()), 1.0);
                hsv.v = 1.0;
                hsv.a = 1.0;
                return Color (hsv);
            }

            bool is_in_path (double x, double y)
            {
                double dy = std::abs (y - property_center_y ());
                double dx = std::abs (x - property_center_x ());
                double dist = std::sqrt (dx * dx + dy * dy);
                if (dist < property_radius_x ())
                {
                    return true;
                }
                return false;
            }


        protected:
            WheelItem (double xc, double yc, double radius) :
                Goocanvas::Ellipse(xc, yc, radius, radius)
            {
                update_pattern ();
                // FIXME: this doesn't work in goocanvasmm -- needs
                // investigation
                //property_fill_pattern () = m_pattern;
                g_object_set (gobj (), "fill-pattern", m_pattern->cobj (), NULL);

                property_center_x ().signal_changed ().connect (sigc::mem_fun
                        (this, &WheelItem::update_pattern));
                property_center_y ().signal_changed ().connect (sigc::mem_fun
                        (this, &WheelItem::update_pattern));
                property_radius_x ().signal_changed ().connect (sigc::mem_fun
                        (this, &WheelItem::update_pattern));
                property_radius_y ().signal_changed ().connect (sigc::mem_fun
                        (this, &WheelItem::update_pattern));
                property_line_width () = STROKE_DEFAULT_WIDTH;
            }

            virtual ~WheelItem () {}

        private:
            void update_pattern ()
            {
                Cairo::RefPtr<Cairo::ImageSurface> image_surface =
                    Cairo::ImageSurface::create (Cairo::FORMAT_ARGB32,
                            // FIXME: do we really have to create the image
                            // surface from the 0,0 point of the entire canvas?
                            // that could be huge in theory...
                            static_cast<int>(property_center_x () + property_radius_x ()),
                            static_cast<int>(property_center_y () + property_radius_y ()));
                unsigned char *data = image_surface->get_data ();
                for (int row = 0; row < image_surface->get_height (); ++row)
                {
                    int dy = row;
                    for (int px = 0; px < image_surface->get_width (); ++px)
                    {
                        int dx = px;
                        Color c = color_at_position (dx, dy);
                        rgb_t rgb = c.as_rgb ();
                        *data++ =
                            static_cast<unsigned char>(
                                    rgb.b * static_cast<double>(std::numeric_limits<unsigned char>::max ()));
                        *data++ =
                            static_cast<unsigned char>(
                                    rgb.g * static_cast<double>(std::numeric_limits<unsigned char>::max ()));
                        *data++ =
                            static_cast<unsigned char>(
                                    rgb.r * static_cast<double>(std::numeric_limits<unsigned char>::max ()));
                        *data++ = std::numeric_limits<unsigned char>::max ();
                    }
                }
                image_surface->flush ();
                m_pattern = Cairo::SurfacePattern::create (image_surface);
            }

            Cairo::RefPtr<Cairo::SurfacePattern> m_pattern;
    };


    struct ColorWheel::Priv : public Goocanvas::Canvas
    {
        Glib::RefPtr<WheelItem> m_wheel;
        typedef std::vector<Glib::RefPtr<MarkerItem> > marker_vector_t;
        marker_vector_t m_markers;

        Priv ()
        {
            set_size_request (WHEEL_DEFAULT_SIZE, WHEEL_DEFAULT_SIZE);
            m_wheel = WheelItem::create (WHEEL_DEFAULT_SIZE / 2.0,
                                         WHEEL_DEFAULT_SIZE / 2.0,
                                         WHEEL_DEFAULT_SIZE / 2.0 -
                                             MARKER_DEFAULT_SIZE - 2 *
                                             STROKE_DEFAULT_WIDTH);
            get_root_item ()->add_child (m_wheel);
        }

        void on_realize ()
        {
            Goocanvas::Canvas::on_realize();
            guint pixel = get_style ()->get_bg (Gtk::STATE_NORMAL).get_pixel ();
            property_background_color_rgb () = pixel;
        }

        void add_color (const boost::shared_ptr<ColorModel>& model)
        {
            bool found = false;
            for (marker_vector_t::const_iterator i = m_markers.begin ();
                    i != m_markers.end (); ++i)
            {
                if ((*i)->get_model () == model)
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                // not yet in the list, so add it
                Glib::RefPtr<MarkerItem> marker = MarkerItem::create (model, MARKER_DEFAULT_SIZE);
                get_root_item ()->add_child (marker);
                m_markers.push_back (marker);
                // also connect to the changed signal so that we can redraw when
                // a color is changed
                marker->set_color_position_funcs (
                        sigc::mem_fun (m_wheel.operator->(),
                            &WheelItem::color_at_position),
                        sigc::mem_fun (m_wheel.operator->(),
                            &WheelItem::position_for_color));
                marker->set_validate_drop_func (sigc::mem_fun
                        (m_wheel.operator->(), &WheelItem::is_in_path));
                marker->signal_button_press_event ().connect (sigc::mem_fun
                        (this, &Priv::on_marker_button_press));
            }
        }

        bool on_marker_button_press (const Glib::RefPtr<Goocanvas::Item>& target,
                GdkEventButton* event)
        {
            grab_focus (target);
            return false;
        }

        void remove_color (const boost::shared_ptr<ColorModel>& model)
        {
            bool found = false;
            marker_vector_t::iterator i;
            for (i = m_markers.begin (); i != m_markers.end (); ++i)
            {
                if ((*i)->get_model () == model)
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                // color found, remove it
                int id = get_root_item ()->find_child (*i);
                get_root_item ()->remove_child (id);
                marker_vector_t::iterator next_iter (i);
                std::advance (next_iter, 1);
                m_markers.erase (i, next_iter);
            }
        }
    };

    ColorWheel::ColorWheel () :
        m_priv (new Priv ())
    {
    }

    Gtk::Widget& ColorWheel::get_widget ()
    {
        THROW_IF_FAIL (m_priv);
        return *m_priv;
    }

    void ColorWheel::add_color (const boost::shared_ptr<ColorModel>& model, bool highlight)
    {
        THROW_IF_FAIL (m_priv);
        m_priv->add_color (model);
    }

    unsigned int ColorWheel::get_num_colors () const
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_markers.size ();
    }
}
