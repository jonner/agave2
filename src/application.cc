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

#include <gtkmm/main.h>
#include <glib/gi18n.h>
#include <glib/gutils.h>
#include "config.h"
#include "application.h"
#include "application-window.h"
#include "scheme.h"
#include "util.h"

namespace agave
{
    static bool version_requested = false;
    static Glib::ustring HELP_FOOTER = _("Copyright 2007, Jonathon Jongsma <jjongsma@gnome.org>");

    class OptionGroup : public Glib::OptionGroup
    {
        public:
            OptionGroup() :
                // these names dont' really matter since we're adding them to the main
                // group below, so they'll never be shown
                Glib::OptionGroup ("general", "General Options:",
                        "show general application options")
            {
                Glib::OptionEntry version_option;
                version_option.set_long_name ("version");
                version_option.set_description (_("Print application version"));
                add_entry (version_option, version_requested);
            }
    };

    struct Application::Priv
    {
        boost::shared_ptr<Scheme> m_current_scheme;
        boost::shared_ptr<Gtk::Main> m_main;
        boost::shared_ptr<ApplicationWindow> m_app_window;
        // TODO: saved schemes, history, prefs

        Priv (int argc, char** argv) :
            m_current_scheme (new Scheme ()),
            m_main (),
            m_app_window ()
        {
            bindtextdomain (GETTEXT_PACKAGE, AGAVE_LOCALEDIR);
            bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
            textdomain (GETTEXT_PACKAGE);

            Glib::OptionContext context;

#if GLIB_CHECK_VERSION (2, 12, 0)
            // FIXME: change these to glibmm functions when 2.14 is released
            g_option_context_set_summary (context.gobj (), _(PACKAGE_DESCRIPTION));
            g_option_context_set_description (context.gobj (), HELP_FOOTER.c_str ());
#endif

            OptionGroup option_group;
            context.set_main_group (option_group);
            option_group.set_translation_domain (GETTEXT_PACKAGE);

            m_main.reset (new Gtk::Main (argc, argv, context));
        }

        int run ()
        {
            if (version_requested)
            {
                std::cout << PACKAGE_NAME << " " << PACKAGE_VERSION << std::endl
                    << _(PACKAGE_DESCRIPTION) << std::endl;
                std::cout << "For more information, see " << PACKAGE_WEBSITE << std::endl;
            }
            else
            {
                //Gnome::Conf::init();
                m_app_window.reset (new ApplicationWindow ());
                THROW_IF_FAIL (m_app_window);
                Gtk::Main::run (m_app_window->get_window ());
            }
            // FIXME: return actual response code
            return 0;
        }
    };

    Application::Application (int argc, char** argv) :
        m_priv (new Priv (argc, argv))
    {
    }

    Application::~Application ()
    {
    }

    int Application::run ()
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->run ();
    }
}
