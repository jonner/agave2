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

#include "swatch.h"
#include "colormodel.h"
#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>

using agave::ColorModel;
using agave::Color;
using agave::Swatch;

int main (int argc, char** argv)
{
    Gtk::Main kit (argc, argv);
    Gtk::Window win;
    Gtk::VBox vbox;
    win.add (vbox);
    Swatch s1 (ColorModel::create (Color (1.0, 0.0, 0.0, 1.0)));
    Swatch s2 (ColorModel::create (Color (0.0, 1.0, 0.0, 1.0)));
    Swatch s3 (ColorModel::create (Color (0.0, 0.0, 1.0, 1.0)));
    Swatch s4 (ColorModel::create (Color (0.0, 0.0, 1.0, 0.5)));

    vbox.pack_start (s1.get_widget ());
    vbox.pack_start (s2.get_widget ());
    vbox.pack_start (s3.get_widget ());
    vbox.pack_start (s4.get_widget ());

    win.show_all ();

    Gtk::Main::run (win);
    return 0;
}
