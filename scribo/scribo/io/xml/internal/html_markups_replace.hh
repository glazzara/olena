// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef SCRIBO_IO_XML_INTERNAL_HTML_MARKUPS_REPLACE_HH
# define SCRIBO_IO_XML_INTERNAL_HTML_MARKUPS_REPLACE_HH

/// \file
///
/// \brief Replace HTML markups characters by their corresponding
/// markups.


namespace scribo
{

  namespace io
  {

    namespace xml
    {

      namespace internal
      {

        /*! \brief Replace HTML markups characters by their corresponding
	  markups.
	*/
	inline
	std::string
	html_markups_replace(std::string& input);


# ifndef MLN_INCLUDE_ONLY

	static inline std::map<char, std::string> init_map()
	{
	  std::map<char, std::string> html_map;
	  html_map['\"'] = "&quot;";
	  html_map['<'] = "&lt;";
	  html_map['>'] = "&gt;";
	  html_map['&'] = "&amp;";
	  return html_map;
	}


	inline
	std::string
	html_markups_replace(const std::string& input)
	{
	  static std::map<char, std::string> map = init_map();

	  std::string output = input;
	  for (unsigned i = 0; i < input.size(); ++i)
	  {
	    std::map<char, std::string>::iterator it = map.find(output.at(i));
	    if (it != map.end())
	    {
	      output.replace(i, 1, it->second);
	      i += it->second.size() - 1;
	    }
	  }
	  return output;
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::io::xml::internal

    } // end of namespace scribo::io::xml

  } // end of namespace scribo::io

} // end of namespace scribo

#endif // ! SCRIBO_IO_XML_INTERNAL_HTML_MARKUPS_REPLACE_HH
