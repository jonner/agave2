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
            typedef boost::shared_ptr<ColorModel> pointer;
            ColorModel ();
            void set_color (const Color& c);
            Color& get_color ();

            sigc::signal<void>& signal_color_changed () const { return m_signal_color_changed; }

        protected:
            void on_color_changed ();

        private:
            mutable sigc::signal<void> m_signal_color_changed;
            Color m_color;
    };
}

#endif // __COLORMODEL_H
