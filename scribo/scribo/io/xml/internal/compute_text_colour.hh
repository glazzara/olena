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

#ifndef SCRIBO_IO_XML_INTERNAL_COMPUTE_TEXT_COLOUR_HH
# define SCRIBO_IO_XML_INTERNAL_COMPUTE_TEXT_COLOUR_HH

/// \file
///
/// Find the color name of a given color.

# include <mln/value/rgb8.hh>
# include <mln/value/int_u8.hh>
# include <mln/make/vec.hh>
# include <mln/norm/l2.hh>


namespace scribo
{

  namespace io
  {

    namespace xml
    {

      namespace internal
      {

	using namespace mln;

	const char *
	compute_text_colour(const value::rgb8& v);


	struct color_t
	{
	  const algebra::vec<3, value::int_u8> v;
	  const char *name;
	  float res;
	};


# ifndef MLN_INCLUDE_ONLY


	inline
	const char *
	compute_text_colour(const value::rgb8& v)
	{
	  static color_t colors[] = {
	    { mln::make::vec(0, 0, 0), "black", 0 },
	    { mln::make::vec(255, 0, 0), "red", 0 },
	    { mln::make::vec(255, 255, 255), "white", 0 },
	    { mln::make::vec(0, 255, 0), "green", 0 },
	    { mln::make::vec(0, 0, 255), "blue", 0 },
	    { mln::make::vec(255, 255, 0), "yellow", 0 },
	    { mln::make::vec(255, 165, 0), "orange", 0 },
	    { mln::make::vec(255, 192, 203), "pink", 0 },
	    { mln::make::vec(192, 192, 192), "grey", 0 },
	    { mln::make::vec(64, 224, 208), "turquoise", 0 },
	    { mln::make::vec(75, 0, 130), "indigo", 0 },
	    { mln::make::vec(238, 130, 238), "violet", 0 },
	    { mln::make::vec(0, 255, 255), "cyan", 0 },
	    { mln::make::vec(255, 0, 255), "magenta", 0 },
	    { mln::make::vec(0, 0, 0), 0, 0 } // Invalid
	  };

	  unsigned closest = 0;
	  for (unsigned i = 0; colors[i].name; ++i)
	  {
	    colors[i].res = norm::l2_distance(colors[i].v, v.to_equiv());
	    if (colors[closest].res > colors[i].res)
	      closest = i;
	  }

	  return colors[closest].name;
	}

#endif // MLN_INCLUDE_ONLY

      } // end of namespace scribo::io::xml::internal

    } // end of namespace scribo::io::xml

  } // end of namespace scribo::io

} // end of namespace scribo


#endif // ! SCRIBO_IO_XML_INTERNAL_COMPUTE_TEXT_COLOUR_HH
