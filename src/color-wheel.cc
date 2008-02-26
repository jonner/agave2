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
#include <gtkmm/drawingarea.h>
#include <glibmm-utils/exception.h>

namespace agave
{
    const double BORDER_WIDTH = 2.0;
    const int MIN_WIDGET_SIZE = 100;

    struct ColorWheel::Priv : public Gtk::DrawingArea
    {
        std::vector<boost::shared_ptr<ColorModel> > m_colors;
        Cairo::RefPtr<Cairo::ImageSurface> m_image_surface;

        Priv ()
        {
            set_size_request (MIN_WIDGET_SIZE, MIN_WIDGET_SIZE);
        }

        virtual bool on_expose_event (GdkEventExpose* event)
        {
            LOG_FUNCTION_SCOPE_NORMAL_DD;
            Cairo::RefPtr<Cairo::Context> cr = get_window ()->create_cairo_context ();
            cr->rectangle (event->area.x, event->area.y, event->area.width, event->area.height);
            cr->clip ();
            Gdk::Rectangle allocation = get_allocation ();
            double widget_size = static_cast<double> (
                    std::min (allocation.get_width (), allocation.get_height ()));
            LOG_DD ("Widget size: " << widget_size);

            // The marker size should be an integer value -- we're just storing
            // it as a double so that it can be used in cairo without casting
            double marker_size = floor (widget_size / 15.0);
            LOG_DD ("Marker size: " << marker_size);

            double diameter = widget_size - (2.0 * BORDER_WIDTH) - marker_size;
            LOG_DD ("Color wheel diameter: " << diameter);

            // check if we already have a cached image surface of the
            // appropriate size
            if (!(m_image_surface
                        && m_image_surface->get_width () == static_cast<int>(diameter)
                        && m_image_surface->get_height () == static_cast<int>(diameter)))
            {
                LOG_DD ("creating new image surface for color wheel, " << diameter << "x" << diameter);
                m_image_surface =
                    Cairo::ImageSurface::create (Cairo::FORMAT_ARGB32,
                            static_cast<int>(diameter), static_cast<int>(diameter));
                unsigned char *data = m_image_surface->get_data ();
                for (int row = 0; row < m_image_surface->get_height (); ++row)
                {
                    int dy = - (row - static_cast<int>(diameter / 2.0));
                    for (int px = 0; px < m_image_surface->get_width (); ++px)
                    {
                        int dx = px - static_cast<int>(diameter / 2.0);
                        double angle = atan2 (dy, dx);
                        if (angle < 0.0)
                        {
                            angle += 2.0 * G_PI;
                        }
                        double dist = sqrt (dx * dx + dy * dy);

                        hsv_t hsv;
                        hsv.h = angle / (2.0 * G_PI);
                        hsv.s = dist / (diameter / 2.0);
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
                m_image_surface->flush ();
            }
            //Cairo::RefPtr<Cairo::Pattern> pattern =
                //Cairo::SurfacePattern::create (m_image_surface);
            cr->set_source (m_image_surface,
                    (allocation.get_width () - diameter) / 2.0,
                    (allocation.get_height () - diameter) / 2.0);
            cr->translate (allocation.get_width () / 2.0,
                    allocation.get_height () / 2.0);
            cr->arc (0.0, 0.0, diameter / 2.0, 0.0, 2.0 * G_PI);
            // fill the center of the circle with the image surface
            cr->fill ();
            // draw a border around the wheel
            cr->set_source_rgb (0.0, 0.0, 0.0);
            cr->set_line_width (BORDER_WIDTH);
            cr->arc (0.0, 0.0, diameter / 2.0 + BORDER_WIDTH / 2.0, 0.0, 2.0 * G_PI);
            cr->stroke ();
            // calculate the (x,y) coordinates where the color markers should be
            // drawn on the color wheel
            CalcMarkerCoords coord_calulator =
                std::for_each (m_colors.begin (), m_colors.end (),
                        CalcMarkerCoords (diameter / 2.0));
            // draw lines from the center of the color wheel to the marker
            // position for each marker color
            std::for_each (coord_calulator.m_coords.begin (),
                    coord_calulator.m_coords.end (), DrawMarkerLines (cr));
            // draw circles at each marker coordinate.  These are all done last
            // so they're all above the marker lines.
            std::for_each (coord_calulator.m_coords.begin (),
                    coord_calulator.m_coords.end (), DrawMarkerCircles (cr, marker_size));
            return true;
        }

        void add_color (const boost::shared_ptr<ColorModel>& model)
        {
            std::vector<boost::shared_ptr<ColorModel> >::const_iterator i =
                std::find (m_colors.begin (), m_colors.end (), model);
            if (i == m_colors.end ())
            {
                // not yet in the list, so add it
                m_colors.push_back (model);
                // need to redraw if we just added it
                queue_draw ();
                // also connect to the changed signal so that we can redraw when
                // a color is changed
                model->signal_color_changed ().connect (sigc::mem_fun (this, &Priv::queue_draw));
            }
        }

        void remove_color (const boost::shared_ptr<ColorModel>& model)
        {
            std::vector<boost::shared_ptr<ColorModel> >::iterator i =
                std::find (m_colors.begin (), m_colors.end (), model);
            if (i != m_colors.end ())
            {
                // color found, remove it
                m_colors.erase (i, ++i);
                // need to redraw if we just removed it
                queue_draw ();
            }
        }

        struct CalcMarkerCoords
        {
            double m_radius;
            typedef std::pair<double, double> xy_t;
            typedef std::map <boost::shared_ptr<ColorModel>, xy_t> coord_map_t;
            coord_map_t m_coords;

            CalcMarkerCoords (double radius) :
                m_radius (radius)
            {}

            void operator () (const boost::shared_ptr<ColorModel>& model)
            {
                double dest_x, dest_y;
                Color c = model->get_color ();
                dest_x = m_radius * c.get_saturation () *
                    cos(c.get_hue () * 2.0 * G_PI);
                dest_y = - m_radius * c.get_saturation () *
                    sin(c.get_hue () * 2.0 * G_PI);
                m_coords[model] = std::make_pair (dest_x, dest_y);
            }
        };

        struct DrawMarkerLines
        {
            Cairo::RefPtr<Cairo::Context> m_cr;
            DrawMarkerLines (Cairo::RefPtr<Cairo::Context>& cr) :
                m_cr (cr)
            {}

            void operator () (const CalcMarkerCoords::coord_map_t::value_type& coords)
            {
                if (m_cr)
                {
                    m_cr->save ();
                    m_cr->push_group ();
                    m_cr->set_line_cap (Cairo::LINE_CAP_ROUND);
                    m_cr->move_to (0.0, 0.0);
                    m_cr->set_source_rgb (0.0, 0.0, 0.0);
                    m_cr->set_line_width (2.0);
                    m_cr->line_to (coords.second.first, coords.second.second);
                    m_cr->stroke ();
                    m_cr->pop_group_to_source ();
                    m_cr->paint_with_alpha (0.5);
                    m_cr->restore ();
                }
            }
        };

        struct DrawMarkerCircles
        {
            Cairo::RefPtr<Cairo::Context> m_cr;
            double m_marker_size;

            DrawMarkerCircles (Cairo::RefPtr<Cairo::Context>& cr, double marker_size) :
                m_cr (cr), m_marker_size (marker_size)
            {}

            void operator () (const CalcMarkerCoords::coord_map_t::value_type& coords)
            {
                if (m_cr)
                {
                    m_cr->save ();
                    m_cr->arc (coords.second.first, coords.second.second, m_marker_size / 2.0, 0.0, 2.0 * G_PI);
                    m_cr->set_source_rgb (coords.first->get_color ().get_red (),
                            coords.first->get_color ().get_green (),
                            coords.first->get_color ().get_blue ());
                    m_cr->fill_preserve ();
                    m_cr->set_source_rgb (0.0, 0.0, 0.0);
                    m_cr->stroke ();
                    m_cr->restore ();
                }
            }
        };
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
        return m_priv->m_colors.size ();
    }
}
