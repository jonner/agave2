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
#ifndef __COLORSCALE_H
#define __COLORSCALE_H

#include <gtkmm/drawingarea.h>
#include <gtkmm/adjustment.h>
#include <boost/shared_ptr.hpp>
#include "colormodel.h"

namespace agave
{
    class ColorScale : public Gtk::DrawingArea
    {
        public:
            enum channel_t {
                CHANNEL_HUE,
                CHANNEL_SATURATION,
                CHANNEL_VALUE,
                CHANNEL_RED,
                CHANNEL_GREEN,
                CHANNEL_BLUE,
                CHANNEL_ALPHA
            };

            ColorScale (channel_t channel);
            ColorScale (const ColorModel::pointer& model, channel_t channel);
            void set_model (const ColorModel::pointer& model);
            ColorModel::pointer get_model ();
            void set_alpha (double new_alpha);
            void set_draw_value (bool enable=true);
            bool get_draw_value ();

        protected:
            virtual bool on_expose_event (GdkEventExpose* event);
            virtual bool on_motion_notify_event (GdkEventMotion* event);
            virtual bool on_button_press_event (GdkEventButton* event);
            virtual bool on_scroll_event (GdkEventScroll* event);
            virtual bool on_enter_notify_event (GdkEventCrossing* event);
            virtual bool on_leave_notify_event (GdkEventCrossing* event);
            virtual bool on_focus_in_event (GdkEventFocus* event);
            virtual bool on_focus_out_event (GdkEventFocus* event);
            virtual bool on_key_press_event (GdkEventKey* event);
            void render_scale (Cairo::RefPtr<Cairo::Context>& cr);
            void render_selectors (Cairo::RefPtr<Cairo::Context>& cr);
            void render_checks (Cairo::RefPtr<Cairo::Context>& cr, double x, double y, double w, double h);
            void render_stipple (Cairo::RefPtr<Cairo::Context>& cr, double x, double y, double w, double h);
            void on_adjustment_value_changed ();
            void on_color_changed ();
            void update_adjustment (const Color& c);
            double inside_x () const;
            double inside_y () const;
            double inside_width () const;
            double inside_height () const;
            double outside_x () const;
            double outside_y () const;
            double outside_width () const;
            double outside_height () const;

        private:
            double get_value_from_coords (double x, double y);
            void increment_page ();
            void decrement_page ();
            void init ();
            const channel_t m_channel;
            boost::shared_ptr<Gtk::Adjustment> m_adj;
            double last_adj_val;
            ColorModel::pointer m_model;
            bool m_draw_value;
    };
}
#endif // __COLORSCALE_H
