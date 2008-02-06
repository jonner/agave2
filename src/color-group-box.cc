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
#include <glibmm-utils/exception.h>
#include "color-group-box.h"
#include "color-edit-box.h"
#include "color-relation.h"

namespace agave
{

    struct BuildColorVector
    {
        std::vector<boost::shared_ptr<ColorModel> > m_colors;

        void operator () (const boost::shared_ptr<ColorEditBox>& edit_box)
        {
            m_colors.push_back (edit_box->get_model ());
        }
    };

    struct ColorGroupBox::Priv
    {
        std::vector<boost::shared_ptr<ColorEditBox> > m_edit_boxes;

        Priv ()
        {
        }
    };

    ColorGroupBox::ColorGroupBox() :
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

    void ColorGroupBox::add_color (const boost::shared_ptr<ColorModel>& model, bool highlight)
    {
        THROW_IF_FAIL (m_priv);
        boost::shared_ptr<ColorEditBox> edit_box (new ColorEditBox (model));
        THROW_IF_FAIL (edit_box);
        if (highlight)
        {
            // highlight this box with a thicker border width around the swatch
            edit_box->set_border_width (3.0);
        }
        m_priv->m_edit_boxes.push_back (edit_box);
        edit_box->show ();
        pack_start (*edit_box);
    }

    unsigned int ColorGroupBox::get_num_colors () const
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_edit_boxes.size ();
    }
}
