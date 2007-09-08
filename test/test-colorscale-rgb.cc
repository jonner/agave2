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
#include "colorscale.h"

using namespace agave;

int main (int argc, char** argv)
{
    Gtk::Main kit (argc, argv);
    Gtk::Window w;
    ColorModel::pointer model (new ColorModel ());
    rgb_t rgb = {0.8, 0.6, 0.3};
    Color c (rgb);
    model->set_color (c);
    ColorScale r_scale (model, ColorScale::CHANNEL_RED);
    ColorScale g_scale (model, ColorScale::CHANNEL_GREEN);
    ColorScale b_scale (model, ColorScale::CHANNEL_BLUE);
    ColorScale a_scale (model, ColorScale::CHANNEL_ALPHA);
    b_scale.set_sensitive (false);
    Gtk::VBox vbox;
    vbox.pack_start (r_scale);
    r_scale.show ();
    vbox.pack_start (g_scale);
    g_scale.show ();
    vbox.pack_start (b_scale);
    b_scale.show ();
    vbox.pack_start (a_scale);
    a_scale.show ();
    w.add (vbox);
    vbox.show ();
    w.show_all_children ();
    w.set_default_size (200, 200);
    Gtk::Main::run (w);
    return 0;
}
