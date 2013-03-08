// Copyright (C) 2008, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_CORE_INTERNAL_WEIGHTED_WINDOW_BASE_HH
# define MLN_CORE_INTERNAL_WEIGHTED_WINDOW_BASE_HH

/// \file
///
/// Definition of a base class for weighted window classes.

# include <mln/core/concept/weighted_window.hh>


namespace mln
{

  namespace internal
  {

    /*!
      \internal
      \brief Base class for weighted window classes.

      \p W is the corresponding window type.
    */
    template <typename W, typename E>
    struct weighted_window_base : public Weighted_Window<E>
    {

      /// Window associated type.
      typedef W window;

      /// Dpsite associated type.
      typedef mln_dpsite(W) dpsite;

      /// Psite associated type.
      typedef mln_psite(W) psite;

      /// Site associated type.
      typedef mln_site(W) site;

      /// Test if the weighted window is empty; final method.
      bool is_empty() const;

      /// Give the window size; final method.  This method is valid
      /// iff the underlying window has a fixed size.
      unsigned size() const;

      /// Test if the weighted window is centered; final method.  This
      /// method is valid iff the support is regular and the
      /// definition is not varying.
      bool is_centered() const;

      /// Give the maximum coordinate gap; final method.  This method
      /// is valid iff the support is regular and the definition is
      /// not varying.
      unsigned delta() const;

      /// Give the \p i-th delta-point; final method.  This method is
      /// valid iff the support is regular and the definition is
      /// unique.
      const mln_dpsite(W)& dp(unsigned i) const;

      /// Test if the \p dp delta-point is in this window; final
      /// method.  This method is valid iff the support is regular and
      /// the definition is unique.
      bool has(const mln_dpsite(W)& dp) const;

      /// return true by default.
      bool is_valid() const;

    protected:
      weighted_window_base();
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename W, typename E>
    inline
    weighted_window_base<W,E>::weighted_window_base()
    {
    }

    template <typename W, typename E>
    inline
    bool
    weighted_window_base<W,E>::is_empty() const
    {
      return exact(this)->win().is_empty();
    }

    template <typename W, typename E>
    inline
    unsigned
    weighted_window_base<W,E>::size() const
    {
      mlc_equal(mln_trait_window_size(W),
		trait::window::size::fixed)::check();
      return exact(this)->win().size();
    }

    template <typename W, typename E>
    inline
    bool
    weighted_window_base<W,E>::is_centered() const
    {
      mlc_equal(mln_trait_window_support(W),
		trait::window::support::regular)::check();
      mlc_not_equal(mln_trait_window_definition(W),
		    trait::window::definition::varying)::check();
      return exact(this)->win().is_centered();
    }

    template <typename W, typename E>
    inline
    unsigned
    weighted_window_base<W,E>::delta() const
    {
      mlc_equal(mln_trait_window_support(W),
		trait::window::support::regular)::check();
      mlc_not_equal(mln_trait_window_definition(W),
		    trait::window::definition::varying)::check();
      return exact(this)->win().delta();
    }

    template <typename W, typename E>
    inline
    const mln_dpsite(W)&
    weighted_window_base<W,E>::dp(unsigned i) const
    {
      mlc_equal(mln_trait_window_support(W),
		trait::window::support::regular)::check();
      mlc_equal(mln_trait_window_definition(W),
		trait::window::definition::unique)::check();
      mln_precondition(i < this->size());
      return exact(this)->win().dp(i);
    }

    template <typename W, typename E>
    inline
    bool
    weighted_window_base<W,E>::has(const mln_dpsite(W)& dp) const
    {
      mlc_equal(mln_trait_window_support(W),
		trait::window::support::regular)::check();
      mlc_equal(mln_trait_window_definition(W),
		trait::window::definition::unique)::check();
      return exact(this)->win().has(dp);
    }

    template <typename W, typename E>
    inline
    bool
    weighted_window_base<W,E>::is_valid() const
    {
      return true;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_WEIGHTED_WINDOW_BASE_HH
