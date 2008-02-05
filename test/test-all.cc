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
#include "swatch.h"

using namespace agave;

class TestWindow : public Gtk::Window
{
    public:
        TestWindow () :
            m_model (new ColorModel (Color (0.8, 0.1, 0.1, 0.8))),
            m_rscale (m_model, ColorScale::CHANNEL_RED),
            m_gscale (m_model, ColorScale::CHANNEL_GREEN),
            m_bscale (m_model, ColorScale::CHANNEL_BLUE),
            m_ascale_rgb (m_model, ColorScale::CHANNEL_ALPHA),
            m_hscale (m_model, ColorScale::CHANNEL_HUE),
            m_sscale (m_model, ColorScale::CHANNEL_SATURATION),
            m_vscale (m_model, ColorScale::CHANNEL_VALUE),
            m_ascale_hsv (m_model, ColorScale::CHANNEL_ALPHA),
            m_swatch (m_model)
        {
            set_title ("Agave Widget Test");
            m_hbox.pack_start (m_box_selection);
            m_hbox.pack_start (m_box_rgb);
            m_hbox.pack_start (m_box_hsv);
            //m_hbox.pack_start (m_box_scheme);

            m_box_selection.pack_start (m_swatch);

            m_box_rgb.pack_start (m_rscale.get_widget ());
            m_box_rgb.pack_start (m_gscale.get_widget ());
            m_box_rgb.pack_start (m_bscale.get_widget ());
            m_box_rgb.pack_start (m_ascale_rgb.get_widget ());

            m_box_hsv.pack_start (m_hscale.get_widget ());
            m_box_hsv.pack_start (m_sscale.get_widget ());
            m_box_hsv.pack_start (m_vscale.get_widget ());
            m_box_hsv.pack_start (m_ascale_hsv.get_widget ());

            set_default_size (750, 150);

            add (m_hbox);
            show_all ();
        }

    private:
        Gtk::HBox m_hbox;
        Gtk::VBox m_box_selection, m_box_rgb, m_box_hsv, m_box_scheme;
        boost::shared_ptr<ColorModel> m_model;
        ColorScale m_rscale;
        ColorScale m_gscale;
        ColorScale m_bscale;
        ColorScale m_ascale_rgb;
        ColorScale m_hscale;
        ColorScale m_sscale;
        ColorScale m_vscale;
        ColorScale m_ascale_hsv;
        Swatch m_swatch;
};

int main (int argc, char** argv)
{
    Gtk::Main kit (argc, argv);
    TestWindow win;
    Gtk::Main::run (win);
}
