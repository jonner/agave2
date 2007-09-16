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
#include "color-edit-box.h"

using namespace agave;

class Window : public Gtk::Window
{
    public:
        Window ()
        {
            for (int i = 0; i < NUM_COLORS; ++i)
            {
                boost::shared_ptr<ColorEditBox> edit_box (new ColorEditBox ());
                m_edit_boxes.push_back (edit_box);
                hbox.pack_start (*edit_box);

                if (i != 0)
                {
                    boost::shared_ptr<ColorRelation> relation (new
                            ColorRelation(m_edit_boxes[0]->get_model (),
                                edit_box->get_model ()));
                    m_relations.push_back (relation);
                }
            }

            hbox.set_spacing (6);
            hbox.set_border_width (6);
            add (hbox);
            show_all ();
        }

    private:
        static const int NUM_COLORS = 5;
        Gtk::HBox hbox;
        std::vector<ColorModel::pointer> m_models;
        std::vector<boost::shared_ptr<ColorEditBox> > m_edit_boxes;
        std::vector<boost::shared_ptr<ColorRelation> > m_relations;
};

int main (int argc, char** argv)
{
    Gtk::Main kit (argc, argv);
    Window win;
    Gtk::Main::run (win);
    return 0;
}
