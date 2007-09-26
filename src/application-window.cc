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
#include <boost/shared_ptr.hpp>
#include <gtkmm/window.h>
#include <gtkmm/label.h>
#include <gtkmm/uimanager.h>
#include <gtkmm/box.h>
#include <gtkmm/menu.h>
#include <gtkmm/stock.h>
#include <glib/gi18n.h>
#include <glibmm-utils/exception.hh>
#include "config.h"
#include "application-window.h"
#include "about-dialog.h"

namespace agave
{

    const Glib::ustring menus =
        "<ui>"
        "<menubar name='MainMenu'>"
        "<menu action='MenuAgave'>"
        "<menuitem action='Quit'/>"
        "</menu>"
        "<menu action='MenuHelp'>"
        "<menuitem action='About'/>"
        "</menu>"
        "</menubar>"
        "</ui>";

    struct ApplicationWindow::Priv : public Gtk::Window
    {
        Glib::RefPtr<Gtk::UIManager> m_ui_manager;
        Glib::RefPtr<Gtk::ActionGroup> m_actions;
        boost::shared_ptr<Gtk::VBox> m_vbox;
        Priv () :
            m_ui_manager (Gtk::UIManager::create ()),
            m_actions (Gtk::ActionGroup::create ()),
            m_vbox (new Gtk::VBox)
        {
            init_actions ();
            m_ui_manager->add_ui_from_string (menus);
            m_ui_manager->insert_action_group (m_actions);
            set_title (PACKAGE_NAME);
            // dynamic_cast doesn't work here for some reason
            Gtk::Menu* main_menu = static_cast<Gtk::Menu*> (m_ui_manager->get_widget ("/MainMenu"));
            g_assert (m_vbox);
            add (*m_vbox);
            g_assert (main_menu);
            m_vbox->pack_start (*main_menu, Gtk::PACK_SHRINK);
            show_all ();
        }

        void init_actions ()
        {
            m_actions->add (
                    Gtk::Action::create ("MenuAgave", "_Agave"));
            m_actions->add (
                    Gtk::Action::create ("Quit", Gtk::Stock::QUIT),
                    sigc::mem_fun (this, &Priv::on_action_quit));
            m_actions->add (
                    Gtk::Action::create ("MenuHelp", _("_Help")));
            m_actions->add (
                    Gtk::Action::create ("About", Gtk::Stock::ABOUT),
                    sigc::mem_fun (this, &Priv::on_action_help_about));
        }

        void set_subtitle (const Glib::ustring& subtitle)
        {
            Glib::ustring title;
            if (subtitle.empty ())
            {
                title = PACKAGE_NAME;
            }
            else
            {
                title = subtitle + " - " + PACKAGE_NAME;
            }
            set_title (title);
        }

        void on_action_quit ()
        {
            printf ("quitting\n");
            hide ();
        }

        void on_action_help_about ()
        {
            static AboutDialog dialog;
            dialog.run ();
        }
    };

    ApplicationWindow::ApplicationWindow () :
        m_priv (new Priv ())
    {
    }

    void ApplicationWindow::set_subtitle (const Glib::ustring& subtitle)
    {
        THROW_IF_FAIL (m_priv);
        m_priv->set_subtitle (subtitle);
    }

    Gtk::Window& ApplicationWindow::get_window () const
    {
        THROW_IF_FAIL (m_priv);
        return *m_priv;
    }
}
