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
#ifndef __SCHEME_MANAGER_H
#define __SCHEME_MANAGER_H

#include <vector>
#include "i-scheme.h"

namespace agave
{
    class SchemeManager
    {
        public:
            static SchemeManager& instance ();
            const std::vector<boost::shared_ptr<IScheme> >& get_schemes ();

        private:
            SchemeManager ();
            virtual ~SchemeManager ();

            // not copyable
            SchemeManager (SchemeManager& other);
            SchemeManager& operator= (SchemeManager& other);

            std::vector<boost::shared_ptr<IScheme> > m_schemes;
            static SchemeManager* s_instance;
    };
}

#endif // __SCHEME_MANAGER_H
