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
#include "color-relation.h"
#include <sigc++/trackable.h>
#include <sigc++/connection.h>

namespace agave
{
    Color generate_identity (const Color& src) { return src; };

    struct ColorRelation::Priv : public sigc::trackable
    {
        boost::shared_ptr<ColorModel> m_source;
        boost::shared_ptr<ColorModel> m_dest;
        SlotColorGen m_generator;
        hsv_t m_local_offset;
        sigc::connection m_source_connection, m_dest_connection;

        Priv (
                boost::shared_ptr<ColorModel> src,
                boost::shared_ptr<ColorModel> dest,
                const SlotColorGen& slot)
        {
            m_local_offset.h = 0.0;
            m_local_offset.s = 0.0;
            m_local_offset.v = 0.0;
            m_local_offset.a = 1.0;
            connect (src, dest, slot);
        }

        void connect (
                boost::shared_ptr<ColorModel> src,
                boost::shared_ptr<ColorModel> dest,
                const SlotColorGen& slot)
        {
            m_source_connection.disconnect ();
            m_dest_connection.disconnect ();
            m_source = src;
            m_dest = dest;
            m_generator = slot;
            if (m_source)
            {
                m_source_connection = m_source->signal_color_changed ().connect
                    (sigc::mem_fun (this,
                                    &ColorRelation::Priv::on_source_color_changed));
            }
            if (m_dest)
            {
                m_dest_connection = m_dest->signal_color_changed ().connect
                    (sigc::mem_fun (this,
                                    &ColorRelation::Priv::on_dest_color_changed));
            }

            // call once immediately upon connecting so that the current state
            // of the two colors matches the relationship that we just defined.
            on_source_color_changed ();
        }

        void on_source_color_changed ()
        {
            if (m_generator)
            {
                m_dest->set_color (m_generator (
                            Color (m_source->get_color ().as_hsv () + m_local_offset)));
            }
        }

        void on_dest_color_changed ()
        {
            if (m_generator)
            {
                Color expected = m_generator (m_source->get_color ());
                m_local_offset = (m_dest->get_color ().as_hsv ()) - expected.as_hsv ();
            }
        }
    };


    ColorRelation::ColorRelation(boost::shared_ptr<ColorModel> src,
                                 boost::shared_ptr<ColorModel> dest,
                                 const SlotColorGen& slot) :
        m_priv (new Priv (src, dest, slot))
    {
    }

}
