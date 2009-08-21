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

#ifndef MLN_CANVAS_LABELING_INTERNAL_TESTS_HH
# define MLN_CANVAS_LABELING_INTERNAL_TESTS_HH

/// \file
///
/// Shared tests for labeling canvas arguments.

namespace mln
{

  namespace canvas
  {

    namespace labeling
    {

      namespace internal
      {


	template <typename I, typename N, typename L, typename F>
	void
	labeling_tests(const Image<I>& input_, const Neighborhood<N>& nbh_,
		       const L& nlabels, const F& f);


# ifndef MLN_INCLUDE_ONLY

	template <typename I, typename N, typename L, typename F>
	void
	labeling_tests(const Image<I>& input_, const Neighborhood<N>& nbh_,
		       const L& nlabels, const F& f)
	{
	  const I& input = exact(input_);
	  const N& nbh   = exact(nbh_);

	  mln_precondition(input.is_valid());
	  // mln_precondition(nbh.is_valid());

	  (void) input;
	  (void) nbh;
	  (void) nlabels;
	  (void) f;
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace mln::canvas::internal

    } // end of namespace mln::canvas::labeling

  } // end of namespace mln::canvas

} // end of namespace mln

#endif // ! MLN_CANVAS_LABELING_INTERNAL_TESTS_HH
