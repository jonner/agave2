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
#ifndef __COLORSCALE_H
#define __COLORSCALE_H

#include <boost/shared_ptr.hpp>
#include <gtkmm/widget.h>
#include "colormodel.h"

namespace agave
{
    class ColorScale
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
            void set_draw_label (bool enable=true);
            bool get_draw_label ();
            Gtk::Widget& get_widget ();

        private:
            struct Priv;
            boost::shared_ptr<Priv> m_priv;
    };
}
#endif // __COLORSCALE_H
