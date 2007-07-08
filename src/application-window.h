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
#ifndef __APPLICATION_WINDOW_H
#define __APPLICATION_WINDOW_H

#include <boost/shared_ptr.hpp>

namespace Gtk
{
    class Window;
}

namespace agave
{

    class ApplicationWindow
    {
        public:
            ApplicationWindow ();
            void set_subtitle (const Glib::ustring& subtitle);
            Gtk::Window& get_window () const;

        private:
            struct Priv;
            boost::shared_ptr<Priv> m_priv;
    };
}

#endif __APPLICATION_WINDOW_H
