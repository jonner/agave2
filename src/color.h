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
#ifndef __COLOR_H
#define __COLOR_H

#include <iostream>
#include <glibmm/ustring.h>
#include <sigc++/trackable.h>
#include <sigc++/signal.h>
#include <boost/shared_ptr.hpp>

namespace agave
{
    /************************************************************
     * RGB
     ***********************************************************/
    struct rgb_t
    {
        typedef double value_type;
        value_type r, g, b, a;
        value_type max () const;
        value_type min () const;
    };

    bool operator==(const rgb_t& lhs, const rgb_t& rhs);

    /************************************************************
     * HSV
     ***********************************************************/
    struct hsv_t
    {
        typedef double value_type;
        value_type h, s, v, a;
    };

    hsv_t operator-(const hsv_t& lhs, const hsv_t& rhs);
    hsv_t operator+(const hsv_t& lhs, const hsv_t& rhs);

    /************************************************************
     * HSL
     ***********************************************************/
    struct hsl_t
    {
        typedef double value_type;
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
    class Color : public sigc::trackable
    {
        public:
            Color ();
            Color (double r, double g, double b, double a = 1.0);
            explicit Color (rgb_t rgb);
            explicit Color (hsv_t hsv);
            explicit Color (hsl_t hsl);
            explicit Color (cmyk_t cmyk);
            explicit Color (std::string hexstring);
            Color (const Color& other);
            virtual ~Color ();

            Color& operator=(const Color& rhs);
            bool operator==(const Color& rhs) const;
            bool operator!=(const Color& rhs) const;
            Color operator-(const Color& rhs) const;
            Color operator+(const Color& rhs) const;
            Color operator*(const Color& rhs) const;
            Color operator*(double factor) const;

            friend std::ostream& operator<<(std::ostream& out, const Color& c);

            /// \name rgb operations
            /// @{
            /**
             * Set the value of the color with a RGBA specification
             */
            void set (rgb_t rgb);
            void set_rgb (double r, double g, double b, double a=1.0);
            void set_red (double r);
            void set_green (double g);
            void set_blue (double b);
            double get_red () const;
            double get_green () const;
            double get_blue () const;
            /**
             * Get value of the color in RGBA format
             */
            rgb_t as_rgb () const;
            /// @}

            /// \name hsv operations
            /// @{
            /**
             * set the value of the color with an HSV(A) specification
             */
            void set (hsv_t hsv);
            void set_hsv (double h, double s, double v, double a=1.0);
            void set_hue (double h);
            void shift_hue (hsv_t::value_type hue_delta);
            void set_saturation (double s);
            void set_value (double v);
            double get_hue () const;
            double get_saturation () const;
            double get_value () const;
            /**
             * Get value of the color in HSV(A) format
             */
            hsv_t as_hsv () const;
            /// @}

            /// \name hsl operations
            /// @{
            /**
             * Set the value of the color with an HSL(A) specification
             */
            void set (hsl_t hsl);
            void set_hsl (double h, double s, double l, double a=1.0);
            /**
             * Get value of the color in HSL(A) format
             */
            hsl_t as_hsl () const;
            /// @}

            /// \name cmyk operations
            /// @{
            /**
             * Set the value of the color with a CMYK(A) specification
             */
            void set (cmyk_t cmyk);
            void set_cmyk (double c, double m, double y, double k, double a=1.0);
            void set_cyan (double c);
            void set_magenta (double m);
            void set_yellow (double y);
            void set_black (double k);
            /**
             * Get value of the color in CMYK(A) format
             */
            cmyk_t as_cmyk () const;
            double get_cyan () const;
            double get_magenta () const;
            double get_yellow () const;
            double get_black () const;
            /// @}

            /**
             * Set the value of the color using a hexstring, as in #FFFFFF
             *
             * @param hexstring A color specification in a RGB hexstring format
             */
            void set (std::string hexstring);

            /**
             * Get value of the color in hexstring format
             */
            std::string as_hexstring () const;

            void set_alpha (double a);
            double get_alpha () const;

            /**
             * Get the luminance of the color
             */
            double luminance () const;

            /**
             * signal emitted whenever the color is changed
             */
            sigc::signal<void>& signal_changed () const;

            /// \name Conversion helper functions
            /// @{
            /**
             * Convert a color specification in RGB colorspace to one in HSV
             * colorspace
             *
             * @param rgb   A color in RGBA colorspace
             * @return   A color in HSVA colorspace
             */
            static hsv_t rgb_to_hsv (const rgb_t& rgb);

            /**
             * Convert a color specification in RGB colorspace to one in HSL
             * colorspace
             *
             * @param rgb   A color in RGBA colorspace
             * @return   A color in HSLA colorspace
             */
            static hsl_t rgb_to_hsl (const rgb_t& rgb);

            /**
             * Convert a color specification in RGB colorspace to one in CMYK
             * colorspace
             *
             * @param rgb   A color in RGBA colorspace
             * @param pullout  A scaling value (0-1) indicating how much black
             * should be pulled out
             * @return  A color in CMYKA colorspace
             */
            static cmyk_t rgb_to_cmyk (const rgb_t& rgb, double pullout = 0.0);

            /**
             * Convert a color specification in HSV colorspace to one in RGB
             * colorspace
             *
             * @param hsv   A color in HSVA colorspace
             * @return   A color in RGBA colorspace
             */
            static rgb_t hsv_to_rgb (const hsv_t& hsv);

            /**
             * Convert a color specification in HSL colorspace to one in RGB
             * colorspace
             *
             * @param hsl   A color in HSLA colorspace
             * @return   A color in RGBA colorspace
             */
            static rgb_t hsl_to_rgb (const hsl_t& hsl);

            /**
             * Convert a color specification in CMYK colorspace to one in RGB
             * colorspace
             *
             * @param cmyk  A color in CMYKA colorspace
             * @return   A color in RGBA colorspace
             */
            static rgb_t cmyk_to_rgb (const cmyk_t& cmyk);
            /// @}

        private:
            struct Priv;
            boost::shared_ptr<Priv> m_priv;
    };

} // namespace agave
#endif // __COLOR_H
