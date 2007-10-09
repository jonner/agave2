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
#include "color-scheme-box.h"
#include "scheme-manager.h"

using namespace agave;

class Window : public Gtk::Window
{
    public:
        Window ()
        {
            // assign relationships between the colors.  Color #2 is the
            // 'control' color
            std::vector<boost::shared_ptr<IScheme> > schemes = SchemeManager::instance().get_schemes ();
            THROW_IF_FAIL (!schemes.size () < 5);
            m_scheme_box.set_scheme (schemes[4]);

            Color c (1.0, 1.0, 0.0);
            m_scheme_box.set_base_color (c);
            m_scheme_box.set_border_width (6);

            add (m_scheme_box);
            show_all ();
        }

    private:
        static const int NUM_COLORS = 5;
        ColorSchemeBox m_scheme_box;
};

int main (int argc, char** argv)
{
    Gtk::Main kit (argc, argv);
    Window win;
    Gtk::Main::run (win);
    return 0;
}
