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

namespace agave
{
    struct ColorEditBox::Priv : public Gtk::VBox
    {
        boost::shared_ptr<ColorModel> m_model;
        Swatch m_swatch;
        Gtk::Notebook m_notebook;
        Gtk::VBox m_hsv_box, m_rgb_box;
        ColorScale m_scale_r, m_scale_g, m_scale_b;
        ColorScale m_scale_h, m_scale_s, m_scale_v;
        Priv () :
            m_model (new ColorModel ()),
            m_swatch (m_model),
            m_scale_r (m_model, ColorScale::CHANNEL_RED),
            m_scale_g (m_model, ColorScale::CHANNEL_GREEN),
            m_scale_b (m_model, ColorScale::CHANNEL_BLUE),
            m_scale_h (m_model, ColorScale::CHANNEL_HUE),
            m_scale_s (m_model, ColorScale::CHANNEL_SATURATION),
            m_scale_v (m_model, ColorScale::CHANNEL_VALUE)
        {
            m_hsv_box.set_spacing (3);
            m_hsv_box.set_border_width (3);
            m_hsv_box.pack_start (m_scale_h.get_widget (), Gtk::PACK_SHRINK);
            m_hsv_box.pack_start (m_scale_s.get_widget (), Gtk::PACK_SHRINK);
            m_hsv_box.pack_start (m_scale_v.get_widget (), Gtk::PACK_SHRINK);
            m_notebook.append_page (m_hsv_box, "HSV", "Specify the color in HSV Colorspace");

            m_rgb_box.set_spacing (3);
            m_rgb_box.set_border_width (3);
            m_rgb_box.pack_start (m_scale_r.get_widget (), Gtk::PACK_SHRINK);
            m_rgb_box.pack_start (m_scale_g.get_widget (), Gtk::PACK_SHRINK);
            m_rgb_box.pack_start (m_scale_b.get_widget (), Gtk::PACK_SHRINK);
            m_notebook.append_page (m_rgb_box, "RGB", "Specify the color in RGB Colorspace");

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

    boost::shared_ptr<const ColorModel> ColorEditBox::get_model () const
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_model;
    }

    Swatch& ColorEditBox::get_swatch ()
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_swatch;
    }

    ColorScale& ColorEditBox::get_red_scale ()
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_scale_r;
    }

    ColorScale& ColorEditBox::get_green_scale ()
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_scale_g;
    }

    ColorScale& ColorEditBox::get_blue_scale ()
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_scale_b;
    }

    ColorScale& ColorEditBox::get_hue_scale ()
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_scale_h;
    }

    ColorScale& ColorEditBox::get_saturation_scale ()
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_scale_s;
    }

    ColorScale& ColorEditBox::get_value_scale ()
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_scale_v;
    }
}
