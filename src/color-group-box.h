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
#ifndef __COLOR_GROUP_BOX_H
#define __COLOR_GROUP_BOX_H

#include <boost/shared_ptr.hpp>
#include <gtkmm/box.h>
#include "i-multi-color-view.h"

namespace agave
{
    class ColorGroupBox :
        public Gtk::HBox,
        public IMultiColorView
    {
        public:
            ColorGroupBox ();
            virtual void add_color (const boost::shared_ptr<ColorModel>& model, bool highlight);
            virtual unsigned int get_num_colors () const;


        private:
            struct Priv;
            boost::shared_ptr<Priv> m_priv;
    };
}

#endif // __COLOR_GROUP_BOX_H
