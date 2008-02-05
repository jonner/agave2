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
#ifndef __SWATCH_H
#define __SWATCH_H

#include <boost/shared_ptr.hpp>
#include <gtkmm/drawingarea.h>
#include <sigc++/signal.h>
#include "colormodel.h"

namespace agave
{
    class Swatch : public Gtk::DrawingArea
    {
        public:
            Swatch ();
            Swatch (const boost::shared_ptr<ColorModel>& model);
            Swatch (const Color& c);
            void set_model (const boost::shared_ptr<ColorModel>& model);
            void set_color (const Color& c);
            boost::shared_ptr<ColorModel> get_model ();
            boost::shared_ptr<const ColorModel> get_model () const;
            virtual bool on_expose_event (GdkEventExpose* event);
            void set_border_width (double width);
            void set_padding (int padding);

        protected:
            void on_color_changed ();
            void render_swatch (Cairo::RefPtr<Cairo::Context>& cr, double w, double h);
            void render_checks (Cairo::RefPtr<Cairo::Context>& cr, double x, double y, double w, double h);
            void on_drag_data_get(const Glib::RefPtr<Gdk::DragContext>& context,
                Gtk::SelectionData& selection_data, guint info, guint time);
            void set_color_icon(const Glib::RefPtr<Gdk::DragContext>& context);

        private:
            void init (const boost::shared_ptr<ColorModel>& model);
            void request_size ();
            struct Priv;
            boost::shared_ptr<Priv> m_priv;
    };
}

#endif // __SWATCH_H
