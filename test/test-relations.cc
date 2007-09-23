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
#include <glibmm-utils/exception.hh>
#include "color-relation.h"
#include "color-edit-box.h"
#include "scheme-manager.h"

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
            }

            // color #2 is the control color, so highlight it with a thicker
            // border
            m_edit_boxes[2]->get_swatch ().set_border_width (2.0);

            // assign relationships between the colors.  Color #2 is the
            // 'control' color
            std::vector<boost::shared_ptr<IScheme> > schemes = SchemeManager::instance().get_schemes ();
            THROW_IF_FAIL (!schemes.size () < 3);
            boost::shared_ptr<IScheme> first_scheme = schemes[2];

            boost::shared_ptr<ColorRelation> relation;
            relation.reset (new
                    ColorRelation(m_edit_boxes[2]->get_model (),
                        m_edit_boxes[0]->get_model (),
                        first_scheme->get_outer_left ()));
            m_relations.push_back (relation);

            relation.reset (new
                    ColorRelation(m_edit_boxes[2]->get_model (),
                        m_edit_boxes[1]->get_model (),
                        first_scheme->get_inner_left ()));
            m_relations.push_back (relation);

            relation.reset (new
                    ColorRelation(m_edit_boxes[2]->get_model (),
                        m_edit_boxes[3]->get_model (),
                        first_scheme->get_inner_right ()));
            m_relations.push_back (relation);

            relation.reset (new
                    ColorRelation(m_edit_boxes[2]->get_model (),
                        m_edit_boxes[4]->get_model (),
                        first_scheme->get_outer_right ()));
            m_relations.push_back (relation);

            Color c (1.0, 1.0, 0.0);
            m_edit_boxes[2]->get_model ()->set_color (c);

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
