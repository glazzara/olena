// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CANVAS_BROWSING_SNAKE_HH
# define MLN_CANVAS_BROWSING_SNAKE_HH

/// \file
///
/// Browsing in a snake-way, forward.

# include <vector>
# include <mln/core/concept/browsing.hh>


namespace mln
{

  namespace canvas
  {

    namespace browsing
    {

      template <typename I, typename W, typename F>
      inline
      mln_ch_value(I, mln_result(F))
      snake(const Image<I>& input, const Neighborhood<N>& nbh, F& functor);


# ifndef MLN_INCLUDE_ONLY

      template <typename I, typename W, typename F>
      inline
      mln_ch_value(I, mln_result(F))
      snake(const Image<I>& input_, const Neighborhood<N>& nbh, F& functor)
      {
	trace::entering("canvas::browsing::snake");

	I input = exact(input_);
	mln_precondition(input.is_valid());

	// p init
	f.p = f.input.bbox().pmin();// - f.dps[0];

	std::vector< int > directions(f.moves.size(), 0);
	unsigned deph = 0;
	unsigned total_deph = f.moves.size() / 2 + 1;

	// initialization
	f.init();

	bool first = true;
	directions[deph] = 1;
	deph = total_deph - 1;

	// Call the move function (for the first point)
	(f.*(f.moves[(deph - 1) * 2 - 1 + directions[deph - 1]])) ();
	while (deph > 0) // If direction is empty, break
	{
	  mln_assertion(deph <= total_deph);
	  mln_assertion(deph > 0);
	  // If f.p is near the border (we ended a direction) -> next child
	  if (!f.input.domain().has(f.p +
				    f.dps[(deph - 1) * 2 - 1 + directions[deph - 1]]))
	  {
	    // Go up the tree
	    deph--;
	    if (deph >= 1)
	      // Change directions
	      directions[deph] = directions[deph] == 1 ? 0 : 1;
	    continue;
	  }

	  if (!first)
	  {
	    // Move f.p
	    f.p += f.dps[(deph - 1) * 2 - 1 + directions[deph - 1]];
	    // Call the move function
	    (f.*(f.moves[(deph - 1) * 2 - 1 + directions[deph - 1]])) ();
	  }
	  else
	    first = false;

	  if (deph != total_deph)
	  {
	    // Go down the tree
	    deph++;
	    first = true;
	  }
	}

	trace::exiting("canvas::browsing::snake");
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::canvas::browsing

  } // end of namespace mln::canvas

} // end of namespace mln


#endif // ! MLN_CANVAS_BROWSING_SNAKE_HH

