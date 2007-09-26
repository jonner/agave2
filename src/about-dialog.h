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
#ifndef __ABOUT_DIALOG_H
#define __ABOUT_DIALOG_H
#include <gtkmm/aboutdialog.h>

namespace agave
{
    class AboutDialog : public Gtk::AboutDialog
    {
        public:
            AboutDialog ();
        private:
            virtual void on_response(int response_id);
    };

}

#endif // __ABOUT_DIALOG_H
