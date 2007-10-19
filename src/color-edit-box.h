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
#ifndef __COLOR_EDIT_BOX_H
#define __COLOR_EDIT_BOX_H

#include <boost/shared_ptr.hpp>
#include <gtkmm/box.h>
#include "colormodel.h"
#include "colorscale.h"
#include "swatch.h"

namespace agave
{
    class ColorEditBox : public Gtk::VBox
    {
        public:
            ColorEditBox ();

            void set_color (const Color& c);
            ColorModel::pointer get_model ();
            Swatch& get_swatch ();
            ColorScale& get_red_scale ();
            ColorScale& get_green_scale ();
            ColorScale& get_blue_scale ();
            ColorScale& get_hue_scale ();
            ColorScale& get_saturation_scale ();
            ColorScale& get_value_scale ();

        private:
            struct Priv;
            boost::shared_ptr<Priv> m_priv;
    };
}

#endif // __COLOR_EDIT_BOX_H
