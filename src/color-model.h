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
#ifndef __COLORMODEL_H
#define __COLORMODEL_H

#include <color.h>
#include <boost/shared_ptr.hpp>
#include <sigc++/signal.h>
#include <sigc++/trackable.h>

namespace agave
{
    class ColorModel : public sigc::trackable
    {
        public:
            ColorModel (const Color& c = Color ());
            void set_color (const Color& c);
            Color& get_color ();

            sigc::signal<void>& signal_color_changed () const;

            static boost::shared_ptr<ColorModel> create (const Color& c = Color());

        private:
            struct Priv;
            boost::shared_ptr<Priv> m_priv;
    };
}

#endif // __COLORMODEL_H
