// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_DEBUG_CHAR_SPACE_IMAGE_HH
# define SCRIBO_DEBUG_CHAR_SPACE_IMAGE_HH

/// \file
///
/// \brief Draw inter character space.

# include <mln/core/concept/image.hh>
# include <mln/core/alias/point2d.hh>
# include <mln/value/rgb8.hh>
# include <mln/data/convert.hh>

# include <mln/draw/box.hh>
# include <mln/draw/line.hh>

# include <mln/literal/colors.hh>

# include <scribo/core/line_set.hh>


namespace scribo
{

  namespace debug
  {

    using namespace mln;


    /// \brief Draw inter character space.
    ///
    /// \ingroup grpalgodebug
    //
    template <typename I, typename L>
    mln_ch_value(I,value::rgb8)
    char_space_image(const Image<I>& input, const line_set<L>& line,
		     const value::rgb8& v = literal::cyan);


# ifndef MLN_INCLUDE_ONLY


    template <typename I, typename L>
    mln_ch_value(I,value::rgb8)
    char_space_image(const Image<I>& input, const line_set<L>& line,
		     const value::rgb8& v)
    {
      mln_trace("scribo::debug::char_space_image");

      mln_ch_value(I,value::rgb8) output = data::convert(value::rgb8(), input);

      const component_set<L>& comps = line.components();
      for_all_lines(l, line)
      {
	if (! line(l).is_valid() || line(l).is_hidden()
	    || line(l).component_ids().size() < 2)
	  continue;


	for_all_elements(i, line(l).component_ids())
	{
	  if (i == line(l).component_ids().nelements() - 1)
	    continue;

	  unsigned c = line(l).component_ids()[i];

	  point2d
	    beg = comps(c).bbox().pmax(),
	    end = beg;
	  beg.row() = comps(c).bbox().pmin().row();
	  mln::draw::line(output, beg, end, v);
	  beg.col() += line(l).char_space();
	  end.col() += line(l).char_space();
	  mln::draw::line(output, beg, end, v);
	}

      }

      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::debug

} // end of namespace scribo

#endif // ! SCRIBO_DEBUG_CHAR_SPACE_IMAGE_HH
