/*******************************************************************************
 *
 *  Copyright (c) 2008 Jonathon Jongsma
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

#include "color-set-details-editor.h"
#include <glibmm-utils/exception.h>
#include <glibmm/i18n.h>
#include <gtkmm/dialog.h>
#include <gtkmm/label.h>
#include <gtkmm/table.h>
#include <gtkmm/entry.h>
#include <gtkmm/stock.h>

namespace agave
{
    struct ColorSetDetailsEditor::Priv : Gtk::Dialog
    {
        Priv (ColorSet& set) :
            m_set (set),
            m_label_name (_("Name:"), Gtk::ALIGN_RIGHT),
            m_label_desc (_("Description:"), Gtk::ALIGN_RIGHT),
            m_label_tags (_("Tags:"), Gtk::ALIGN_RIGHT),
            m_label_id (_("ID:"), Gtk::ALIGN_RIGHT),
            m_label_id_value (set.get_id (), Gtk::ALIGN_LEFT),
            m_table (4, 2)
        {
            set_title (_("Edit Color Set Details"));
            set_has_separator (false);
            add_button (Gtk::Stock::CLOSE, Gtk::RESPONSE_CLOSE);
            Gtk::VBox* vbox = get_vbox ();
            vbox->pack_start (m_content);
            m_content.set_border_width (6);
            m_table.set_col_spacings (6);
            m_table.set_row_spacings (6);
            m_content.pack_start (m_table);

            m_table.attach (m_label_id, 0, 1, 0, 1);
            m_table.attach (m_label_id_value, 1, 2, 0, 1);

            m_table.attach (m_label_name, 0, 1, 1, 2);
            m_table.attach (m_entry_name, 1, 2, 1, 2);

            m_table.attach (m_label_desc, 0, 1, 2, 3);
            m_table.attach (m_entry_desc, 1, 2, 2, 3);

            m_table.attach (m_label_tags, 0, 1, 3, 4);
            m_table.attach (m_entry_tags, 1, 2, 3, 4);

            m_entry_name.set_text (set.get_name ());
            m_entry_desc.set_text (set.get_description ());

            show_all ();
        }

        void on_response (int response_id)
        {
            if (response_id == Gtk::RESPONSE_CLOSE)
            {
                m_set.set_name (m_entry_name.get_text ());
                m_set.set_description (m_entry_desc.get_text ());
            }
        }

        ColorSet& m_set;
        Gtk::Label m_label_name, m_label_desc, m_label_tags, m_label_id, m_label_id_value;
        Gtk::Entry m_entry_name, m_entry_desc, m_entry_tags;
        Gtk::Table m_table;
        Gtk::VBox m_content;
    };

    ColorSetDetailsEditor::ColorSetDetailsEditor (ColorSet& set) :
        m_priv (new Priv (set))
    {}

    int ColorSetDetailsEditor::run ()
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->run ();
    }
}
