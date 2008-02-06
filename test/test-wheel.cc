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

#include "color-wheel.h"
#include "colormodel.h"
#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>

int main (int argc, char** argv)
{
    Gtk::Main kit (argc, argv);
    Gtk::Window win;
    Gtk::VBox vbox;
    win.add (vbox);
    agave::ColorWheel wheel;
    boost::shared_ptr<agave::ColorModel> color1 (new agave::ColorModel (agave::Color (0.8, 0.8, 0.4)));
    wheel.add_color (color1);
    boost::shared_ptr<agave::ColorModel> color2 (new agave::ColorModel (agave::Color (0.4, 0.8, 0.4)));
    wheel.add_color (color2);
    boost::shared_ptr<agave::ColorModel> color3 (new agave::ColorModel (agave::Color (1.0, 0.0, 0.0)));
    wheel.add_color (color3);
    boost::shared_ptr<agave::ColorModel> color4 (new agave::ColorModel (agave::Color (1.0, 1.0, 1.0)));
    wheel.add_color (color4);

    vbox.pack_start (wheel.get_widget ());

    win.show_all ();

    Gtk::Main::run (win);
    return 0;
}
