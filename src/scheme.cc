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

#include <map>
#include <glibmm-utils/exception.hh>
#include "scheme.h"
#include "color.h"
#include "scheme-generators.h"

namespace agave
{

    static ISchemeGenerator* get_generator (scheme_type_t scheme_type)
    {
        static std::map<scheme_type_t, ISchemeGenerator*> generator_map;
        static bool initialized = false;
        if (!initialized)
        {
            generator_map[SCHEME_TYPE_ANALOGOUS] = new AnalogousSchemeGenerator ();
            generator_map[SCHEME_TYPE_MONOCHROMATIC] = new MonochromaticSchemeGenerator ();
            generator_map[SCHEME_TYPE_TRIAD] = new TriadSchemeGenerator ();
            generator_map[SCHEME_TYPE_COMPLEMENTARY] = new ComplementarySchemeGenerator ();
            generator_map[SCHEME_TYPE_COMPOUND] = new CompoundSchemeGenerator ();
            generator_map[SCHEME_TYPE_SHADES] = new ShadesSchemeGenerator ();
            generator_map[SCHEME_TYPE_TETRAD] = new TetradSchemeGenerator ();
            generator_map[SCHEME_TYPE_CUSTOM] = new CustomSchemeGenerator ();
            initialized = true;
        }
        return generator_map[scheme_type];
    }

    struct Scheme::Priv
    {
        std::vector<Color> m_colors;
        Color m_base_color;
        std::vector<Color>::const_iterator m_base_color_iter;
        ISchemeGenerator* m_generator;
        scheme_type_t m_scheme_type;

        Priv () :
            m_base_color_iter (m_colors.begin ()),
            m_generator (0)
        {
        }
    };

    Scheme::Scheme () :
        m_priv (new Priv())
    {
    }

    Scheme::Scheme (const Color& base_color, scheme_type_t scheme_type) :
        m_priv (new Priv())
    {
        set (base_color, scheme_type);
    }

    std::vector<Color>& Scheme::colors ()
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_colors;
    }

    const std::vector<Color>& Scheme::colors () const
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_colors;
    }

    void Scheme::set (const Color& base_color, scheme_type_t scheme_type)
    {
        THROW_IF_FAIL (m_priv);
        m_priv->m_scheme_type = scheme_type;
        m_priv->m_generator = get_generator (scheme_type);
        m_priv->m_base_color = base_color;
        if (m_priv->m_generator)
        {
            m_priv->m_base_color_iter = m_priv->m_generator->generate
                (base_color, m_priv->m_colors);
        }
    }

    void Scheme::set_base_color (const Color& color)
    {
        set (color, get_scheme_type ());
    }

    Color Scheme::get_base_color () const
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_base_color;
    }

    void Scheme::set_scheme_type (scheme_type_t scheme_type)
    {
        set (get_base_color (), scheme_type);
    }

    scheme_type_t Scheme::get_scheme_type () const
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_scheme_type;
    }

    std::ostream& operator<<(std::ostream& out, const Scheme& s)
    {
        THROW_IF_FAIL (s.m_priv);
        out << "<" << &s << "> {Scheme} " << s.m_priv->m_generator->scheme_name ();
        for (std::vector<Color>::const_iterator it = s.colors ().begin ();
                it != s.colors ().end (); ++it)
        {
            out << std::endl;
            out << "  " << *it;
            if (it == s.m_priv->m_base_color_iter)
            {
                out << "*";
            }
        }
        return out;
    }
}
