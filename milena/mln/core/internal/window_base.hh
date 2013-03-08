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

#ifndef MLN_CORE_INTERNAL_WINDOW_BASE_HH
# define MLN_CORE_INTERNAL_WINDOW_BASE_HH

/// \file
///
/// Definition of a base class for window classes.

# include <mln/core/concept/window.hh>


namespace mln
{

  namespace internal
  {


    /*!
      \internal
      \brief A base class for window classes.

      \p D is a dpsite type.
    */
    template <typename D, typename E>
    struct window_base : public Window<E>
    {

      /// DPsite associated type.
      typedef D dpsite;

      /// Psite associated type.
      typedef mln_psite(D) psite;

      /// Site associated type.
      typedef mln_site(D) site;

      /// Type of the window center
      typedef psite center_t;

      /// Test if this window can be a neighborhood.
      // This method is used in the neighborhood window-adapter.
      bool is_neighbable_() const;

      /// return true by default.
      bool is_valid() const;

    protected:
      window_base();
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename D, typename E>
    inline
    window_base<D,E>::window_base()
    {
    }

    template <typename D, typename E>
    inline
    bool
    window_base<D,E>::is_valid() const
    {
      return true;
    }

    template <typename D, typename E>
    inline
    bool
    window_base<D,E>::is_neighbable_() const
    {
      return exact(this)->is_symmetric() && ! exact(this)->is_centered();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_WINDOW_BASE_HH
