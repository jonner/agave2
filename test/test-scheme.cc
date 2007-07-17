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
#include <iostream>
#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include "scheme.h"
#include "swatch.h"

using namespace agave;

struct SchemeBox : public Gtk::HBox
{
    SchemeBox (const Scheme& scheme)
    {
        int i = 0;
        for (std::vector<Color>::const_iterator color_iter = scheme.colors ().begin ();
                color_iter != scheme.colors ().end (); ++color_iter)
        {
            c[i].reset (new agave::Swatch(*color_iter));
            pack_start (*c[i]);
            ++i;
        }
    }

    boost::shared_ptr<agave::Swatch> c[5];
};

int main (int argc, char** argv)
{
    Gtk::Main kit (argc, argv);
    Gtk::Window win;
    Gtk::VBox vbox;
    win.add (vbox);

    hsv_t hsv = {0.0, 1.0, 1.0, 1.0};
    Color c (hsv);
    Scheme s(c, SCHEME_TYPE_ANALOGOUS);
    std::cout << s << std::endl;
    SchemeBox box0(s);
    vbox.pack_start (box0);
    s.set_scheme_type (SCHEME_TYPE_MONOCHROMATIC);
    std::cout << s << std::endl;
    SchemeBox box1(s);
    vbox.pack_start (box1);
    s.set_scheme_type (SCHEME_TYPE_TRIAD);
    std::cout << s << std::endl;
    SchemeBox box2(s);
    vbox.pack_start (box2);
    s.set_scheme_type (SCHEME_TYPE_COMPLEMENTARY);
    std::cout << s << std::endl;
    SchemeBox box3(s);
    vbox.pack_start (box3);
    s.set_scheme_type (SCHEME_TYPE_COMPOUND);
    std::cout << s << std::endl;
    SchemeBox box4(s);
    vbox.pack_start (box4);
    s.set_scheme_type (SCHEME_TYPE_SHADES);
    std::cout << s << std::endl;
    SchemeBox box5(s);
    vbox.pack_start (box5);
    s.set_scheme_type (SCHEME_TYPE_TETRAD);
    std::cout << s << std::endl;
    SchemeBox box6(s);
    vbox.pack_start (box6);
    s.set_scheme_type (SCHEME_TYPE_CUSTOM);
    std::cout << s << std::endl;
    SchemeBox box7(s);
    vbox.pack_start (box7);

    win.show_all ();
    Gtk::Main::run (win);
    return 0;
}
