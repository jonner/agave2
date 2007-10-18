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
#include <glibmm-utils/exception.h>
#include <gtkmm/liststore.h>
#include "scheme-combo-box.h"
#include "scheme-manager.h"

namespace agave
{
    struct SchemeColumnRecord : public Gtk::TreeModel::ColumnRecord
    {
        SchemeColumnRecord ()
        {
            add (name);
            add (scheme);
        }
        Gtk::TreeModelColumn<Glib::ustring> name;
        Gtk::TreeModelColumn<boost::shared_ptr<IScheme> > scheme;
    };

    struct SchemeComboBox::Priv
    {
        SchemeColumnRecord m_columns;
        Glib::RefPtr<Gtk::ListStore> m_model;

        Priv () :
            m_model (Gtk::ListStore::create (m_columns))
        {
            std::vector<boost::shared_ptr<IScheme> > schemes =
                SchemeManager::instance ().get_schemes ();
            typedef std::vector<boost::shared_ptr<IScheme> >::const_iterator scheme_iter_t;
            for (scheme_iter_t iter = schemes.begin ();
                    iter != schemes.end (); ++iter)
            {
                Gtk::TreeModel::iterator tree_iter = m_model->append ();
                (*tree_iter)[m_columns.name] = (*iter)->get_name ();
                (*tree_iter)[m_columns.scheme] = *iter;
            }
        }
    };

    SchemeComboBox::SchemeComboBox () :
        m_priv (new Priv ())
    {
        THROW_IF_FAIL (m_priv);
        set_model (m_priv->m_model);
        pack_start (m_priv->m_columns.name);
        set_active (0);
    }

    boost::shared_ptr<IScheme> SchemeComboBox::get_active_scheme ()
    {
        THROW_IF_FAIL (m_priv);
        Gtk::TreeModel::iterator active_iter = get_active ();
        if (active_iter)
        {
            return (*active_iter)[m_priv->m_columns.scheme];
        }
        return boost::shared_ptr<IScheme> ();
    }
}
