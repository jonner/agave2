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

#include <algorithm>
#include <iomanip>
#include <glibmm.h>
#include "color-set.h"

namespace agave
{
    std::string ColorSet::update_id ()
    {
        using Glib::ustring;
        std::string input;
        for (std::list<Color>::const_iterator it = m_colors.begin ();
                it != m_colors.end (); ++it)
        {
            input += ustring::compose ("%1-%2-%3-%4",
                ustring::format (std::fixed, std::setprecision(4), it->get_red ()),
                ustring::format (std::fixed, std::setprecision(4), it->get_green ()),
                ustring::format (std::fixed, std::setprecision(4), it->get_blue ()),
                ustring::format (std::fixed, std::setprecision(4), it->get_alpha ()));
        }
        return Glib::Checksum::compute_checksum (Glib::Checksum::CHECKSUM_SHA1, input);
    }

    static int session_count = 0;

    ColorSet::ColorSet ()
    {
        using Glib::ustring;
        m_name = ustring::compose ("Color Set %1",
                                   ustring::format(++session_count));
    }

    std::string ColorSet::get_id () const
    {
        return m_id;
    }

    void ColorSet::set_id (std::string new_id)
    {
        m_id = new_id;
    }

    Glib::ustring ColorSet::get_name () const
    {
        return m_name;
    }

    void ColorSet::set_name (Glib::ustring name)
    {
        m_name = name;
    }

    Glib::ustring ColorSet::get_description () const
    {
        return m_description;
    }

    void ColorSet::set_description (Glib::ustring description)
    {
        m_description = description;
    }

    void ColorSet::add_tag (Glib::ustring tag)
    {
        std::list<Glib::ustring>::iterator iter =
            std::find (m_tags.begin (), m_tags.end (), tag);
        if (iter == m_tags.end ())
        {
            m_tags.push_back (tag);
        }
    }

    void ColorSet::remove_tag (Glib::ustring tag)
    {
        std::list<Glib::ustring>::iterator iter =
            std::find (m_tags.begin (), m_tags.end (), tag);
        if (iter != m_tags.end ())
        {
            m_tags.erase (iter);
        }
    }

    std::list<Glib::ustring> ColorSet::get_tags () const
    {
        return m_tags;
    }

    void ColorSet::set_colors (const std::list<Color>& colors)
    {
        m_colors.assign (colors.begin (), colors.end ());
        m_id = update_id ();
    }

    std::list<Color> ColorSet::get_colors () const
    {
        return m_colors;
    }

    void ColorSet::clear ()
    {
        m_name.clear ();
        m_description.clear ();
        m_tags.clear ();
        m_colors.clear ();
    }

    bool ColorSet::operator== (const ColorSet& other)
    {
        return (m_id == other.m_id);
    }

    ColorSet::iterator ColorSet::begin ()
    {
        return m_colors.begin ();
    }

    ColorSet::const_iterator ColorSet::begin () const
    {
        return m_colors.begin ();
    }

    ColorSet::iterator ColorSet::end ()
    {
        return m_colors.end ();
    }

    ColorSet::const_iterator ColorSet::end () const
    {
        return m_colors.end ();
    }

    ColorSet::reverse_iterator ColorSet::rbegin ()
    {
        return m_colors.rbegin ();
    }

    ColorSet::const_reverse_iterator ColorSet::rbegin () const
    {
        return m_colors.rbegin ();
    }

    ColorSet::reverse_iterator ColorSet::rend ()
    {
        return m_colors.rend ();
    }

    ColorSet::const_reverse_iterator ColorSet::rend () const
    {
        return m_colors.rend ();
    }

    std::ostream& operator<<(std::ostream& out, const ColorSet& s)
    {
        out << "ID: " << s.m_id << std::endl;
        out << "Name: " << s.get_name () << std::endl;
        out << "Description: " << s.get_description () << std::endl;
        for (ColorSet::const_iterator color_iter = s.begin ();
             color_iter != s.end ();
             ++color_iter)
        {
            out << *color_iter << std::endl;
        }
        return out;
    }
}
