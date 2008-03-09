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
#ifndef __COLOR_SET_H
#define __COLOR_SET_H

#include <list>
#include <glibmm/ustring.h>
#include "color.h"

namespace agave
{
    class ColorSet
    {
        public:
            typedef std::list<Color>::iterator iterator;
            typedef std::list<Color>::const_iterator const_iterator;
            typedef std::list<Color>::reverse_iterator reverse_iterator;
            typedef std::list<Color>::const_reverse_iterator const_reverse_iterator;

            ColorSet ();
            std::string get_id () const;
            void set_id (std::string new_id);
            Glib::ustring get_name () const;
            void set_name (Glib::ustring name);
            Glib::ustring get_description () const;
            void set_description (Glib::ustring description);
            void add_tag (Glib::ustring tag);
            void remove_tag (Glib::ustring tag);
            std::list<Glib::ustring> get_tags () const;
            void set_colors (const std::list<Color>& colors);
            std::list<Color> get_colors () const;
            void clear ();
            bool operator== (const ColorSet& other);

            iterator begin ();
            const_iterator begin () const;
            iterator end ();
            const_iterator end () const;
            reverse_iterator rbegin ();
            const_reverse_iterator rbegin () const;
            reverse_iterator rend ();
            const_reverse_iterator rend () const;


            friend std::ostream& operator<<(std::ostream& out, const ColorSet& s);

        private:
            std::string update_id ();

            std::string m_id;
            Glib::ustring m_name;
            Glib::ustring m_description;
            std::list<Glib::ustring> m_tags;
            std::list<Color> m_colors;
    };
}

#endif // __COLOR_SET_H
