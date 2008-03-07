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
#include <glibmm.h>
#include "color-set.h"

namespace agave
{
    static std::string next_id ()
    {
        // create a hash of the user's name, the current time, and a random
        // value
        Glib::Rand rand;
        Glib::TimeVal tval;
        tval.assign_current_time ();
        std::ostringstream stream;
        stream << Glib::get_user_name () << "-" << tval.tv_sec << "-" << tval.tv_usec << "-" << rand.get_int ();
        Glib::Checksum sha1(Glib::Checksum::CHECKSUM_SHA1);
        sha1.update (stream.str ());
        return sha1.get_string ();
    }

    ColorSet::ColorSet () :
        m_id (next_id ())
    {
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

    std::ostream& operator<<(std::ostream& out, const ColorSet& s)
    {
        out << "ID: " << s.m_id << std::endl;
        out << "Name: " << s.get_name () << std::endl;
        out << "Description: " << s.get_description () << std::endl;
        std::list<Color>::const_iterator color_iter;
        for (color_iter = s.m_colors.begin ();
                color_iter != s.m_colors.end ();
                ++color_iter)
        {
            out << *color_iter << std::endl;
        }
        return out;
    }
}
