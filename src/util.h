/*******************************************************************************
 *  PROJECT: Agave
 *
 *  AUTHOR: Jonathon Jongsma
 *
 *  Copyright (c) 2007 Jonathon Jongsma
 *
 *  License:
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the
 *    Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 *    Boston, MA  02111-1307  USA
 *
 *******************************************************************************/
#ifndef __UTIL_H
#define __UTIL_H

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <glibmm/ustring.h>

#ifndef HERE
#define HERE __ASSERT_FUNCTION << ":" <<__FILE__<< ":" << __LINE__ << ":"
#endif

#ifndef LOG_MARKER_INFO
#define LOG_MARKER_INFO "|I|"
#endif

#ifndef LOG_MARKER_ERROR
#define LOG_MARKER_ERROR "|E|"
#endif

#ifndef LOG_MARKER_EXCEPTION
#define LOG_MARKER_EXCEPTION "|X|"
#endif

#ifndef LOG_EXCEPTION
#define LOG_EXCEPTION(message) \
    std::cerr << LOG_MARKER_EXCEPTION << HERE << message << std::endl
#endif

#define THROW_IF_FAIL(cond) \
if (!(cond)) { \
LOG_EXCEPTION ("condition (" << #cond << ") failed; raising exception\n" ) ;\
throw std::logic_error \
    (Glib::ustring ("Assertion failed: ") + #cond)  ;\
}

#endif // __UTIL_H
