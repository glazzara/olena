// Copyright (C) 2008 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_SET_CARD_HH
# define MLN_SET_CARD_HH

/// \file
///
/// Compute the cardinality of a site set.

# include <mln/core/concept/site_set.hh>


namespace mln
{

  namespace set
  {

    /// Compute the cardinality of the site set \p s.
    template <typename S>
    unsigned card(const Site_Set<S>& s);


# ifndef MLN_INCLUDE_ONLY


    // Implementations.

    namespace impl
    {

      // Generic version.

      namespace generic
      {

	template <typename S>
	unsigned card(const Site_Set<S>& s_)
	{
	  trace::entering("set::impl::generic::card");
	  const S& s = exact(s_);
	  mln_precondition(s.is_valid());

	  unsigned n = 0;
	  mln_piter(S) p(s);
	  for_all(p)
	    ++n;

	  trace::exiting("set::impl::generic::card");
	  return n;
	}

      } // end of namespace mln::set::impl::generic


      // A single specialization.

      template <typename S>
      inline
      unsigned card_from_method(const Site_Set<S>& s)
      {
	trace::entering("set::impl::card_from_method");
	unsigned n = exact(s).nsites();
	trace::exiting("set::impl::card_from_method");
	return n;
      }

    } // end of namespace mln::set::impl



    // Dispatch.

    namespace internal
    {

      template <typename S>
      inline
      unsigned card_dispatch(mln::trait::site_set::nsites::any,
			     const Site_Set<S>& s)
      {
	return impl::generic::card(s);
      }

      template <typename S>
      inline
      unsigned card_dispatch(mln::trait::site_set::nsites::known,
			     const Site_Set<S>& s)
      {
	return impl::card_from_method(s);
      }

      // Dispatch facade.

      template <typename S>
      inline
      unsigned card_dispatch(const Site_Set<S>& s)
      {
	return card_dispatch(mln_trait_site_set_nsites(S)(),
			     s);
      }

    } // end of namespace mln::set::internal



    // Facade.

    template <typename S>
    inline
    unsigned card(const Site_Set<S>& s)
    {
      trace::entering("set::card");
      mln_precondition(exact(s).is_valid());

      unsigned n = internal::card_dispatch(s);

      trace::exiting("set::card");
      return n;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::set

} // end of namespace mln


#endif // ! MLN_SET_CARD_HH
