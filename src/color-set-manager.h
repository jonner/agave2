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
#ifndef __COLOR_SET_MANAGER_H
#define __COLOR_SET_MANAGER_H

#include "color-set.h"

namespace agave
{
    class ColorSetManager
    {
        public:
            typedef std::list<ColorSet>::iterator iterator;
            typedef std::list<ColorSet>::const_iterator const_iterator;
            typedef std::list<ColorSet>::reverse_iterator reverse_iterator;
            typedef std::list<ColorSet>::const_reverse_iterator const_reverse_iterator;

            ColorSetManager (std::string filename);
            void load ();
            void save ();
            void add_set (const ColorSet& set);
            void remove_set (const ColorSet& set);
            void clear ();

            iterator begin ();
            const_iterator begin () const;
            iterator end ();
            const_iterator end () const;
            reverse_iterator rbegin ();
            const_reverse_iterator rbegin () const;
            reverse_iterator rend ();
            const_reverse_iterator rend () const;

        private:
            const std::string m_filename;
            std::list<ColorSet> m_sets;
    };
}

#endif // __COLOR_SET_MANAGER_H
