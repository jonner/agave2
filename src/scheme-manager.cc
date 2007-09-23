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
#include "scheme-manager.h"

namespace agave
{
    SchemeManager* SchemeManager::s_instance = 0;

    class AnalogousScheme : public IScheme
    {
        public:
            virtual Glib::ustring get_name () const {
                return "Analogous";
            }

            virtual ColorRelation::SlotColorGen
                get_outer_left () const
                {
                    return sigc::mem_fun (this, &AnalogousScheme::outer_left);
                }

            virtual ColorRelation::SlotColorGen
                get_inner_left () const
                {
                    return sigc::mem_fun (this, &AnalogousScheme::inner_left);
                }

            virtual ColorRelation::SlotColorGen
                get_inner_right () const
                {
                    return sigc::mem_fun (this, &AnalogousScheme::inner_right);
                }

            virtual ColorRelation::SlotColorGen
                get_outer_right () const
                {
                    return sigc::mem_fun (this, &AnalogousScheme::outer_right);
                }

        private:
            Color outer_left (const Color& c) const
            {
                double s_shift = -0.05;
                if (c.get_saturation () <= 0.95)
                {
                    s_shift = 0.05;
                }
                double v_shift = -0.09;
                if (c.get_value () <= 0.91)
                {
                    v_shift = 0.09;
                }
                hsv_t shift = {0.05, s_shift, v_shift, 0.0};
                hsv_t result = c.as_hsv () + shift;
                if (result.s < 0.1)
                {
                    result.s = 0.1;
                }
                if (result.v < 0.2)
                {
                    result.v = 0.2;
                }
                return Color (result);
            }

            Color inner_left (const Color& c) const
            {
                double s_shift = -0.05;
                if (c.get_saturation () <= 0.95)
                {
                    s_shift = 0.05;
                }
                double v_shift = 0.0;
                if (c.get_value () <= 0.95)
                {
                    v_shift = 0.05;
                }
                hsv_t shift = {0.1, s_shift, v_shift, 0.0};
                hsv_t result = c.as_hsv () + shift;
                if (result.s < 0.1)
                {
                    result.s = 0.1;
                }
                if (result.v < 0.2)
                {
                    result.v = 0.2;
                }
                return Color (result);
            }

            Color inner_right (const Color& c) const
            {
                double s_shift = -0.05;
                if (c.get_saturation () <= 0.95)
                {
                    s_shift = 0.05;
                }
                double v_shift = -0.09;
                if (c.get_value () <= 0.91)
                {
                    v_shift = 0.09;
                }
                hsv_t shift = {-0.05, s_shift, v_shift, 0.0};
                hsv_t result = c.as_hsv () + shift;
                if (result.s < 0.1)
                {
                    result.s = 0.1;
                }
                if (result.v < 0.2)
                {
                    result.v = 0.2;
                }
                return Color (result);
            }

            Color outer_right (const Color& c) const
            {
                double s_shift = -0.05;
                if (c.get_saturation () <= 0.95)
                {
                    s_shift = 0.05;
                }
                double v_shift = 0.0;
                if (c.get_value () <= 0.95)
                {
                    v_shift = 0.05;
                }
                hsv_t shift = {-0.1, s_shift, v_shift, 0.0};
                hsv_t result = c.as_hsv () + shift;
                if (result.s < 0.1)
                {
                    result.s = 0.1;
                }
                if (result.v < 0.2)
                {
                    result.v = 0.2;
                }
                return Color (result);
            }
    };

    class MonochromaticScheme : public IScheme
    {
        public:
            virtual Glib::ustring get_name () const {
                return "Monochromatic";
            }

            virtual ColorRelation::SlotColorGen
                get_outer_left () const
                {
                    return sigc::mem_fun (this, &MonochromaticScheme::outer_left);
                }

            virtual ColorRelation::SlotColorGen
                get_inner_left () const
                {
                    return sigc::mem_fun (this, &MonochromaticScheme::inner_left);
                }

            virtual ColorRelation::SlotColorGen
                get_inner_right () const
                {
                    return sigc::mem_fun (this, &MonochromaticScheme::inner_right);
                }

            virtual ColorRelation::SlotColorGen
                get_outer_right () const
                {
                    return sigc::mem_fun (this, &MonochromaticScheme::outer_right);
                }

        private:
            Color outer_left (const Color& c) const
            {
                hsv_t shift = {0.0, 0.0, 0.0, 0.0};
                if (c.get_saturation () < 0.40)
                {
                    shift.s = 0.30;
                }
                else
                {
                    shift.s = -0.30;
                }
                if (c.get_value () < 0.4)
                {
                    shift.v = 0.1;
                }
                else
                {
                    shift.v = 0.1 - (0.1 / 0.6) * (c.get_value () - 0.4);
                }
                hsv_t result = c.as_hsv () + shift;
                if (result.v < 0.2)
                {
                    result.v = 0.2;
                }
                return Color (result);
            }

            Color inner_left (const Color& c) const
            {
                hsv_t shift = {0.0, 0.0, 0.0, 0.0};
                if (c.get_value () > 0.70)
                {
                    shift.v = -0.50;
                }
                else if (c.get_value () > 0.40)
                {
                    shift.v = 0.30;
                }
                else
                {
                    shift.v = 0.30;
                }
                return Color (c.as_hsv () + shift);
            }

            Color inner_right (const Color& c) const
            {
                hsv_t shift = {0.0, 0.0, 0.0, 0.0};
                if (c.get_saturation () < 0.40)
                {
                    shift.s = 0.30;
                }
                else
                {
                    shift.s = -0.30;
                }
                if (c.get_value () > 0.70)
                {
                    shift.v = -0.50;
                }
                else if (c.get_value () > 0.40)
                {
                    shift.v = 0.30;
                }
                else
                {
                    shift.v = 0.30;
                }
                return Color (c.as_hsv () + shift);
            }

            Color outer_right (const Color& c) const
            {
                hsv_t shift = {0.0, 0.0, 0.0, 0.0};
                if (c.get_value () > 0.70)
                {
                    shift.v = -0.20;
                }
                else if (c.get_value () > 0.40)
                {
                    shift.v = -0.20;
                }
                else
                {
                    shift.v = 0.60;
                }
                return Color (c.as_hsv () + shift);
            }
    };

    class TriadScheme : public IScheme
    {
        public:
            virtual Glib::ustring get_name () const {
                return "Triadic";
            }

            virtual ColorRelation::SlotColorGen
                get_outer_left () const
                {
                    return sigc::mem_fun (this, &TriadScheme::outer_left);
                }

            virtual ColorRelation::SlotColorGen
                get_inner_left () const
                {
                    return sigc::mem_fun (this, &TriadScheme::inner_left);
                }

            virtual ColorRelation::SlotColorGen
                get_inner_right () const
                {
                    return sigc::mem_fun (this, &TriadScheme::inner_right);
                }

            virtual ColorRelation::SlotColorGen
                get_outer_right () const
                {
                    return sigc::mem_fun (this, &TriadScheme::outer_right);
                }

        private:
            Color outer_left (const Color& c) const
            {
                hsv_t shift = {1.0 / 3.0, 0.0, 0.0, 0.0};

                if (c.get_saturation () > 0.2)
                {
                    shift.s = -0.1;
                }
                else
                {
                    shift.s = 0.1;
                }
                if (c.get_value () > 0.7)
                {
                    // value shift is 0 at 1.0 and 0.05 at 0.7, so make the shift change
                    // linearly between these points
                    shift.v = (c.get_value () - 0.7) * (0.05 / 0.3);
                }
                else
                {
                    shift.v = 0.05;
                }
                hsv_t result = c.as_hsv () + shift;

                // don't let the value get below 0.2
                if (result.v < 0.2)
                {
                    result.v = 0.2;
                }
                return Color (result);
            }

            Color inner_left (const Color& c) const
            {
                hsv_t shift = {0.0, 0.0, 0.0, 0.0};
                if (c.get_saturation () > 0.9)
                {
                    shift.s = -0.1;
                }
                else
                {
                    shift.s = 0.1;
                }
                if (c.get_value () > 0.5)
                {
                    shift.v = -0.3;
                }
                else
                {
                    shift.v = 0.3;
                }

                hsv_t result = c.as_hsv () + shift;
                return Color (result);
            }

            Color inner_right (const Color& c) const
            {
                hsv_t shift = {-1.0 / 3.0, 0.0, 0.0, 0.0};

                if (c.get_saturation () > 0.1)
                {
                    shift.s = -0.1;
                }
                else
                {
                    shift.s = 0.1;
                }
                if (c.get_value () > 0.5)
                {
                    shift.v = -0.2;
                }
                else
                {
                    shift.v = 0.2;
                }

                hsv_t result = c.as_hsv () + shift;
                return Color (result);
            }

            Color outer_right (const Color& c) const
            {
                hsv_t shift = {-1.0 / 3.0, 0.0, 0.0, 0.0};

                if (c.get_saturation () > 0.95)
                {
                    shift.s = -0.05;
                }
                else
                {
                    shift.s = 0.05;
                }
                if (c.get_value () > 0.7)
                {
                    shift.v = -0.3;
                }
                else
                {
                    shift.v = 0.3;
                }

                hsv_t result = c.as_hsv () + shift;
                return Color (result);
            }
    };

    // NOTE: this is not thread-safe, don't use in multi-threaded apps
    SchemeManager& SchemeManager::instance ()
    {
        if (!s_instance)
        {
            s_instance = new SchemeManager ();
        }
        return *s_instance;
    }

    SchemeManager::SchemeManager ()
    {
        // get and cache the list of available schemes
        boost::shared_ptr<IScheme> analogous (new AnalogousScheme ());
        m_schemes.push_back (analogous);

        boost::shared_ptr<IScheme> monochromatic (new MonochromaticScheme ());
        m_schemes.push_back (monochromatic);

        boost::shared_ptr<IScheme> triad (new TriadScheme ());
        m_schemes.push_back (triad);

        // theoretically this could now load user-defined schemes somewhere
        // (either as defined to-be-defined text format or as compiled plugins),
        // but that hasn't been implemented
    }

    SchemeManager::~SchemeManager ()
    {
    }

    const std::vector<boost::shared_ptr<IScheme> >& SchemeManager::get_schemes ()
    {
        return m_schemes;
    }
}
