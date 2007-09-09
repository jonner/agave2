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
#include "colorscale.h"
#include "swatch.h"

using namespace agave;

class ColorEditBox : public Gtk::VBox
{
    public:
        ColorEditBox () :
            m_model (new ColorModel ()),
            m_swatch (m_model),
            m_scale_r (m_model, ColorScale::CHANNEL_RED),
            m_scale_g (m_model, ColorScale::CHANNEL_GREEN),
            m_scale_b (m_model, ColorScale::CHANNEL_BLUE),
            m_scale_h (m_model, ColorScale::CHANNEL_HUE),
            m_scale_s (m_model, ColorScale::CHANNEL_SATURATION),
            m_scale_v (m_model, ColorScale::CHANNEL_VALUE)
        {
            m_hsv_box.set_spacing (3);
            m_hsv_box.set_border_width (3);
            m_hsv_box.pack_start (m_scale_h, Gtk::PACK_SHRINK);
            m_hsv_box.pack_start (m_scale_s, Gtk::PACK_SHRINK);
            m_hsv_box.pack_start (m_scale_v, Gtk::PACK_SHRINK);
            m_notebook.append_page (m_hsv_box, "HSV", "Specify the color in HSV Colorspace");

            m_rgb_box.set_spacing (3);
            m_rgb_box.set_border_width (3);
            m_rgb_box.pack_start (m_scale_r, Gtk::PACK_SHRINK);
            m_rgb_box.pack_start (m_scale_g, Gtk::PACK_SHRINK);
            m_rgb_box.pack_start (m_scale_b, Gtk::PACK_SHRINK);
            m_notebook.append_page (m_rgb_box, "RGB", "Specify the color in RGB Colorspace");

            m_swatch.set_size_request (100, 100);
            m_swatch.set_draw_border ();
            pack_start (m_swatch);
            pack_start (m_notebook, Gtk::PACK_SHRINK);
            set_border_width (6);
            set_spacing (6);
        }

    private:
        ColorModel::pointer m_model;
        Swatch m_swatch;
        Gtk::Notebook m_notebook;
        Gtk::VBox m_hsv_box, m_rgb_box;
        ColorScale m_scale_r, m_scale_g, m_scale_b;
        ColorScale m_scale_h, m_scale_s, m_scale_v;
};

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
        hbox.pack_start (boxes[i]);
    }
    win.show_all ();
    Gtk::Main::run (win);
    return 0;
}
