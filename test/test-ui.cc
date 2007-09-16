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

#include <gtkmm.h>
#include <boost/shared_ptr.hpp>
#include "color-edit-box.h"

using namespace agave;

int main (int argc, char** argv)
{
    const int NUM_BOXES = 5;
    Gtk::Main kit (argc, argv);
    Gtk::Window win;
    ColorEditBox boxes[NUM_BOXES];
    Gtk::HBox hbox;
    win.add (hbox);
    for (int i = 0; i < NUM_BOXES; ++i)
    {
        hsv_t hsv = { static_cast<double>(i) / static_cast<double>(NUM_BOXES), 1.0, 1.0, 1.0 };
        Color c (hsv);
        boxes[i].set_color (c);
        hbox.pack_start (boxes[i]);
    }
    win.show_all ();
    Gtk::Main::run (win);
    return 0;
}
