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
#include "color.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>

namespace agave
{
    const double Color::m_red_luminance = 0.2126;
    const double Color::m_green_luminance = 0.7152;
    const double Color::m_blue_luminance = 0.0722;
    static const double MAX_VALUE = 1.0;
    static const double MIN_VALUE = 0.0;

    rgb_t::value_type rgb_t::max () const
    {
        return std::max (
                std::max (std::max (r, g), std::max (g, b)),
                std::max (r, b));
    }

    rgb_t::value_type rgb_t::min () const
    {
        return std::min (
                std::min (std::min (r, g), std::min (g, b)),
                std::min (r, b));
    }

    hsv_t Color::rgb_to_hsv (const rgb_t& rgb)
    {
        hsv_t hsv;
        gdouble max, min, delta;

        max = rgb.max ();
        min = rgb.min ();

        hsv.v = max;
        delta = max - min;

        if (delta > 0.0001)
        {
            hsv.s = delta / max;

            if (rgb.r == max)
            {
                hsv.h = (rgb.g - rgb.b) / delta;
                if (hsv.h < 0.0)
                    hsv.h += 6.0;
            }
            else if (rgb.g == max)
            {
                hsv.h = 2.0 + (rgb.b - rgb.r) / delta;
            }
            else if (rgb.b == max)
            {
                hsv.h = 4.0 + (rgb.r - rgb.g) / delta;
            }

            hsv.h /= 6.0;
        }
        else
        {
            hsv.s = 0.0;
            hsv.h = 0.0;
        }

        hsv.a = rgb.a;

        return hsv;
    }

    rgb_t Color::hsv_to_rgb (const hsv_t& hsv)
    {
        rgb_t rgb;
        gint i;
        gdouble f, w, q, t;

        gdouble hue;

        if (hsv.s == 0.0)
        {
            rgb.r = hsv.v;
            rgb.g = hsv.v;
            rgb.b = hsv.v;
        }
        else
        {
            hue = hsv.h;

            if (hue == 1.0)
                hue = 0.0;

            hue *= 6.0;

            i = (gint) hue;
            f = hue - i;
            w = hsv.v * (1.0 - hsv.s);
            q = hsv.v * (1.0 - (hsv.s * f));
            t = hsv.v * (1.0 - (hsv.s * (1.0 - f)));

            switch (i)
            {
                case 0:
                    rgb.r = hsv.v;
                    rgb.g = t;
                    rgb.b = w;
                    break;
                case 1:
                    rgb.r = q;
                    rgb.g = hsv.v;
                    rgb.b = w;
                    break;
                case 2:
                    rgb.r = w;
                    rgb.g = hsv.v;
                    rgb.b = t;
                    break;
                case 3:
                    rgb.r = w;
                    rgb.g = q;
                    rgb.b = hsv.v;
                    break;
                case 4:
                    rgb.r = t;
                    rgb.g = w;
                    rgb.b = hsv.v;
                    break;
                case 5:
                    rgb.r = hsv.v;
                    rgb.g = w;
                    rgb.b = q;
                    break;
            }
        }

        rgb.a = hsv.a;

        return rgb;
    }

    hsl_t Color::rgb_to_hsl (const rgb_t& rgb)
    {
        hsl_t hsl;
        gdouble max, min, delta;

        max = rgb.max ();
        min = rgb.min ();

        hsl.l = (max + min) / 2.0;

        if (max == min)
        {
            hsl.s = 0.0;
            hsl.h = -1.0;   // undefined
        }
        else
        {
            if (hsl.l <= 0.5)
                hsl.s = (max - min) / (max + min);
            else
                hsl.s = (max - min) / (2.0 - max - min);

            delta = max - min;

            if (delta == 0.0)
                delta = 1.0;

            if (rgb.r == max)
            {
                hsl.h = (rgb.g - rgb.b) / delta;
            }
            else if (rgb.g == max)
            {
                hsl.h = 2.0 + (rgb.b - rgb.r) / delta;
            }
            else if (rgb.b == max)
            {
                hsl.h = 4.0 + (rgb.r - rgb.g) / delta;
            }

            hsl.h /= 6.0;

            if (hsl.h < 0.0)
                hsl.h += 1.0;
        }

        hsl.a = rgb.a;

        return hsl;
    }

    static gdouble
    hsl_value (gdouble n1, gdouble n2, gdouble hue)
    {
        gdouble val;

        if (hue > 6.0)
            hue -= 6.0;
        else if (hue < 0.0)
            hue += 6.0;

        if (hue < 1.0)
            val = n1 + (n2 - n1) * hue;
        else if (hue < 3.0)
            val = n2;
        else if (hue < 4.0)
            val = n1 + (n2 - n1) * (4.0 - hue);
        else
            val = n1;

        return val;
    }

    rgb_t Color::hsl_to_rgb (const hsl_t& hsl)
    {
        rgb_t rgb;

        if (hsl.s == 0)
        {
            /*  achromatic case  */
            rgb.r = hsl.l;
            rgb.g = hsl.l;
            rgb.b = hsl.l;
        }
        else
        {
            gdouble m1, m2;

            if (hsl.l <= 0.5)
                m2 = hsl.l * (1.0 + hsl.s);
            else
                m2 = hsl.l + hsl.s - hsl.l * hsl.s;

            m1 = 2.0 * hsl.l - m2;

            rgb.r = hsl_value (m1, m2, hsl.h * 6.0 + 2.0);
            rgb.g = hsl_value (m1, m2, hsl.h * 6.0);
            rgb.b = hsl_value (m1, m2, hsl.h * 6.0 - 2.0);
        }

        rgb.a = hsl.a;

        return rgb;
    }

    cmyk_t Color::rgb_to_cmyk (const rgb_t& rgb, double pullout)
    {
        cmyk_t cmyk;
        gdouble c, m, y, k;

        c = 1.0 - rgb.r;
        m = 1.0 - rgb.g;
        y = 1.0 - rgb.b;

        k = 1.0;
        if (c < k)  k = c;
        if (m < k)  k = m;
        if (y < k)  k = y;

        k *= pullout;

        if (k < 1.0)
        {
            cmyk.c = (c - k) / (1.0 - k);
            cmyk.m = (m - k) / (1.0 - k);
            cmyk.y = (y - k) / (1.0 - k);
        }
        else
        {
            cmyk.c = 0.0;
            cmyk.m = 0.0;
            cmyk.y = 0.0;
        }

        cmyk.k = k;
        cmyk.a = rgb.a;

        return cmyk;
    }

    rgb_t Color::cmyk_to_rgb (const cmyk_t& cmyk)
    {
        rgb_t rgb;
        gdouble c, m, y, k;

        k = cmyk.k;

        if (k < 1.0)
        {
            c = cmyk.c * (1.0 - k) + k;
            m = cmyk.m * (1.0 - k) + k;
            y = cmyk.y * (1.0 - k) + k;
        }
        else
        {
            c = 1.0;
            m = 1.0;
            y = 1.0;
        }

        rgb.r = 1.0 - c;
        rgb.g = 1.0 - m;
        rgb.b = 1.0 - y;
        rgb.a = cmyk.a;

        return rgb;
    }

    Color::Color ()
    {
        // initialize to opaque white
        set_hsv (0.0, 1.0, 1.0, 1.0);
    }

    Color::Color (rgb_t rgb)
    {
        set (rgb);
    }

    Color::Color (double r, double g, double b, double a)
    {
        set_rgb (r, g, b, a);
    }

    Color::Color (hsv_t hsv)
    {
        set (hsv);
    }

    Color::Color (hsl_t hsl)
    {
        set (hsl_to_rgb (hsl));
        clamp ();
    }

    Color::Color (cmyk_t cmyk)
    {
        set (cmyk_to_rgb (cmyk));
        clamp ();
    }

    Color::Color (std::string hexstring)
    {
        // FIXME
        clamp ();
    }

    Color::Color (const Color& other)
    {
        *this = other;
        clamp ();
    }

    Color::~Color ()
    {
    }

    Color& Color::operator=(const Color& rhs)
    {
        if (this == &rhs) return *this;
        m_rgb = rhs.m_rgb;
        m_hsv = rhs.m_hsv;
        clamp ();
        return *this;
    }

    bool operator==(const rgb_t& lhs, const rgb_t& rhs)
    {
        return (lhs.r == rhs.r &&
                lhs.g == rhs.g &&
                lhs.b == rhs.b &&
                lhs.a == rhs.a);
    }

    bool Color::operator==(const Color& rhs) const
    {
        return (m_rgb == rhs.m_rgb);
    }

    bool Color::operator!=(const Color& rhs) const
    {
        return !(*this == rhs);
    }

    Color Color::operator-(const Color& rhs) const
    {
        Color result;
        result.m_rgb.r = m_rgb.r - rhs.m_rgb.r;
        result.m_rgb.g = m_rgb.g - rhs.m_rgb.g;
        result.m_rgb.b = m_rgb.b - rhs.m_rgb.b;
        result.m_rgb.a = m_rgb.a;
        result.clamp ();
        return result;
    }

    Color Color::operator+(const Color& rhs) const
    {
        Color result;
        result.m_rgb.r = m_rgb.r + rhs.m_rgb.r;
        result.m_rgb.g = m_rgb.g + rhs.m_rgb.g;
        result.m_rgb.b = m_rgb.b + rhs.m_rgb.b;
        result.m_rgb.a = m_rgb.a;
        result.clamp ();
        return result;
    }

    Color Color::operator*(const Color& rhs) const
    {
        return *this;
    }

    Color Color::operator*(double factor) const
    {
        Color result;
        result.m_rgb.r = m_rgb.r * factor;
        result.m_rgb.g = m_rgb.g * factor;
        result.m_rgb.b = m_rgb.b * factor;
        result.m_rgb.a = m_rgb.a * factor;
        result.clamp ();
        return result;
    }

    void Color::clamp ()
    {
        // clamp rgb
        m_rgb.r = std::min (m_rgb.r, MAX_VALUE);
        m_rgb.g = std::min (m_rgb.g, MAX_VALUE);
        m_rgb.b = std::min (m_rgb.b, MAX_VALUE);
        m_rgb.a = std::min (m_rgb.a, MAX_VALUE);

        m_rgb.r = std::max (m_rgb.r, MIN_VALUE);
        m_rgb.g = std::max (m_rgb.g, MIN_VALUE);
        m_rgb.b = std::max (m_rgb.b, MIN_VALUE);
        m_rgb.a = std::max (m_rgb.a, MIN_VALUE);

        // clamp hsv
        m_hsv.h = std::min (m_hsv.h, MAX_VALUE);
        m_hsv.s = std::min (m_hsv.s, MAX_VALUE);
        m_hsv.v = std::min (m_hsv.v, MAX_VALUE);
        m_hsv.a = std::min (m_hsv.a, MAX_VALUE);

        m_hsv.h = std::max (m_hsv.h, MIN_VALUE);
        m_hsv.s = std::max (m_hsv.s, MIN_VALUE);
        m_hsv.v = std::max (m_hsv.v, MIN_VALUE);
        m_hsv.a = std::max (m_hsv.a, MIN_VALUE);
    }


    std::ostream& operator<< (std::ostream& out, const Color& c)
    {
        rgb_t rgb = c.as_rgb ();
        out << std::fixed << std::setprecision (4);
        out << "<" << &c << "> {Color} ";
        /*
        out << "[R=" << rgb.r
            << " G=" << rgb.g
            << " B=" << rgb.b
            << " A=" << rgb.a
            << "]";
        */

        hsv_t hsv = c.as_hsv ();
        out << c.as_hexstring ();
        out << " [H=" << hsv.h
            << " S=" << hsv.s
            << " V=" << hsv.v
            << " A=" << hsv.a
            << "]";
        return out;
    }


    void Color::shift_hue (hsv_t::value_type hue_delta)
    {
        hsv_t hsv = as_hsv ();
        hsv.h += hue_delta;
        if (hsv.h < 0)
        {
            hsv.h += 1.0;
        }
        set (hsv);
    }

    void Color::set (rgb_t rgb)
    {
        m_rgb = rgb;
        m_hsv = rgb_to_hsv (rgb);
        clamp ();
        m_signal_changed.emit ();
    }

    void Color::set_rgb (double r, double g, double b, double a)
    {
        rgb_t rgb = {r, g, b, a};
        set (rgb);
    }

    void Color::set_red (double r)
    {
        rgb_t new_rgb = m_rgb;
        new_rgb.r = r;
        set (new_rgb);
    }

    void Color::set_green (double g)
    {
        rgb_t new_rgb = m_rgb;
        new_rgb.g = g;
        set (new_rgb);
    }

    void Color::set_blue (double b)
    {
        rgb_t new_rgb = m_rgb;
        new_rgb.b = b;
        set (new_rgb);
    }

    double Color::get_red () const
    {
        return m_rgb.r;
    }

    double Color::get_green () const
    {
        return m_rgb.g;
    }

    double Color::get_blue () const
    {
        return m_rgb.b;
    }

    void Color::set (hsv_t hsv)
    {
        m_hsv = hsv;
        m_rgb = hsv_to_rgb (hsv);
        clamp ();
        m_signal_changed.emit ();
    }

    void Color::set_hsv (double h, double s, double v, double a)
    {
        hsv_t new_hsv = {h, s, v, a};
        set (new_hsv);
    }

    void Color::set_hue (double h)
    {
        hsv_t new_hsv = as_hsv ();
        new_hsv.h = h;
        set (new_hsv);
    }

    void Color::set_saturation (double s)
    {
        hsv_t new_hsv = as_hsv ();
        new_hsv.s = s;
        set (new_hsv);
    }

    void Color::set_value (double v)
    {
        hsv_t new_hsv = as_hsv ();
        new_hsv.v = v;
        set (new_hsv);
    }

    double Color::get_hue () const
    {
        hsv_t hsv = as_hsv ();
        return hsv.h;
    }

    double Color::get_saturation () const
    {
        hsv_t hsv = as_hsv ();
        return hsv.s;
    }

    double Color::get_value () const
    {
        hsv_t hsv = as_hsv ();
        return hsv.v;
    }

    void Color::set (hsl_t hsl)
    {
        set (hsl_to_rgb (hsl));
    }

    void Color::set (cmyk_t cmyk)
    {
        set (cmyk_to_rgb (cmyk));
    }

    void Color::set_cmyk (double c, double m, double y, double k, double a)
    {
        cmyk_t new_cmyk = {c, m, y, k, a};
        set (new_cmyk);
    }

    void Color::set_cyan (double c)
    {
        cmyk_t new_cmyk = as_cmyk ();
        new_cmyk.c = c;
        set (new_cmyk);
    }

    void Color::set_magenta (double m)
    {
        cmyk_t new_cmyk = as_cmyk ();
        new_cmyk.m = m;
        set (new_cmyk);
    }

    void Color::set_yellow (double y)
    {
        cmyk_t new_cmyk = as_cmyk ();
        new_cmyk.y = y;
        set (new_cmyk);
    }

    void Color::set_black (double k)
    {
        cmyk_t new_cmyk = as_cmyk ();
        new_cmyk.k = k;
        set (new_cmyk);
    }

    double Color::get_cyan () const
    {
        cmyk_t cmyk = as_cmyk ();
        return cmyk.c;
    }

    double Color::get_magenta () const
    {
        cmyk_t cmyk = as_cmyk ();
        return cmyk.m;
    }

    double Color::get_yellow () const
    {
        cmyk_t cmyk = as_cmyk ();
        return cmyk.y;
    }

    double Color::get_black () const
    {
        cmyk_t cmyk = as_cmyk ();
        return cmyk.k;
    }

    void Color::set_alpha (double a)
    {
        rgb_t new_rgb = m_rgb;
        new_rgb.a = a;
        set (new_rgb);
    }

    double Color::get_alpha () const
    {
        return m_rgb.a;
    }

    void Color::set (std::string hexstring)
    {
        // FIXME: parse string to rgb color
    }

    rgb_t Color::as_rgb () const
    {
        return m_rgb;
    }

    hsv_t Color::as_hsv () const
    {
        return m_hsv;
    }

    hsl_t Color::as_hsl () const
    {
        return rgb_to_hsl (m_rgb);
    }

    cmyk_t Color::as_cmyk () const
    {
        return rgb_to_cmyk (m_rgb);
    }

    std::string Color::as_hexstring () const
    {
        std::ostringstream ostream;
        uint16_t x;
        x = static_cast<uint16_t>((m_rgb.r) * static_cast<double>(255.0));
        ostream << std::hex << std::setw (2) << std::setfill('0') << x;
        x = static_cast<uint16_t>((m_rgb.g) * static_cast<double>(255.0));
        ostream << std::hex << std::setw (2) << std::setfill('0') << x;
        x = static_cast<uint16_t>((m_rgb.b) * static_cast<double>(255.0));
        ostream << std::hex << std::setw (2) << std::setfill('0') << x;
        return ostream.str ();
    }

    double Color::luminance () const
    {
        return (m_rgb.r * m_red_luminance +
                m_rgb.g * m_green_luminance +
                m_rgb.b * m_blue_luminance);
    }
}
