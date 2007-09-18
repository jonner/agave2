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
#ifndef __SCHEME_H
#define __SCHEME_H

#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "color-relation.h"

namespace agave
{
    class Scheme
    {
        public:
            Scheme (const Glib::ustring& name,
                    color_gen_func outer_left,
                    color_gen_func inner_left,
                    color_gen_func inner_right,
                    color_gen_func outer_right);

            Glib::ustring get_name () const {return m_name;}
            color_gen_func get_outer_left () {return m_outer_left_gen;}
            color_gen_func get_inner_left () {return m_inner_left_gen;}
            color_gen_func get_inner_right () {return m_inner_right_gen;}
            color_gen_func get_outer_right () {return m_outer_right_gen;}

        private:
            Glib::ustring m_name;
            color_gen_func m_outer_left_gen;
            color_gen_func m_inner_left_gen;
            color_gen_func m_inner_right_gen;
            color_gen_func m_outer_right_gen;
    };
}

#endif // __SCHEME_H
