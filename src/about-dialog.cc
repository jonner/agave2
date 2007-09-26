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
#include <glib/gi18n.h>
#include "config.h"
#include "about-dialog.h"

namespace agave
{
    AboutDialog::AboutDialog ()
    {
        set_name (PACKAGE_NAME);
        set_version (PACKAGE_VERSION);
        set_comments (_(PACKAGE_DESCRIPTION));
        //dialog.set_logo_icon_name ("agave-icon");
        std::vector<Glib::ustring> authors;
        authors.push_back ("Jonathon Jongsma");
        set_authors (authors);
        set_translator_credits ("translator-credits");
        set_website (PACKAGE_WEBSITE);
    }

    void AboutDialog::on_response(int response_id)
    {
        if (response_id == Gtk::RESPONSE_DELETE_EVENT ||
                response_id == Gtk::RESPONSE_CANCEL ||
                response_id == Gtk::RESPONSE_CLOSE)
        {
            hide();
        }
    }
}
