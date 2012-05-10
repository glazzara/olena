// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_INTERNAL_NEIGHBORHOOD_BASE_HH
# define MLN_CORE_INTERNAL_NEIGHBORHOOD_BASE_HH

/// \file
///
/// Definition of a base class for neighborhood classes.
///
/// \todo Complete conditional implementation inheritance
/// w.r.t. properties.

# include <mln/core/concept/neighborhood.hh>
# include <mln/core/concept/window.hh>


namespace mln
{


  // Forward declaration.
  template <typename W> class neighb;
  namespace win
  {
    template <unsigned n, typename W, typename F>
    class multiple_size;
  }


  namespace internal
  {

    template <typename W, typename E>
    struct neighborhood_extra_impl
      : public Neighborhood<E>
    {
    };

    template <typename W, typename F, typename E>
    struct neighborhood_extra_impl< win::multiple_size<2,W,F>, E >
      : public Neighborhood<E>
    {

      /// Give the foreground neighborhood in the case of a dual
      /// neighborhood.  For instance, with (object:c4, background:c8),
      /// the result is c4.

      neighb<W> foreground() const
      {
	W win = internal::force_exact<E>(*this).win().window_(1); // True, so object.
	neighb<W> nbh(win);
	return nbh;
      }

      /// Give the background neighborhood in the case of a dual
      /// neighborhood.  For instance, with (object:c4, background:c8),
      /// the result is c8.

      neighb<W> background() const
      {
	W win = internal::force_exact<E>(*this).win().window_(0); // False, so background.
	neighb<W> nbh(win);
	return nbh;
      }

    };




    template <typename W, typename E>
    struct neighborhood_impl : public neighborhood_extra_impl<W,E>
    {
      // Optional methods...

      /// Give the neighborhood size, i.e., the number of elements it
      /// contains.
      unsigned size() const;

      /// Give the maximum coordinate gap between the neighborhood
      /// center and a neighboring point.
      unsigned delta() const;

      /// Give the maximum coordinate gap between the neighborhood
      /// center and a neighboring point.
      const mln_dpsite(W)& dp(unsigned i) const;

      // end of Optional methods.
    };


    /// Base class for neighborhood implementation classes.
    ///
    /// \p W is the underlying window type.

    template <typename W, typename E>
    struct neighborhood_base : public neighborhood_impl<W,E>
    {
      /// Window associated type.
      typedef W window;

      /// Dpsite associated type.
      typedef mln_dpsite(W) dpsite;

      /// Psite associated type.
      typedef mln_psite(W) psite;

      /// Site associated type.
      typedef mln_site(W) site;

      /// Return true by default.
      bool is_valid() const;

    protected:
      neighborhood_base();
    };




# ifndef MLN_INCLUDE_ONLY


    // neighborhood_base

    template <typename W, typename E>
    inline
    neighborhood_base<W,E>::neighborhood_base()
    {
    }

    template <typename W, typename E>
    inline
    bool
    neighborhood_base<W,E>::is_valid() const
    {
      return true;
    }

    // neighborhood_impl

    template <typename W, typename E>
    inline
    unsigned
    neighborhood_impl<W,E>::size() const
    {
      mlc_is(mln_trait_window_size(W),
	     trait::window::size::fixed)::check();
      return exact(this)->win().size();
    }

    template <typename W, typename E>
    inline
    unsigned
    neighborhood_impl<W,E>::delta() const
    {
      mlc_is(mln_trait_window_support(W),
	     trait::window::support::regular)::check();
      mlc_is_not(mln_trait_window_definition(W),
		 trait::window::definition::varying)::check();
      return exact(this)->win().delta();
    }

    template <typename W, typename E>
    inline
    const mln_dpsite(W)&
    neighborhood_impl<W,E>::dp(unsigned i) const
    {
      mlc_is(mln_trait_window_support(W),
	     trait::window::support::regular)::check();
      mlc_is(mln_trait_window_definition(W),
	     trait::window::definition::unique)::check();
      return exact(this)->win().dp(i);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_NEIGHBORHOOD_BASE_HH
