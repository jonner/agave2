/*******************************************************************************
 *
 *  Copyright (c) 2008 Jonathon Jongsma
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

#include <glibmm/markup.h>
#include <giomm/init.h>
#include <giomm/file.h>
#include <giomm/bufferedoutputstream.h>
#include "color-set-manager.h"

namespace agave
{
    // NOTE: this may not be the ideal place to initialize giomm in this
    // library.  It could be moved later.
    class StaticInit
    {
        public:
            StaticInit ()
            {
                Gio::init ();
            }
    };

    static StaticInit init;

    using namespace Glib::Markup;

    static const Glib::ustring ELEMENT_SETS = "sets";
    static const Glib::ustring ELEMENT_SET = "set";
    static const Glib::ustring ELEMENT_NAME = "name";
    static const Glib::ustring ELEMENT_DESCRIPTION = "description";
    static const Glib::ustring ATTRIBUTE_ID = "id";
    static const Glib::ustring ELEMENT_TAGS = "tags";
    static const Glib::ustring ELEMENT_TAG = "tag";
    static const Glib::ustring ELEMENT_COLORS = "colors";
    static const Glib::ustring ELEMENT_COLOR = "color";

    class SavedSetParser : public Parser
    {
        public:
            SavedSetParser () :
                m_active_elements (0)
            {}

            virtual void
            on_start_element (ParseContext& context,
                              const Glib::ustring& element_name,
                              const Parser::AttributeMap& attributes)
            {
                m_active_elements |= m_element_map[element_name];
                std::cout << "<opening element " << element_name << "\n";
                std::cout << std::hex << m_active_elements << "\n";
            }

            virtual void
            on_end_element (ParseContext& context,
                            const Glib::ustring& element_name)
            {
                m_active_elements &= ~m_element_map[element_name];
                std::cout << "closing element " << element_name << ">\n";
                std::cout << std::hex << m_active_elements << "\n";
            }

            virtual void
            on_text (ParseContext& context,
                     const Glib::ustring& text)
            {
            }

            virtual void
            on_passthrough (ParseContext& context,
                            const Glib::ustring& passthrough_text)
            {
            }

            virtual void
            on_error (ParseContext& context,
                      const Glib::MarkupError& error)
            {
            }

        private:
            class ElementMap : public std::map<Glib::ustring, uint16_t>
            {
                public:
                    ElementMap ()
                    {
                        unsigned int pos = 0;
                        insert (std::make_pair (ELEMENT_SETS, 1 << pos++));
                        insert (std::make_pair (ELEMENT_SET, 1 << pos++));
                        insert (std::make_pair (ELEMENT_NAME, 1 << pos++));
                        insert (std::make_pair (ELEMENT_DESCRIPTION, 1 << pos++));
                        insert (std::make_pair (ELEMENT_TAGS, 1 << pos++));
                        insert (std::make_pair (ELEMENT_TAG, 1 << pos++));
                        insert (std::make_pair (ELEMENT_COLORS, 1 << pos++));
                        insert (std::make_pair (ELEMENT_COLOR, 1 << pos++));
                    }
            };

            uint16_t m_active_elements;
            ColorSet m_working_set;
            std::list<ColorSet> m_parsed_set;

            static ElementMap m_element_map;
    };

    SavedSetParser::ElementMap SavedSetParser::m_element_map;

    ColorSetManager::ColorSetManager (std::string filename) :
        m_filename (filename)
    { load (); }

    inline static Glib::ustring
    write_simple_element (const Glib::ustring& element_name,
                   const Glib::ustring& text,
                   const Parser::AttributeMap& attributes = Parser::AttributeMap())
    {
        Glib::ustring result;
        result += "<" + element_name;
        for (Parser::AttributeMap::const_iterator attr_iter = attributes.begin ();
                attr_iter != attributes.end (); ++attr_iter)
        {
            result += " " + attr_iter->first + "=" + attr_iter->second;
        }
        result += ">";
        result += escape_text (text);
        result += Glib::ustring::compose ("</%1>\n", element_name);
        return result;
    }

    void ColorSetManager::load ()
    {
        /*
        // load saved schemes from file
        SavedSetParser parser;
        Glib::Markup::ParseContext pcontext (parser);
        pcontext.parse (text);
        */
    }

    void ColorSetManager::save ()
    {
        Glib::RefPtr<Gio::File> file = Gio::File::create_for_path (m_filename);
        Glib::RefPtr<Gio::BufferedOutputStream> out_stream =
            Gio::BufferedOutputStream::create (file->replace ());
        out_stream->write (Glib::ustring::compose ("<%1>\n", ELEMENT_SETS));
        for (ColorSetManager::iterator set_iter = begin ();
             set_iter != end ();
             ++set_iter)
        {
            std::string id = set_iter->get_id ();
            out_stream->write (Glib::ustring::compose ("<%1 id=\"%2\">\n", ELEMENT_SET, id));
            out_stream->write (write_simple_element (ELEMENT_NAME, set_iter->get_name ()));
            out_stream->write (write_simple_element (ELEMENT_DESCRIPTION, set_iter->get_description ()));
            out_stream->write (Glib::ustring::compose ("<%1>\n", ELEMENT_COLORS));
            for (ColorSet::const_iterator color_iter = set_iter->begin ();
                 color_iter != set_iter->end (); ++color_iter)
            {
                out_stream->write (write_simple_element (ELEMENT_COLOR, color_iter->as_hexstring ()));
            }
            out_stream->write (Glib::ustring::compose ("</%1>\n", ELEMENT_COLORS));
            out_stream->write (Glib::ustring::compose ("</%1>\n", ELEMENT_SET));
        }
        out_stream->write (Glib::ustring::compose ("</%1>\n", ELEMENT_SETS));
        out_stream->close ();
    }

    // FIXME: this is a poor interface.  It doesn't work intuitively.  The user
    // has to create a ColorSet object and pass it to this function and then
    // throw it away and use the one returned from this function since it might
    // have slightly different values than the one they supposedly just added.
    // The purpose of this was to prevent multiple sets with the same colors,
    // but maybe that's not such a bad thing to allow...
    ColorSet& ColorSetManager::add_set (const ColorSet& set)
    {
        iterator it = std::find (m_sets.begin (), m_sets.end (), set);
        if (it == m_sets.end ())
        {
            m_sets.push_back (set);
            return m_sets.back ();
        }
        return *it;
    }

    void ColorSetManager::remove_set (const ColorSet& set)
    {
        iterator it = std::find (m_sets.begin (), m_sets.end (), set);
        if (it != m_sets.end ())
        {
            m_sets.erase (it);
        }
    }

    void ColorSetManager::clear ()
    { m_sets.clear (); }

    ColorSetManager::iterator
        ColorSetManager::begin ()
        { return m_sets.begin (); }

    ColorSetManager::const_iterator
        ColorSetManager::begin () const
        { return m_sets.begin (); }

    ColorSetManager::iterator
        ColorSetManager::end ()
        { return m_sets.end (); }

    ColorSetManager::const_iterator
        ColorSetManager::end () const
        { return m_sets.end (); }

    ColorSetManager::reverse_iterator
        ColorSetManager::rbegin ()
        { return m_sets.rbegin (); }

    ColorSetManager::const_reverse_iterator
        ColorSetManager::rbegin () const
        { return m_sets.rbegin (); }

    ColorSetManager::reverse_iterator
        ColorSetManager::rend ()
        { return m_sets.rend (); }

    ColorSetManager::const_reverse_iterator
        ColorSetManager::rend () const
        { return m_sets.rend (); }
}
