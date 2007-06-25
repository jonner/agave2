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

int main (int argc, char** argv)
{
    agave::rgb_t rgb = {0.34, 0.12, 0.67, 0.75};
    agave::Color c(rgb);
    std::cout << c << std::endl;
    std::cout << c*2 << std::endl;
    agave::rgb_t rgb2 = {3.34, 0.12, 0.67, 0.75};
    agave::Color c2(rgb2);
    std::cout << c2 << std::endl;
    std::cout << c + c2 << std::endl;
}

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

    void Color::rgb_to_hsv (const rgb_t& rgb, hsv_t& hsv)
    {
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
    }

    void Color::hsv_to_rgb(const hsv_t& hsv, rgb_t& rgb)
    {
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
    }

    void Color::rgb_to_hsl(const rgb_t& rgb, hsl_t& hsl)
    {
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

    void Color::hsl_to_rgb(const hsl_t& hsl, rgb_t& rgb)
    {
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
    }

    void Color::cmyk_to_rgb(const cmyk_t& cmyk, rgb_t& rgb)
    {
    }

    Color::Color(void)
    {
        m_data.r = 0.;
        m_data.g = 0.;
        m_data.b = 0.;
        m_data.a = 1.;
    }

    Color::Color(rgb_t rgb)
    {
        m_data = rgb;
        clamp ();
    }

    Color::Color(double r, double g, double b, double a)
    {
        m_data.r = r;
        m_data.g = g;
        m_data.b = b;
        m_data.a = a;
        clamp ();
    }

    Color::Color(hsv_t hsv)
    {
        hsv_to_rgb (hsv, m_data);
        clamp ();
    }

    Color::Color(hsl_t hsl)
    {
        hsl_to_rgb (hsl, m_data);
        clamp ();
    }

    Color::Color(cmyk_t cmyk)
    {
        cmyk_to_rgb (cmyk, m_data);
        clamp ();
    }

    Color::Color(Glib::ustring hexstring)
    {
        // FIXME
        clamp ();
    }

    Color::Color(const Color& other)
    {
        *this = other;
        clamp ();
    }

    Color::~Color(void)
    {
    }

    Color& Color::operator=(const Color& rhs)
    {
        if (this == &rhs) return *this;
        m_data = rhs.m_data;
        clamp ();
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
        return (m_data == rhs.m_data);
    }

    bool Color::operator!=(const Color& rhs) const
    {
        return !(*this == rhs);
    }

    Color Color::operator-(const Color& rhs) const
    {
        Color result;
        result.m_data.r = m_data.r - rhs.m_data.r;
        result.m_data.g = m_data.g - rhs.m_data.g;
        result.m_data.b = m_data.b - rhs.m_data.b;
        result.m_data.a = m_data.a;
        result.clamp ();
        return result;
    }

    Color Color::operator+(const Color& rhs) const
    {
        Color result;
        result.m_data.r = m_data.r + rhs.m_data.r;
        result.m_data.g = m_data.g + rhs.m_data.g;
        result.m_data.b = m_data.b + rhs.m_data.b;
        result.m_data.a = m_data.a;
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
        result.m_data.r = m_data.r * factor;
        result.m_data.g = m_data.g * factor;
        result.m_data.b = m_data.b * factor;
        result.m_data.a = m_data.a * factor;
        result.clamp ();
        return result;
    }

    void Color::clamp ()
    {
        m_data.r = std::min (m_data.r, MAX_VALUE);
        m_data.g = std::min (m_data.g, MAX_VALUE);
        m_data.b = std::min (m_data.b, MAX_VALUE);
        m_data.a = std::min (m_data.a, MAX_VALUE);

        m_data.r = std::max (m_data.r, MIN_VALUE);
        m_data.g = std::max (m_data.g, MIN_VALUE);
        m_data.b = std::max (m_data.b, MIN_VALUE);
        m_data.a = std::max (m_data.a, MIN_VALUE);
    }


    std::ostream& operator<<(std::ostream& out, const Color& c)
    {
        return out << "<" << &c << "> " << "[R=" << c.m_data.r << " G=" <<
            c.m_data.g << " B=" << c.m_data.b << " A=" << c.m_data.a << "]";
    }
}
