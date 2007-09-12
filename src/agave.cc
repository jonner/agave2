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
#include <iostream>
#include <glibmm/exception.h>
#include "application.h"

int main (int argc, char** argv)
{
    int exit_code = 0;
    try {
        agave::Application app (argc, argv);
        exit_code = app.run ();
    } catch (const std::exception& e)
    {
        std::cerr << "Unhandled exception: " << e.what () << std::endl;
        exit_code = 1;
    }
    catch (const Glib::Exception& e)
    {
        std::cerr << "Unhandled exception: " << e.what () << std::endl;
        exit_code = 1;
    }
    return exit_code;
}
