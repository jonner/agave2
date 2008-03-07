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
#ifndef __COLOR_SET_DETAILS_EDITOR_H
#define __COLOR_SET_DETAILS_EDITOR_H

#include <boost/shared_ptr.hpp>
#include "color-set.h"

namespace agave
{
    class ColorSetDetailsEditor
    {
        public:
            ColorSetDetailsEditor (ColorSet& set);
            int run ();

        private:
            class Priv;
            boost::shared_ptr<Priv> m_priv;
    };
}

#endif // __COLOR_SET_DETAILS_EDITOR_H
