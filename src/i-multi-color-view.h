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
#ifndef __I_SCHEME_VIEW_H
#define __I_SCHEME_VIEW_H

#include <vector>
#include <boost/shared_ptr.hpp>

namespace agave
{
    class ColorModel;

    class IMultiColorView
    {
        public:
            virtual ~IMultiColorView() {}
            virtual void add_color (const boost::shared_ptr<ColorModel>& model, bool highlight) = 0;
            virtual unsigned int get_num_colors () const = 0;
    };
} // namespace agave

#endif // __I_SCHEME_VIEW_H
