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
#include <glibmm-utils/exception.hh>
#include "color-scheme-box.h"
#include "color-edit-box.h"
#include "color-relation.h"

namespace agave
{
    struct ColorSchemeBox::Priv
    {
        boost::shared_ptr<IScheme> m_scheme;
        std::vector<boost::shared_ptr<ColorEditBox> > m_edit_boxes;
        std::vector<boost::shared_ptr<ColorRelation> > m_relations;
        boost::shared_ptr<ColorEditBox> m_base_color;
        static const int NUM_COLORS = 5;

        Priv ()
        {
            for (int i = 0; i < NUM_COLORS; ++i)
            {
                m_edit_boxes.push_back (
                        boost::shared_ptr<ColorEditBox>(new ColorEditBox ()));
            }

            // color #2 is the control color, so highlight it with a thicker
            // border
            m_base_color = m_edit_boxes[2];
            m_base_color->get_swatch ().set_border_width (2.0);
        }

        void set_scheme (const boost::shared_ptr<IScheme>& scheme)
        {
            THROW_IF_FAIL (m_base_color);
            THROW_IF_FAIL (m_edit_boxes.size () == 5);
            // get rid of the old scheme's relations, if any
            m_relations.clear ();

            boost::shared_ptr<ColorRelation> relation;
            relation.reset (new
                    ColorRelation(m_base_color->get_model (),
                        m_edit_boxes[0]->get_model (),
                        scheme->get_outer_left ()));
            m_relations.push_back (relation);

            relation.reset (new
                    ColorRelation(m_base_color->get_model (),
                        m_edit_boxes[1]->get_model (),
                        scheme->get_inner_left ()));
            m_relations.push_back (relation);

            relation.reset (new
                    ColorRelation(m_base_color->get_model (),
                        m_edit_boxes[3]->get_model (),
                        scheme->get_inner_right ()));
            m_relations.push_back (relation);

            relation.reset (new
                    ColorRelation(m_base_color->get_model (),
                        m_edit_boxes[4]->get_model (),
                        scheme->get_outer_right ()));
            m_relations.push_back (relation);
        }

        void set_base_color (const Color& c)
        {
            THROW_IF_FAIL (m_base_color);
            m_base_color->get_model ()->set_color (c);
        }
    };

    ColorSchemeBox::ColorSchemeBox() :
        m_priv (new Priv ())
    {
        THROW_IF_FAIL (m_priv);
        typedef std::vector<boost::shared_ptr<ColorEditBox> >::const_iterator edit_iter_t;
        for (edit_iter_t iter = m_priv->m_edit_boxes.begin ();
                iter != m_priv->m_edit_boxes.end (); ++iter)
        {
            pack_start (**iter);
        }
        set_spacing (6);
    }

    void ColorSchemeBox::set_scheme (const boost::shared_ptr<IScheme> & scheme)
    {
        THROW_IF_FAIL (m_priv);
        m_priv->set_scheme (scheme);
    }

    void ColorSchemeBox::set_base_color (const Color& c)
    {
        THROW_IF_FAIL (m_priv);
        m_priv->set_base_color (c);
    }
}
