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
#include "color-wheel.h"
#include "color-model.h"
#include <goocanvasmm.h>
#include <goocanvas.h>
#include <glibmm-utils/exception.h>

namespace agave
{
    const double BORDER_WIDTH = 2.0;
    const int MIN_WIDGET_SIZE = 100;

    /*
    static bool
    bounds_intersect (const Goocanvas::Bounds& b1, const Goocanvas::Bounds& b2)
    {
        if (b1.get_x1 () > b2.get_x2 ()
                || b1.get_x2 () < b2.get_x1 ()
                || b1.get_y1 () > b2.get_y2 ()
                || b1.get_y2 () < b2.get_y1 ()) {
            return false;
        }
        return true;
    }
    */

    typedef sigc::slot<bool, double, double> SlotValidateDrop;
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
            { m_validate = func; }

            boost::shared_ptr<ColorModel> get_model () { return m_model;}
            void set_model (const boost::shared_ptr<ColorModel>& model) { m_model = model;}

        protected:
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
                if (m_dragging && m_validate (event->x, event->y))
                {
                    property_center_x () = event->x;
                    property_center_y () = event->y;
                }
                return false;
            }

            void on_color_changed ()
            {
                // FIXME: this is kind of a nasty hack because we end up losing a
                // lot of precision -- isn't there a better way to do this?
                unsigned char color[4];
                color[3] = static_cast<unsigned char>(m_model->get_color ().get_red () *
                        std::numeric_limits<unsigned char>::max ());
                color[2] = static_cast<unsigned char>(m_model->get_color ().get_green () *
                        std::numeric_limits<unsigned char>::max ());
                color[1] = static_cast<unsigned char>(m_model->get_color ().get_blue () *
                        std::numeric_limits<unsigned char>::max ());
                color[0] = std::numeric_limits<unsigned char>::max ();
                property_fill_color_rgba () = *reinterpret_cast<uint32_t*>(color);
                // FIXME: move the marker to the proper location on the color wheel
            }


        private:
            MarkerItem (const boost::shared_ptr<ColorModel>& model, double radius) :
                Goocanvas::Ellipse (0.0, 0.0, radius, radius),
                m_dragging (false),
                m_drag_origin_x (0),
                m_drag_origin_y (0),
                m_validate (sigc::ptr_fun (drop_anywhere)),
                m_model (model)
            {
                property_pointer_events () = Goocanvas::CANVAS_EVENTS_ALL;
                signal_button_press_event ().connect (sigc::mem_fun (this,
                            &MarkerItem::xon_button_press_event));
                signal_button_release_event ().connect (sigc::mem_fun (this,
                            &MarkerItem::xon_button_release_event));
                signal_motion_notify_event ().connect (sigc::mem_fun (this,
                            &MarkerItem::xon_motion_notify_event));
                if (m_model) {
                    m_model->signal_color_changed ().connect (sigc::mem_fun
                            (this, &MarkerItem::on_color_changed));
                }
                // FIXME: set center coordinates based on color value
            }

            bool m_dragging;
            double m_drag_origin_x, m_drag_origin_y;
            SlotValidateDrop m_validate;
            boost::shared_ptr<ColorModel> m_model;
    };

    class WheelItem :
        public Goocanvas::ItemSimple
    {
        public:
            static Glib::RefPtr<WheelItem> create (double xc, double yc, double radius)
            { return Glib::RefPtr<WheelItem> (new WheelItem (xc, yc, radius)); }
            bool is_in_bounds (double x, double y)
            {
                Cairo::RefPtr<Cairo::Context> cr =
                    get_canvas ()->create_cairo_context ();
                create_path (cr);
                return (cr->in_fill (x, y));
            }

        protected:
            WheelItem (double xc, double yc, double radius) :
                Goocanvas::ItemSimple(),
                m_xc (xc), m_yc (yc), m_radius (radius),
                m_canvas (0),
                m_parent (0),
                m_need_update (true),
                m_need_entire_subtree_update (true),
                m_prop_description (*this, "description")
            {
                m_bounds.set_x1 (0.0);
                m_bounds.set_x2 (0.0);
                m_bounds.set_y1 (0.0);
                m_bounds.set_y2 (0.0);
            }

            virtual ~WheelItem () {}

            virtual void
            update_vfunc (bool entire_tree,
                          const Cairo::RefPtr<Cairo::Context> &cr,
                          Goocanvas::Bounds& bounds)
            {
                Goocanvas::Bounds child_bounds;
                if (entire_tree || m_need_update) {
                    if (m_canvas) {
                        // request redraw of the existing bounds
                        goo_canvas_request_redraw(m_canvas, m_bounds.gobj ());
                    }

                    cr->save ();

                    if (m_need_entire_subtree_update) {
                        entire_tree = true;
                    }
                    m_need_update = false;
                    m_need_entire_subtree_update = false;

                    // pretend to update all of the child items :)
                    for (child_array_t::const_iterator it = m_children.begin ();
                            it != m_children.end (); ++it) {
                        (*it)->update (entire_tree, cr, child_bounds);
                    }

                    // calculate the group's bounds
                    create_path (cr);
                    double x1, x2, y1, y2;
                    cr->get_stroke_extents (x1, y1, x2, y2);
                    bounds.set_x1 (x1);
                    bounds.set_y1 (y1);
                    bounds.set_x2 (x2);
                    bounds.set_y2 (y2);
                    m_bounds = bounds;

                    cr->restore ();

                    if (m_canvas) {
                        // request redraw of the new bounds
                        goo_canvas_request_redraw(m_canvas, m_bounds.gobj ());
                    }
                }
            }

            virtual void paint_vfunc (const Cairo::RefPtr<Cairo::Context> &cr,
                    const Goocanvas::Bounds &bounds,
                    double scale)
            {
                /*
                   if (!bounds_intersect (bounds, get_bounds ())) {
                   return;
                   }
                   */
                cr->save ();
                // only paint within the exposed area
                cr->rectangle (bounds.get_x1 (), bounds.get_y1 (),
                        bounds.get_x2 () - bounds.get_x1 (),
                        bounds.get_y2 () - bounds.get_y1 ());
                cr->clip ();

                if (!m_image_surface)
                {
                    m_image_surface =
                        Cairo::ImageSurface::create (Cairo::FORMAT_ARGB32,
                                static_cast<int>(m_radius * 2.0),
                                static_cast<int>(m_radius * 2.0));
                }
                unsigned char *data = m_image_surface->get_data ();
                for (int row = 0; row < m_image_surface->get_height (); ++row)
                {
                    int dy = - (row - static_cast<int>(m_radius));
                    for (int px = 0; px < m_image_surface->get_width (); ++px)
                    {
                        int dx = px - static_cast<int>(m_radius);
                        double angle = atan2 (dy, dx);
                        if (angle < 0.0)
                        {
                            angle += 2.0 * G_PI;
                        }
                        double dist = sqrt (dx * dx + dy * dy);

                        hsv_t hsv;
                        hsv.h = angle / (2.0 * G_PI);
                        hsv.s = dist / (m_radius);
                        hsv.v = 1.0;
                        rgb_t rgb = Color::hsv_to_rgb (hsv);
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
                m_image_surface->flush ();

                cr->set_source (m_image_surface, m_xc - m_radius, m_yc - m_radius);
                create_path (cr);
                // fill the center of the circle with the image surface
                cr->fill ();
                // draw a border around the wheel
                cr->set_source_rgb (0.0, 0.0, 0.0);
                create_path (cr);
                cr->stroke ();
                cr->restore ();

                // now paint all child items
                for (child_array_t::const_iterator it = m_children.begin ();
                        it != m_children.end (); ++it) {
                    (*it)->paint (cr, bounds, scale);
                }
            }

            virtual GList*
            get_items_at_vfunc (double x,
                                double y,
                                const Cairo::RefPtr<Cairo::Context>& cr,
                                bool is_pointer_event,
                                bool parent_is_visible, GList* found_items)
            {
                typedef Glib::ListHandle<Glib::RefPtr<Goocanvas::Item> > item_list_t;
                if (is_in_bounds (x, y))
                {
                    found_items = g_list_append (found_items, gobj ());
                }
                item_list_t empty_list (NULL, Glib::OWNERSHIP_NONE);
                GList* new_found = 0;
                for (child_array_t::const_iterator it = m_children.begin ();
                        it != m_children.end (); ++it) {
                    new_found = (*it)->get_items_at (x, y, cr,
                            is_pointer_event,
                            parent_is_visible,
                            empty_list).data ();
                    if (new_found) {
                        // prepend the found item to the list of found items
                        found_items = g_list_concat (new_found, found_items);
                    }
                }
                return found_items;
            }

            virtual int
            get_n_children_vfunc () const
            { return m_children.size (); }

            virtual Glib::RefPtr<Goocanvas::Item>
            get_child_vfunc (int child_num)
            { return m_children[child_num]; }

            virtual void
            add_child_vfunc (const Glib::RefPtr<Goocanvas::Item>& child, int position)
            {
                m_children.push_back (child);
                goo_canvas_item_set_parent (child->gobj (), GOO_CANVAS_ITEM(gobj ()));
                request_update ();
            }

            virtual void
            request_update_vfunc ()
            {
                if (!m_need_update) {
                    m_need_update = true;
                    if (get_parent ()) {
                        get_parent ()->request_update ();
                    } else if (get_canvas ()) {
                        get_canvas ()->request_update ();
                    }
                }
            }

            virtual void
            set_canvas_vfunc (GooCanvas* canvas)
            { m_canvas = canvas; }

            virtual GooCanvas*
            get_canvas_vfunc ()
            { return m_canvas; }

            virtual void
            set_parent_vfunc (GooCanvasItem* parent)
            {
                m_parent = parent;
                if (parent) {
                    m_canvas = goo_canvas_item_get_canvas (parent);
                }
            }

            virtual GooCanvasItem*
            get_parent_vfunc ()
            { return m_parent; }

        private:
            void
            create_path (const Cairo::RefPtr<Cairo::Context>& cr)
            {
                cr->set_line_width (BORDER_WIDTH);
                cr->save ();
                cr->translate (m_xc, m_yc);
                cr->arc (0.0, 0.0, m_radius, 0.0, 2.0 * G_PI);
                cr->restore ();
            }
            double m_xc, m_yc, m_radius;
            typedef std::vector<Glib::RefPtr<Goocanvas::Item> > child_array_t;
            child_array_t m_children;
            GooCanvas* m_canvas;
            GooCanvasItem* m_parent;
            bool m_need_update;
            bool m_need_entire_subtree_update;
            Goocanvas::Bounds m_bounds;
            Glib::Property<Glib::ustring> m_prop_description;
            Cairo::RefPtr<Cairo::ImageSurface> m_image_surface;
    };


    struct ColorWheel::Priv : public Goocanvas::Canvas
    {
        Cairo::RefPtr<Cairo::ImageSurface> m_image_surface;
        Glib::RefPtr<WheelItem> m_wheel;
        typedef std::vector<Glib::RefPtr<MarkerItem> > marker_vector_t;
        marker_vector_t m_markers;

        Priv ()
        {
            set_size_request (MIN_WIDGET_SIZE, MIN_WIDGET_SIZE);
            m_wheel = WheelItem::create (MIN_WIDGET_SIZE / 2.0, MIN_WIDGET_SIZE / 2.0, MIN_WIDGET_SIZE / 2.0 - BORDER_WIDTH);
            get_root_item ()->add_child (m_wheel);
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
                Glib::RefPtr<MarkerItem> marker = MarkerItem::create (model, 10.0);
                get_root_item ()->add_child (marker);
                m_markers.push_back (marker);
                // also connect to the changed signal so that we can redraw when
                // a color is changed
                marker->property_center_x ().signal_changed ().connect
                    (sigc::bind (sigc::mem_fun (this, &Priv::on_marker_moved),
                                 marker));
                marker->property_center_y ().signal_changed ().connect
                    (sigc::bind (sigc::mem_fun (this, &Priv::on_marker_moved),
                                 marker));
            }
        }

        void on_marker_moved (const Glib::RefPtr<MarkerItem>& marker)
        {
            LOG_DD ("a marker moved");
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
