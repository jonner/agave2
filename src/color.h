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
#ifndef __COLOR_H
#define __COLOR_H

#include <iostream>
#include <glibmm/ustring.h>

namespace agave
{
    /************************************************************
     * RGB
     ***********************************************************/
    struct rgb_t
    {
        typedef double value_type;
        value_type r, g, b, a;
        value_type max(void) const;
        value_type min(void) const;
    };

    bool operator==(const rgb_t& lhs, const rgb_t& rhs);

    struct rgb_int_t
    {
        typedef unsigned int value_type;
        value_type r, g, b, a;
    };

    /************************************************************
     * HSV
     ***********************************************************/
    struct hsv_t
    {
        typedef double value_type;
        value_type h, s, v, a;
    };

    struct hsv_int_t
    {
        typedef unsigned int value_type;
        value_type h, s, v, a;
    };

    /************************************************************
     * HSL
     ***********************************************************/
    struct hsl_t
    {
        typedef double value_type;
        value_type h, s, l, a;
    };

    struct hsl_int_t
    {
        typedef unsigned int value_type;
        value_type h, s, l, a;
    };

    /************************************************************
     * CMYK
     ***********************************************************/
    struct cmyk_t
    {
        typedef double value_type;
        value_type c, m, y, k, a;
    };

    struct cmyk_int_t
    {
        typedef unsigned int value_type;
        value_type c, m, y, k, a;
    };


    /**
     * A class to encapsulate the specification of a color.
     *
     * You can set the color using a specification of a color in a variety of
     * different colorspaces, including RGB, HSV, HSL, or CMYK.  In addition,
     * you can set a color from a string representation (as used in HTML and CSS
     * -- e.g. #FFFFFF)
     *
     * In addition, this class provides several utility functions to convert
     * between these colorspaces. 
     *
     * @author Jonathon Jongsma
     */
    class Color
    {
        public:
            Color(void);
            Color(rgb_t rgb);
            Color(double r, double g, double b, double a = 1.0);
            Color(hsv_t hsv);
            Color(hsl_t hsl);
            Color(cmyk_t cmyk);
            Color(Glib::ustring hexstring);
            Color(const Color& other);
            virtual ~Color(void);

            Color& operator=(const Color& rhs);
            bool operator==(const Color& rhs) const;
            bool operator!=(const Color& rhs) const;
            Color operator-(const Color& rhs) const;
            Color operator+(const Color& rhs) const;
            Color operator*(const Color& rhs) const;
            Color operator*(double factor) const;

            friend std::ostream& operator<<(std::ostream& out, const Color& c);

            /**
             * Set the value of the color with a RGBA specification
             *
             * @param rgb   A color in RGBA colorspace
             */
            void set(rgb_t rgb);

            /**
             * set the value of the color with an HSV(A) specification
             *
             * @param hsv   A color in HSVA colorspace
             */
            void set(hsv_t hsv);

            /**
             * Set the value of the color with an HSL(A) specification
             *
             * @param hsl   A color in HSLA colorspace
             */
            void set(hsl_t hsl);

            /**
             * Set the value of the color with a CMYK(A) specification
             *
             * @param cmyk  A color in CMYKA colorspace
             */
            void set(cmyk_t cmyk);

            /**
             * Set the value of the color using a hexstring, as in #FFFFFF
             *
             * @param hexstring A color specification in a RGB hexstring format
             */
            void set(Glib::ustring hexstring);

            /**
             * Get value of the color in RGBA format
             */
            rgb_t as_rgb(void) const;

            /**
             * Get value of the color in HSV(A) format
             */
            hsv_t as_hsv(void) const;

            /**
             * Get value of the color in HSL(A) format
             */
            hsl_t as_hsl(void) const;

            /**
             * Get value of the color in CMYK(A) format
             */
            cmyk_t as_cmyk(void) const;

            /**
             * Get value of the color in hexstring format
             */
            Glib::ustring as_hexstring(void) const;

            /**
             * Get the luminance of the color
             */
            inline double luminance(void) const
            {
                return (m_data.r * m_red_luminance +
                        m_data.g * m_green_luminance +
                        m_data.b * m_blue_luminance);
            }

            /**
             * Convert a color specification in RGB colorspace to one in HSV
             * colorspace
             *
             * @param rgb   A color in RGBA colorspace
             * @param hsv   A color in HSVA colorspace
             */
            static void rgb_to_hsv(const rgb_t& rgb, hsv_t& hsv);

            /**
             * Convert a color specification in RGB colorspace to one in HSL
             * colorspace
             *
             * @param rgb   A color in RGBA colorspace
             * @param hsl   A color in HSLA colorspace
             */
            static void rgb_to_hsl(const rgb_t& rgb, hsl_t& hsl);

            /**
             * Convert a color specification in RGB colorspace to one in CMYK
             * colorspace
             *
             * @param rgb   A color in RGBA colorspace
             * @param cmyk  A color in CMYKA colorspace
             */
            static void rgb_to_cmyk(const rgb_t& rgb, cmyk_t& cmyk);

            /**
             * Convert a color specification in HSV colorspace to one in RGB
             * colorspace
             *
             * @param hsv   A color in HSVA colorspace
             * @param rgb   A color in RGBA colorspace
             */
            static void hsv_to_rgb(const hsv_t& hsv, rgb_t& rgb);

            /**
             * Convert a color specification in HSL colorspace to one in RGB
             * colorspace
             *
             * @param hsl   A color in HSLA colorspace
             * @param rgb   A color in RGBA colorspace
             */
            static void hsl_to_rgb(const hsl_t& hsl, rgb_t& rgb);

            /**
             * Convert a color specification in CMYK colorspace to one in RGB
             * colorspace
             *
             * @param cmyk  A color in CMYKA colorspace
             * @param rgb   A color in RGBA colorspace
             */
            static void cmyk_to_rgb(const cmyk_t& cmyk, rgb_t& rgb);

        private:
            void clamp();

            /** Internal data representation is RGBA.  This is converted to
             * other colorspaces as needed
             */
            rgb_t m_data;

            // constants for determining the luminance of a particular color
            // borrowed from the GIMP
            static const double m_red_luminance;
            static const double m_green_luminance;
            static const double m_blue_luminance;
    };

} // namespace agave
#endif // __COLOR_H
