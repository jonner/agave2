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
#ifndef __COLOR_RELATION_H
#define __COLOR_RELATION_H

#include <sigc++/functors/slot.h>
#include "color.h"
#include "colormodel.h"

namespace agave
{
    /// A functor type for generating one color from another one
    typedef Color (*color_gen_func)(const Color&);

    Color generate_identity (const Color& src);

    class ColorRelation
    {
        public:
            typedef sigc::slot<Color, const Color&> SlotColorGen;
            ColorRelation (ColorModel::pointer src, ColorModel::pointer dest, const SlotColorGen& slot = sigc::ptr_fun(&generate_identity));
            void connect (ColorModel::pointer src, ColorModel::pointer dest, const SlotColorGen& slot = sigc::ptr_fun(&generate_identity));
            void set_generator (const SlotColorGen& slot);

        private:
            void on_source_color_changed ();
            void on_dest_color_changed ();
            ColorModel::pointer m_source;
            ColorModel::pointer m_dest;
            SlotColorGen m_generator;
            hsv_t m_local_offset;
    };
}

#endif // __COLOR_RELATION_H
