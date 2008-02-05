/*******************************************************************************
 *
 *  Copyright (c) 2008 Jonathon Jongsma
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
#ifndef __HSV_SCALE_SET_H
#define __HSV_SCALE_SET_H

#include "colorscale.h"
#include "colormodel.h"
#include "i-color-view.h"
#include <boost/shared_ptr.hpp>
#include <gtkmm/box.h>
#include <glibmm-utils/exception.h>

namespace agave
{

    class HsvScaleSet :
        public Gtk::VBox,
        public IColorView
    {
        public:
            virtual ~HsvScaleSet() {}
            HsvScaleSet (const boost::shared_ptr<ColorModel>& model);
            virtual void set_model (const boost::shared_ptr<ColorModel>& model);
            virtual boost::shared_ptr<ColorModel> get_model ();

        private:
            struct Priv;
            boost::shared_ptr<Priv> m_priv;
    };
}

#endif //  __HSV_SCALE_SET_H
