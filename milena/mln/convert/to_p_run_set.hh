// Copyright (C) 2008 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_CONVERT_TO_P_RUN_SET_HH
# define MLN_CONVERT_TO_P_RUN_SET_HH

/*! \file mln/convert/to_p_run_set.hh
 *
 * \brief Conversions to mln::p_run_set<P>.
 *
 * \todo Revamp!
 */

# include <mln/core/concept/image.hh>
# include <mln/core/p_runs.hh>
# include <mln/literal/zero.hh>



namespace mln
{

  namespace convert
  {


    /// Convert a binary image \p input into a set of runs.
    template <typename I>
    p_run_set<mln_site(I)> to_p_run_set(const Image<I>& input);



# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I>
      p_run_set<mln_site(I)> to_p_run_set_(const I& input)
      {
	typedef mln_site(I) P;
	p_run_set<P> runs;
	mln_fwd_piter(I) p(input.domain());
	p.start();
	for (;;)
	  {
	    // Skip background.
	    while (p.is_valid() && input(p) == false)
	      p.next();
	    if (! p.is_valid()) // The end.
	      break;
	    mln_invariant(input(p) == true);
	    P start = p, q;
	    // Go to run end.
	    do
	      {
		q = p;
		p.next();
	      }
	    while (p.is_valid() && input(p) == true && p == q + right);
	    runs.insert(start, q);
	  }
	return runs;
      }


      template <typename S, typename I>
      void fill_p_run_set_(S& s, const I& input)
      {
	mln_value(I) O = literal::zero;
	typedef mln_site(I) P;
	mln_fwd_piter(I) p(input.domain());
	p.start();
	for (;;)
	  {
	    // Skip background.
	    while (p.is_valid() && input(p) == O)
	      p.next();
	    if (! p.is_valid()) // The end.
	      break;
	    mln_invariant(input(p) != O);
	    mln_value(I) v = input(p);
	    P start = p, q;
	    // Go to run end.
	    do
	      {
		q = p;
		p.next();
	      }
	    while (p.is_valid() && input(p) == v && p == q + right);
	    s.insert(v, p_run<P>(start, q));
	  }
      }

    } // end of namespace mln::convert::impl


    template <typename I>
    p_run_set<mln_site(I)> to_p_run_set(const Image<I>& input_)
    {
      const I& input = exact(input_);
      mln_precondition(input.has_data());
      return impl::to_p_run_set_(input);
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::convert

} // end of namespace mln


#endif // ! MLN_CONVERT_TO_P_RUN_SET_HH
