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
#include "colormodel.h"
#include <glibmm-utils/exception.h>

namespace agave
{

    struct ColorModel::Priv
    {
        mutable sigc::signal<void> m_signal_color_changed;
        Color m_color;

        Priv ()
        {
            m_color.signal_changed ().connect (sigc::mem_fun (this,
                        &Priv::on_color_changed));
        }

        void on_color_changed ()
        {
            m_signal_color_changed.emit ();
        }
    };

    ColorModel::ColorModel (const Color& c) :
        m_priv (new Priv ())
    {
        THROW_IF_FAIL (m_priv);
        set_color (c);
    }

    void ColorModel::set_color (const Color& c)
    {
        THROW_IF_FAIL (m_priv);
        m_priv->m_color = c;
        signal_color_changed ().emit ();
    }

    Color& ColorModel::get_color ()
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_color;
    }

    sigc::signal<void>& ColorModel::signal_color_changed () const
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_signal_color_changed;
    }

    boost::shared_ptr<ColorModel> ColorModel::create (const Color& c)
    {
        return boost::shared_ptr<ColorModel> (new ColorModel (c));
    }
}
