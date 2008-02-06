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
#include "rgb-scale-set.h"
#include "colorscale.h"

namespace agave
{
    struct RgbScaleSet::Priv
    {
        boost::shared_ptr<ColorModel> m_model;
        ColorScale m_scale_r, m_scale_g, m_scale_b;
        Priv (const boost::shared_ptr<ColorModel>& model) :
            m_model (model),
            m_scale_r (m_model, ColorScale::CHANNEL_RED),
            m_scale_g (m_model, ColorScale::CHANNEL_GREEN),
            m_scale_b (m_model, ColorScale::CHANNEL_BLUE)
        {
        }

        void set_model(const boost::shared_ptr<ColorModel>& model)
        {
            m_model = model;
            m_scale_r.set_model (m_model);
            m_scale_g.set_model (m_model);
            m_scale_b.set_model (m_model);
        }
    };

    RgbScaleSet::RgbScaleSet (const boost::shared_ptr<ColorModel>& model) :
        m_priv (new Priv (model))
    {
        THROW_IF_FAIL (m_priv);
        set_spacing (3);
        set_border_width (3);
        pack_start (m_priv->m_scale_r.get_widget (), Gtk::PACK_SHRINK);
        pack_start (m_priv->m_scale_g.get_widget (), Gtk::PACK_SHRINK);
        pack_start (m_priv->m_scale_b.get_widget (), Gtk::PACK_SHRINK);
    }

    void RgbScaleSet::set_model (const boost::shared_ptr<ColorModel>& model)
    {
        THROW_IF_FAIL (m_priv);
        m_priv->set_model (model);
    }

    boost::shared_ptr<ColorModel> RgbScaleSet::get_model ()
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_model;
    }

}
