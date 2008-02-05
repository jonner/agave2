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
#include "hsv-scale-set.h"

namespace agave
{
    struct HsvScaleSet::Priv
    {
        boost::shared_ptr<ColorModel> m_model;
        ColorScale m_scale_h, m_scale_s, m_scale_v;
        Priv (const boost::shared_ptr<ColorModel>& model) :
            m_model (model),
            m_scale_h (m_model, ColorScale::CHANNEL_HUE),
            m_scale_s (m_model, ColorScale::CHANNEL_SATURATION),
            m_scale_v (m_model, ColorScale::CHANNEL_VALUE)
        {
        }

        void set_model(const boost::shared_ptr<ColorModel>& model)
        {
            m_model = model;
            m_scale_h.set_model (m_model);
            m_scale_s.set_model (m_model);
            m_scale_v.set_model (m_model);
        }
    };

    HsvScaleSet::HsvScaleSet (const boost::shared_ptr<ColorModel>& model) :
        m_priv (new Priv (model))
    {
        THROW_IF_FAIL (m_priv);
        set_spacing (3);
        set_border_width (3);
        pack_start (m_priv->m_scale_h.get_widget (), Gtk::PACK_SHRINK);
        pack_start (m_priv->m_scale_s.get_widget (), Gtk::PACK_SHRINK);
        pack_start (m_priv->m_scale_v.get_widget (), Gtk::PACK_SHRINK);
    }

    void HsvScaleSet::set_model (const boost::shared_ptr<ColorModel>& model)
    {
        THROW_IF_FAIL (m_priv);
        m_priv->set_model (model);
    }

    boost::shared_ptr<ColorModel> HsvScaleSet::get_model ()
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_model;
    }

}
