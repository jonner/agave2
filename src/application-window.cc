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

#include <boost/shared_ptr.hpp>
#include <gtkmm/window.h>
#include <gtkmm/label.h>
#include <gtkmm/uimanager.h>
#include <gtkmm/box.h>
#include <gtkmm/menu.h>
#include <gtkmm/stock.h>
#include <gtkmm/aboutdialog.h>
#include <glib/gi18n.h>
#include "config.h"
#include "application-window.h"
#include "util.h"

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
            Gtk::AboutDialog dialog;
            dialog.set_name (PACKAGE_NAME);
            dialog.set_version (PACKAGE_VERSION);
            dialog.set_comments (_(PACKAGE_DESCRIPTION));
            //dialog.set_logo_icon_name ("agave-icon");
            std::vector<Glib::ustring> authors;
            authors.push_back ("Jonathon Jongsma");
            dialog.set_authors (authors);
            dialog.set_translator_credits ("translator-credits");
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
