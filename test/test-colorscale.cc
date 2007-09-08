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

ColorModel::pointer model1, model2;
ColorScale *h_scale, *s_scale, *v_scale, *a_scale_hsv;
ColorScale *r_scale, *g_scale, *b_scale, *a_scale_rgb;

void on_button_clicked ()
{
    static bool foo = false;
    if (foo)
    {
        r_scale->set_model (model1);
        g_scale->set_model (model1);
        b_scale->set_model (model1);
        a_scale_rgb->set_model (model1);
        h_scale->set_model (model2);
        s_scale->set_model (model2);
        v_scale->set_model (model2);
        a_scale_hsv->set_model (model2);
    }
    else
    {
        r_scale->set_model (model2);
        g_scale->set_model (model2);
        b_scale->set_model (model2);
        a_scale_rgb->set_model (model2);
        h_scale->set_model (model1);
        s_scale->set_model (model1);
        v_scale->set_model (model1);
        a_scale_hsv->set_model (model1);
    }
    foo = !foo;
}

void init_models ()
{
    model1.reset (new ColorModel ());
    model2.reset (new ColorModel ());
    hsv_t hsv1 = {0.2, 0.8, 0.7, 0.4};
    Color c1 (hsv1);
    model1->set_color (c1);
    hsv_t hsv2 = {0.8, 0.2, 0.4, 0.7};
    Color c2 (hsv2);
    model2->set_color (c2);
}

int main (int argc, char** argv)
{
    Gtk::Main kit (argc, argv);
    Gtk::Window w;
    Gtk::VBox vbox;
    Gtk::HBox hbox;
    Gtk::VBox vbox_rgb;
    Gtk::VBox vbox_hsv;
    vbox.pack_start (hbox);
    hbox.pack_start (vbox_rgb);
    hbox.pack_start (vbox_hsv);

    init_models ();

    // set up rgb scales
    r_scale = new ColorScale (model1, ColorScale::CHANNEL_RED);
    g_scale = new ColorScale (model1, ColorScale::CHANNEL_GREEN);
    b_scale = new ColorScale (model1, ColorScale::CHANNEL_BLUE);
    a_scale_rgb = new ColorScale (model1, ColorScale::CHANNEL_ALPHA);
    vbox_rgb.pack_start (*r_scale);
    vbox_rgb.pack_start (*g_scale);
    vbox_rgb.pack_start (*b_scale);
    vbox_rgb.pack_start (*a_scale_rgb);

    // set up hsv scales
    h_scale = new ColorScale (model2, ColorScale::CHANNEL_HUE);
    s_scale = new ColorScale (model2, ColorScale::CHANNEL_SATURATION);
    v_scale = new ColorScale (model2, ColorScale::CHANNEL_VALUE);
    a_scale_hsv = new ColorScale (model2, ColorScale::CHANNEL_ALPHA);
    vbox_hsv.pack_start (*h_scale);
    vbox_hsv.pack_start (*s_scale);
    vbox_hsv.pack_start (*v_scale);
    vbox_hsv.pack_start (*a_scale_hsv);

    Gtk::Button button("Swap Models");
    button.signal_clicked ().connect (sigc::ptr_fun (&on_button_clicked));
    vbox.pack_start (button, Gtk::PACK_SHRINK);

    w.add (vbox);
    w.show_all ();
    w.set_default_size (400, 200);
    Gtk::Main::run (w);

    delete h_scale;
    delete s_scale;
    delete v_scale;
    delete a_scale_hsv;
    delete r_scale;
    delete g_scale;
    delete b_scale;
    delete a_scale_rgb;
    return 0;
}
