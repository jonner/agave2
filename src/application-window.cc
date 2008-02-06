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
#include <glibmm-utils/exception.h>
#include <glibmm-utils/log-stream-utils.h>
#include "config.h"
#include "application-window.h"
#include "about-dialog.h"
#include "color-group-box.h"
#include "scheme-combo-box.h"
#include "color-relation.h"
#include "i-scheme.h"
#include "color-wheel.h"

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
        std::vector<boost::shared_ptr<ColorModel> > m_colors;
        static const int DEFAULT_NUM_COLORS = 5;
        boost::shared_ptr<ColorModel> m_base_color;
        std::vector<boost::shared_ptr<ColorRelation> > m_relations;
        boost::shared_ptr<IScheme> m_scheme;
        Gtk::VBox m_vbox;
        Gtk::VBox m_vlayout;
        ColorWheel m_wheel;
        ColorGroupBox m_color_group;
        SchemeComboBox m_scheme_combo;

        Priv () :
            m_ui_manager (Gtk::UIManager::create ()),
            m_actions (Gtk::ActionGroup::create ())
        {
            // create the color models
            for (int i = 0; i < DEFAULT_NUM_COLORS; ++i)
            {
                boost::shared_ptr<ColorModel> model = ColorModel::create ();
                m_colors.push_back (model);
                // the middle one is the base color
                bool is_base = (i == 2);
                if (is_base)
                {
                    m_base_color = model;
                }
                m_wheel.add_color(model, is_base);
                m_color_group.add_color(model, is_base);
            }

            init_actions ();
            init_signals ();
            m_ui_manager->add_ui_from_string (menus);
            m_ui_manager->insert_action_group (m_actions);
            add_accel_group (m_ui_manager->get_accel_group ());
            set_title (PACKAGE_NAME);
            // dynamic_cast doesn't work here for some reason
            Gtk::Menu* main_menu = static_cast<Gtk::Menu*> (m_ui_manager->get_widget ("/MainMenu"));
            THROW_IF_FAIL (main_menu);

            m_vbox.pack_start (*main_menu, Gtk::PACK_SHRINK);
            m_vbox.pack_start (m_vlayout);

            m_vlayout.set_border_width (6);
            m_vlayout.set_spacing (6);
            m_wheel.get_widget ().set_size_request (200, 200);
            m_vlayout.pack_start (m_wheel.get_widget ());
            m_vlayout.pack_start (m_color_group);
            m_vlayout.pack_start (m_scheme_combo, Gtk::PACK_SHRINK);

            // trigger the scheme change manually at startup so we start with
            // some scheme relationship
            on_scheme_combo_changed();

            add (m_vbox);
            set_default_icon_name ("agave");
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

        void init_signals ()
        {
            m_scheme_combo.signal_changed ().connect (sigc::mem_fun (this,
                        &Priv::on_scheme_combo_changed));
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
            hide ();
        }

        void on_action_help_about ()
        {
            static AboutDialog dialog;
            dialog.run ();
        }

        void on_scheme_combo_changed ()
        {
            LOG_FUNCTION_SCOPE_NORMAL_DD ;
            set_scheme (m_scheme_combo.get_active_scheme ());
        }

        void set_scheme (const boost::shared_ptr<IScheme>& scheme)
        {
            m_scheme = scheme;
            THROW_IF_FAIL (m_base_color);
            // get rid of the old scheme's relations, if any
            m_relations.clear ();

            boost::shared_ptr<ColorRelation> relation;
            relation.reset (new
                    ColorRelation(m_base_color, m_colors[0],
                        scheme->get_outer_left ()));
            m_relations.push_back (relation);

            relation.reset (new
                    ColorRelation(m_base_color, m_colors[1],
                        scheme->get_inner_left ()));
            m_relations.push_back (relation);

            relation.reset (new
                    ColorRelation(m_base_color, m_colors[3],
                        scheme->get_inner_right ()));
            m_relations.push_back (relation);

            relation.reset (new
                    ColorRelation(m_base_color, m_colors[4],
                        scheme->get_outer_right ()));
            m_relations.push_back (relation);
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
