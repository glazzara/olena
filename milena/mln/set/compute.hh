// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_SET_COMPUTE_HH
# define MLN_SET_COMPUTE_HH

/// \file mln/set/compute.hh
///
/// Compute an accumulator on a site set.

# include <mln/core/concept/meta_accumulator.hh>
# include <mln/core/concept/image.hh>
# include <mln/core/concept/site_set.hh>


namespace mln
{

  namespace set
  {

    /// Compute an accumulator onto a site set.
    ///
    /// \param[in] a An accumulator.
    /// \param[in] s A site set.
    /// \return The accumulator result.
    //
    template <typename A, typename S>
    mln_result(A)
    compute(const Accumulator<A>& a, const Site_Set<S>& s);


    /// Compute an accumulator onto a site set.
    ///
    /// \param[in] a A meta-accumulator.
    /// \param[in] s A site set.
    // \return The accumulator result.
    //
    template <typename A, typename S>
    mln_accu_with(A, mln_site(S))::result
    compute(const Meta_Accumulator<A>& a, const Site_Set<S>& s);



# ifndef MLN_INCLUDE_ONLY


    // Implementation.


    namespace impl
    {

      namespace generic
      {

	template <typename A, typename S>
	inline
	mln_result(A)
	compute(const Accumulator<A>& a_, const Site_Set<S>& s_)
	{
	  trace::entering("set::impl::generic::compute");

	  mlc_converts_to(mln_site(S), mln_argument(A))::check();

	  A a = exact(a_);
	  const S& s = exact(s_);

	  a.init();
	  mln_piter(S) p(s);
	  for_all(p)
	    a.take(p);

	  trace::exiting("set::impl::generic::compute");
	  return a.to_result();
	}

      } // end of namespace mln::set::impl::generic

    } // end of namespace mln::set::impl



    // Facades.


    template <typename A, typename S>
    inline
    mln_result(A)
    compute(const Accumulator<A>& a, const Site_Set<S>& s)
    {
      trace::entering("set::compute");

      mlc_converts_to(mln_site(S), mln_argument(A))::check();

      mln_result(A) r = impl::generic::compute(a, s);

      trace::exiting("set::compute");
      return r;
    }


    template <typename A, typename S>
    mln_accu_with(A, mln_site(S))::result
    compute(const Meta_Accumulator<A>& a, const Site_Set<S>& s)
    {
      trace::entering("set::compute");

      typedef mln_accu_with(A, mln_site(S)) A_;
      A_ a_ = accu::unmeta(exact(a), mln_site(S)());

      mln_result(A_) r = impl::generic::compute(a_, s);
      
      trace::exiting("set::compute");
      return r;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::set

} // end of namespace mln


#endif // ! MLN_SET_COMPUTE_HH
