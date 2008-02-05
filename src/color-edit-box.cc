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
#include <gtkmm/notebook.h>
#include <glibmm-utils/exception.h>
#include "color-edit-box.h"
#include "colormodel.h"
#include "rgb-scale-set.h"
#include "hsv-scale-set.h"

namespace agave
{
    struct ColorEditBox::Priv : public Gtk::VBox
    {
        boost::shared_ptr<ColorModel> m_model;
        Swatch m_swatch;
        Gtk::Notebook m_notebook;
        RgbScaleSet m_rgb_scales;
        HsvScaleSet m_hsv_scales;
        Priv () :
            m_model (new ColorModel ()),
            m_swatch (m_model),
            m_rgb_scales (m_model),
            m_hsv_scales (m_model)
        {
            m_notebook.append_page (m_hsv_scales, "HSV", "Specify the color in HSV Colorspace");
            m_notebook.append_page (m_rgb_scales, "RGB", "Specify the color in RGB Colorspace");

            m_swatch.set_size_request (100, 100);
        }
    };

    ColorEditBox::ColorEditBox () :
        m_priv (new Priv ())
    {
        THROW_IF_FAIL (m_priv);
        pack_start (m_priv->m_swatch);
        pack_start (m_priv->m_notebook, Gtk::PACK_SHRINK);
        set_spacing (6);
    }

    void ColorEditBox::set_color (const Color& c)
    {
        THROW_IF_FAIL (m_priv && m_priv->m_model);
        m_priv->m_model->set_color (c);
    }

    boost::shared_ptr<ColorModel> ColorEditBox::get_model ()
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_model;
    }

    Swatch& ColorEditBox::get_swatch ()
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_swatch;
    }

}
