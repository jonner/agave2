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

#include <gtkmm/window.h>
#include <gtkmm/label.h>
#include "config.h"
#include "application-window.h"
#include "util.h"

namespace agave
{

    struct ApplicationWindow::Priv : public Gtk::Window
    {
        Priv ()
        {
            set_title (PACKAGE_NAME);
        }

        void set_subtitle (const Glib::ustring& subtitle)
        {
            Glib::ustring title;
            if (subtitle.empty ())
            {
                title = PACKAGE_NAME;
            }
            else
            {
                title = subtitle + " - " + PACKAGE_NAME;
            }
            set_title (title);
        }
    };

    ApplicationWindow::ApplicationWindow () :
        m_priv (new Priv ())
    {
    }

    void ApplicationWindow::set_subtitle (const Glib::ustring& subtitle)
    {
        THROW_IF_FAIL (m_priv);
        m_priv->set_subtitle (subtitle);
    }

    Gtk::Window& ApplicationWindow::get_window () const
    {
        THROW_IF_FAIL (m_priv);
        return *m_priv;
    }
}
