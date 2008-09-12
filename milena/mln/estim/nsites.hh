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

#ifndef MLN_ESTIM_NSITES_HH
# define MLN_ESTIM_NSITES_HH

/*! \file mln/estim/nsites.hh
 *
 * \brief Compute the number of sites of an image or a site set.
 */

# include <mln/core/concept/site_set.hh>
# include <mln/core/concept/image.hh>


namespace mln
{

  namespace estim
  {

    /// Compute the number of sites of the site set \p input.
    template <typename S>
    unsigned nsites(const Site_Set<S>& s);

    /// Compute the number of sites of the image \p input.
    template <typename I>
    unsigned nsites(const Image<I>& input);



# ifndef MLN_INCLUDE_ONLY


    // Implementations.

    namespace impl
    {

      // Generic version.

      namespace generic
      {

	template <typename S>
	unsigned nsites(const Site_Set<S>& s_)
	{
	  trace::entering("estim::impl::generic::nsites");
	  const S& s = exact(s_);
	  mln_precondition(s.is_valid());
	  
	  unsigned n = 0;
	  mln_piter(S) p(s);
	  for_all(p)
	    ++n;

	  trace::exiting("estim::impl::generic::nsites");
	  return n;
	}      

      } // end of namespace mln::estim::impl::generic


      // A single specialization.

      template <typename S>
      inline
      unsigned nsites_method(const Site_Set<S>& s)
      {
	trace::entering("estim::impl::nsites_method");
	unsigned n = exact(s).nsites();
	trace::exiting("estim::impl::nsites_method");
	return n;
      }

    } // end of namespace mln::estim::impl



    // Dispatch.

    namespace internal
    {

      template <typename S>
      inline
      unsigned nsites_dispatch(mln::trait::site_set::nsites::any,
			       const Site_Set<S>& s)
      {
	return impl::generic::nsites(s);
      }      

      template <typename S>
      inline
      unsigned nsites_dispatch(mln::trait::site_set::nsites::known,
			       const Site_Set<S>& s)
      {
	return impl::nsites_method(s);
      }      

      // Dispatch facade.

      template <typename S>
      inline
      unsigned nsites_dispatch(const Site_Set<S>& s)
      {
	return nsites_dispatch(mln_trait_site_set_nsites(S)(),
			       s);
      }      

    } // end of namespace mln::estim::internal


    
    // Facades.

    template <typename S>
    inline
    unsigned nsites(const Site_Set<S>& s)
    {
      trace::entering("estim::nsites");
      mln_precondition(exact(s).is_valid());

      unsigned n = internal::nsites_dispatch(s);

      trace::exiting("estim::nsites");
      return n;
    }

    template <typename I>
    inline
    unsigned nsites(const Image<I>& input_)
    {
      trace::entering("estim::nsites");
      const I& input = exact(input_);

      mln_precondition(input.has_data());
      mln_precondition(input.domain().is_valid());

      // Relies on the nsites routines on a site set.
      unsigned n = internal::nsites_dispatch(input.domain());

      trace::exiting("estim::nsites");
      return n;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::estim

} // end of namespace mln


#endif // ! MLN_ESTIM_NSITES_HH
