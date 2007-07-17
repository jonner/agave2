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
#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>

int main (int argc, char** argv)
{
    Gtk::Main kit (argc, argv);
    Gtk::Window win;
    Gtk::VBox vbox;
    win.add (vbox);
    agave::Swatch s1 (agave::Color (1.0, 0.0, 0.0, 1.0));
    agave::Swatch s2 (agave::Color (0.0, 1.0, 0.0, 1.0));
    agave::Swatch s3 (agave::Color (0.0, 0.0, 1.0, 1.0));
    agave::Swatch s4 (agave::Color (0.0, 0.0, 1.0, 0.5));

    vbox.pack_start (s1);
    vbox.pack_start (s2);
    vbox.pack_start (s3);
    vbox.pack_start (s4);

    win.show_all ();

    Gtk::Main::run (win);
    return 0;
}
