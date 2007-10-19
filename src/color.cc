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
#include "color.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>
#include <glibmm-utils/exception.h>

namespace agave
{
    // constants for determining the luminance of a particular color borrowed
    // from the GIMP
    const double RED_LUMINANCE = 0.2126;
    const double GREEN_LUMINANCE = 0.7152;
    const double BLUE_LUMINANCE = 0.0722;

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

    hsv_t operator-(const hsv_t& lhs, const hsv_t& rhs)
    {
        hsv_t new_hsv;
        new_hsv.h = lhs.h - rhs.h;
        new_hsv.s = lhs.s - rhs.s;
        new_hsv.v = lhs.v - rhs.v;
        new_hsv.a = lhs.a;
        return new_hsv;
    }

    hsv_t operator+(const hsv_t& lhs, const hsv_t& rhs)
    {
        hsv_t new_hsv;
        new_hsv.h = lhs.h + rhs.h;
        new_hsv.s = lhs.s + rhs.s;
        new_hsv.v = lhs.v + rhs.v;
        new_hsv.a = lhs.a;
        return new_hsv;
    }

    struct Color::Priv
    {
        void clamp ()
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
            m_hsv.s = std::min (m_hsv.s, MAX_VALUE);
            m_hsv.v = std::min (m_hsv.v, MAX_VALUE);
            m_hsv.a = std::min (m_hsv.a, MAX_VALUE);

            m_hsv.s = std::max (m_hsv.s, MIN_VALUE);
            m_hsv.v = std::max (m_hsv.v, MIN_VALUE);
            m_hsv.a = std::max (m_hsv.a, MIN_VALUE);

            // hue is a special case -- it should wrap around, not be clamped
            while (m_hsv.h < MIN_VALUE)
            {
                m_hsv.h += MAX_VALUE - MIN_VALUE;
            }
            while (m_hsv.h >= MAX_VALUE)
            {
                m_hsv.h -= MAX_VALUE - MIN_VALUE;
            }
        }

        /** Internal data representation in RGBA.
        */
        rgb_t m_rgb;
        /** Internal representation in HSV.  We keep a cached copy of this
         * around since it reduces conversions on demand, and since
         * converting to rgb and back is lossy (e.g. if VALUE reaches 0,
         * then HUE and SATURATION get set to 0 automatically)
         */
        hsv_t m_hsv;

        mutable sigc::signal<void> m_signal_changed;
    };

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

    Color::Color () :
        m_priv (new Priv ())
    {
        // initialize to opaque red
        set_hsv (0.0, 1.0, 1.0, 1.0);
    }

    Color::Color (rgb_t rgb) :
        m_priv (new Priv ())
    {
        set (rgb);
    }

    Color::Color (double r, double g, double b, double a) :
        m_priv (new Priv ())
    {
        set_rgb (r, g, b, a);
    }

    Color::Color (hsv_t hsv) :
        m_priv (new Priv ())
    {
        set (hsv);
    }

    Color::Color (hsl_t hsl) :
        m_priv (new Priv ())
    {
        THROW_IF_FAIL (m_priv);
        set (hsl_to_rgb (hsl));
        m_priv->clamp ();
    }

    Color::Color (cmyk_t cmyk) :
        m_priv (new Priv ())
    {
        THROW_IF_FAIL (m_priv);
        set (cmyk_to_rgb (cmyk));
        m_priv->clamp ();
    }

    Color::Color (std::string hexstring) :
        m_priv (new Priv ())
    {
        // FIXME
        THROW_IF_FAIL (m_priv);
        m_priv->clamp ();
    }

    Color::Color (const Color& other) :
        m_priv (new Priv ())
    {
        THROW_IF_FAIL (m_priv);
        *this = other;
        m_priv->clamp ();
    }

    Color::~Color ()
    {
    }

    Color& Color::operator=(const Color& rhs)
    {
        THROW_IF_FAIL (m_priv);
        if (this == &rhs) return *this;
        m_priv->m_rgb = rhs.m_priv->m_rgb;
        m_priv->m_hsv = rhs.m_priv->m_hsv;
        m_priv->clamp ();
        m_priv->m_signal_changed.emit ();
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
        return (m_priv->m_rgb == rhs.m_priv->m_rgb);
    }

    bool Color::operator!=(const Color& rhs) const
    {
        return !(*this == rhs);
    }

    Color Color::operator-(const Color& rhs) const
    {
        hsv_t hsv = as_hsv () - rhs.as_hsv ();
        return Color (hsv);
    }

    Color Color::operator+(const Color& rhs) const
    {
        hsv_t hsv = as_hsv () + rhs.as_hsv ();
        return Color (hsv);
    }

    Color Color::operator*(const Color& rhs) const
    {
        return *this;
    }

    Color Color::operator*(double factor) const
    {
        THROW_IF_FAIL (m_priv);
        Color result;
        result.m_priv->m_rgb.r = m_priv->m_rgb.r * factor;
        result.m_priv->m_rgb.g = m_priv->m_rgb.g * factor;
        result.m_priv->m_rgb.b = m_priv->m_rgb.b * factor;
        result.m_priv->m_rgb.a = m_priv->m_rgb.a * factor;
        result.m_priv->clamp ();
        return result;
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
        THROW_IF_FAIL (m_priv);
        m_priv->m_rgb = rgb;
        m_priv->m_hsv = rgb_to_hsv (rgb);
        m_priv->clamp ();
        m_priv->m_signal_changed.emit ();
    }

    void Color::set_rgb (double r, double g, double b, double a)
    {
        rgb_t rgb = {r, g, b, a};
        set (rgb);
    }

    void Color::set_red (double r)
    {
        THROW_IF_FAIL (m_priv);
        rgb_t new_rgb = m_priv->m_rgb;
        new_rgb.r = r;
        set (new_rgb);
    }

    void Color::set_green (double g)
    {
        THROW_IF_FAIL (m_priv);
        rgb_t new_rgb = m_priv->m_rgb;
        new_rgb.g = g;
        set (new_rgb);
    }

    void Color::set_blue (double b)
    {
        THROW_IF_FAIL (m_priv);
        rgb_t new_rgb = m_priv->m_rgb;
        new_rgb.b = b;
        set (new_rgb);
    }

    double Color::get_red () const
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_rgb.r;
    }

    double Color::get_green () const
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_rgb.g;
    }

    double Color::get_blue () const
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_rgb.b;
    }

    void Color::set (hsv_t hsv)
    {
        THROW_IF_FAIL (m_priv);
        m_priv->m_hsv = hsv;
        m_priv->clamp ();
        m_priv->m_rgb = hsv_to_rgb (m_priv->m_hsv);
        m_priv->m_signal_changed.emit ();
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
        THROW_IF_FAIL (m_priv);
        rgb_t new_rgb = m_priv->m_rgb;
        new_rgb.a = a;
        set (new_rgb);
    }

    double Color::get_alpha () const
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_rgb.a;
    }

    void Color::set (std::string hexstring)
    {
        // FIXME: parse string to rgb color
    }

    rgb_t Color::as_rgb () const
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_rgb;
    }

    hsv_t Color::as_hsv () const
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_hsv;
    }

    hsl_t Color::as_hsl () const
    {
        THROW_IF_FAIL (m_priv);
        return rgb_to_hsl (m_priv->m_rgb);
    }

    cmyk_t Color::as_cmyk () const
    {
        THROW_IF_FAIL (m_priv);
        return rgb_to_cmyk (m_priv->m_rgb);
    }

    std::string Color::as_hexstring () const
    {
        THROW_IF_FAIL (m_priv);
        std::ostringstream ostream;
        uint16_t x;
        x = static_cast<uint16_t>((m_priv->m_rgb.r) * static_cast<double>(255.0));
        ostream << std::hex << std::setw (2) << std::setfill('0') << x;
        x = static_cast<uint16_t>((m_priv->m_rgb.g) * static_cast<double>(255.0));
        ostream << std::hex << std::setw (2) << std::setfill('0') << x;
        x = static_cast<uint16_t>((m_priv->m_rgb.b) * static_cast<double>(255.0));
        ostream << std::hex << std::setw (2) << std::setfill('0') << x;
        return ostream.str ();
    }

    double Color::luminance () const
    {
        THROW_IF_FAIL (m_priv);
        return (m_priv->m_rgb.r * RED_LUMINANCE +
                m_priv->m_rgb.g * GREEN_LUMINANCE +
                m_priv->m_rgb.b * BLUE_LUMINANCE);
    }

    sigc::signal<void>& Color::signal_changed () const
    {
        THROW_IF_FAIL (m_priv);
        return m_priv->m_signal_changed;
    }
}
