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
#ifndef __APPLICATION_H
#define __APPLICATION_H

#include <boost/shared_ptr.hpp>

namespace agave
{
    class Application
    {
        public:
            Application (int argc, char** argv);
            ~Application ();
            int run ();

        private:
            struct Priv;
            boost::shared_ptr<Priv> m_priv;
    };
}

#endif // __APPLICATION_H
