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

#ifndef SCRIBO_TEXT_LINE_LINKS_X_HEIGHT_HH
# define SCRIBO_TEXT_LINE_LINKS_X_HEIGHT_HH

/// \file
///
/// \brief Filter line links according to character x height.

# include <scribo/core/macros.hh>
# include <scribo/core/line_set.hh>
# include <scribo/core/line_links.hh>


namespace scribo
{

  namespace filter
  {

    /*! \brief Filter line links according to character x height.

      \param[in] links Line links information.

      \return A new line_links structure where some links may have
      been invalidated.

      \ingroup grpalgofilterlinelink
     */
    template <typename L>
    line_links<L>
    line_links_x_height(const line_links<L>& links);


# ifndef MLN_INCLUDE_ONLY

    template <typename L>
    line_links<L>
    line_links_x_height(const line_links<L>& links)
    {
      line_links<L> output = links.duplicate();

      float min_x, max_x;//, min_a, max_a;
      const line_set<L>& lines = links.lines();
      for_all_links(l, links)
	if (links(l))
	{
	  max_x = lines(l).x_height();
	  // max_a = lines(l).a_height;
	  min_x = lines(output(l)).x_height();
	  // min_a = lines(output(l)).a_height;

	  if (lines(l).x_height() < lines(output(l)).x_height())
	    std::swap(max_x, min_x);

	  // if (lines(l).a_height < lines(output(l)).a_height)
	  //   std::swap(max_a, min_a);

	  if (min_x / max_x < 0.72f)// || min_a / max_a < 0.80f)
	    output(l) = l;
	}

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo

#endif // ! SCRIBO_TEXT_LINE_LINKS_X_HEIGHT_HH
