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

#ifndef MLN_MORPHO_CONDITIONAL_DILATION_HH
# define MLN_MORPHO_CONDITIONAL_DILATION_HH

/// \file
///
/// Morphological conditional dilation.
///
/// \todo Write a version with (f, g, nbh, n) instead of (f, g, win).

# include <mln/morpho/includes.hh>


namespace mln
{

  namespace morpho
  {

    namespace conditional
    {

      /// Morphological conditional dilation.
      template <typename I, typename J, typename W>
      mln_concrete(I)
      dilation(const Image<I>& f, const Image<J>& g,
	       const Window<W>& win);


# ifndef MLN_INCLUDE_ONLY


      // Tests.

      namespace internal
      {

	template <typename I, typename J, typename W>
	inline
	void
	dilation_tests(const Image<I>& f_, const Image<J>& g_,
		       const Window<W>& win_)
	{
	  const I& f = exact(f_);
	  const J& g = exact(g_);
	  const W& win = exact(win_);

	  mln_precondition(f.is_valid());
	  mln_precondition(g.is_valid());
	  mln_precondition(win.is_valid());

	  mln_precondition(f.domain() == g.domain());

	  (void) f;
	  (void) g;
	  (void) win;
	}

      } // end of namespace morpho::conditional::internal


      template <typename I, typename J, typename W>
      inline
      mln_concrete(I)
      dilation(const Image<I>& f, const Image<J>& g,
	       const Window<W>& win)
      {
	trace::entering("morpho::conditional::dilation");

	internal::dilation_tests(f, g, win);

	mln_concrete(I) output = morpho::min(morpho::dilation(f, win), g);

	trace::exiting("morpho::conditional::dilation");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::conditional

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_CONDITIONAL_DILATION_HH
