/*******************************************************************************
 *
 *  Copyright (c) 2007 Jonathon Jongsma
 *
 *  This file is part of Agave
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>
 *
 *******************************************************************************/
#include <gtkmm.h>
#include "color-relation.h"
#include "swatch.h"
#include "colorscale.h"

using namespace agave;

class Window : public Gtk::Window
{
    public:
        Window ()
        {
            for (int i = 0; i < NUM_COLORS; ++i)
            {
                ColorModel::pointer model = ColorModel::pointer (new ColorModel ());
                m_models.push_back (model);
                boost::shared_ptr<Swatch> swatch (new Swatch (model));
                m_swatches.push_back (swatch);
                boost::shared_ptr<ColorScale> scale (new ColorScale (model, ColorScale::CHANNEL_HUE));
                m_scales.push_back (scale);
                boost::shared_ptr<Gtk::VBox> vbox (new Gtk::VBox ());
                m_vboxes.push_back (vbox);
                vbox->pack_start (*swatch);
                vbox->pack_start (*scale, Gtk::PACK_SHRINK);
                hbox.pack_start (*vbox);

                if (i != 0)
                {
                    boost::shared_ptr<ColorRelation> relation (new ColorRelation(m_models[0], model));
                    m_relations.push_back (relation);
                }
            }

            add (hbox);
            set_size_request (500, 150);
            show_all ();
        }

    private:
        static const int NUM_COLORS = 5;
        Gtk::HBox hbox;
        std::vector<ColorModel::pointer> m_models;
        std::vector<boost::shared_ptr<Swatch> > m_swatches;
        std::vector<boost::shared_ptr<ColorRelation> > m_relations;
        std::vector<boost::shared_ptr<ColorScale> > m_scales;
        std::vector<boost::shared_ptr<Gtk::VBox> > m_vboxes;
};

int main (int argc, char** argv)
{
    Gtk::Main kit (argc, argv);
    Window win;
    Gtk::Main::run (win);
    return 0;
}
