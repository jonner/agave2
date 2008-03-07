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

#include "color-set-manager.h"

namespace agave
{
    ColorSetManager::ColorSetManager (std::string filename) :
        m_filename (filename)
    { }

    void ColorSetManager::load ()
    {
        // FIXME: do this
    }

    void ColorSetManager::save ()
    {
        // FIXME: do this
    }

    void ColorSetManager::add_set (const ColorSet& set)
    {
        const_iterator it = std::find (m_sets.begin (), m_sets.end (), set);
        if (it == m_sets.end ())
        {
            m_sets.push_back (set);
        }
    }

    void ColorSetManager::remove_set (const ColorSet& set)
    {
        iterator it = std::find (m_sets.begin (), m_sets.end (), set);
        if (it != m_sets.end ())
        {
            m_sets.erase (it);
        }
    }

    void ColorSetManager::clear ()
    { m_sets.clear (); }

    ColorSetManager::iterator
        ColorSetManager::begin ()
        { return m_sets.begin (); }

    ColorSetManager::const_iterator
        ColorSetManager::begin () const
        { return m_sets.begin (); }

    ColorSetManager::iterator
        ColorSetManager::end ()
        { return m_sets.end (); }

    ColorSetManager::const_iterator
        ColorSetManager::end () const
        { return m_sets.end (); }

    ColorSetManager::reverse_iterator
        ColorSetManager::rbegin ()
        { return m_sets.rbegin (); }

    ColorSetManager::const_reverse_iterator
        ColorSetManager::rbegin () const
        { return m_sets.rbegin (); }

    ColorSetManager::reverse_iterator
        ColorSetManager::rend ()
        { return m_sets.rend (); }

    ColorSetManager::const_reverse_iterator
        ColorSetManager::rend () const
        { return m_sets.rend (); }
}
