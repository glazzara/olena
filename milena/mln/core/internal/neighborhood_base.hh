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

#ifndef MLN_CORE_INTERNAL_NEIGHBORHOOD_BASE_HH
# define MLN_CORE_INTERNAL_NEIGHBORHOOD_BASE_HH

/*! \file mln/core/internal/neighborhood_base.hh
 *
 * \brief Definition of a base class for neighborhood classes.
 */

# include <mln/core/concept/neighborhood.hh>


namespace mln
{

  namespace internal
  {


    /*! \internal A base class for neighborhood classes.
     *
     * \p D is a dpsite type.
     */
    template <typename D, typename E>
    struct neighborhood_base : public Neighborhood<E>
    {

      /// DPsite associated type.
      typedef D dpsite;

      /// Psite associated type.
      typedef mln_psite(D) psite;

      /// Site associated type.
      typedef mln_site(D) site;


      /*! \brief Test (as a window) if it is centered so (as a
       *  neighborhood) return false.
       *
       * \return Always false.
       */
      bool is_centered() const;
      
      /*! \brief Test (as a window) if it is symmetric so (as a
       *  neighborhood) return true.
       *
       * \return Always true.
       */
      bool is_symmetric() const;
      
      /*! Apply (as a window) a central symmetry so (as a
	neighborhood) it is a no-op.
      */
      void sym();

    protected:
      neighborhood_base();
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename D, typename E>
    inline
    neighborhood_base<D,E>::neighborhood_base()
    {
    }

    template <typename D, typename E>
    inline
    bool
    neighborhood_base<D,E>::is_centered() const
    {
      return false;
    }

    template <typename D, typename E>
    inline
    bool
    neighborhood_base<D,E>::is_symmetric() const
    {
      return true;
    }

    template <typename D, typename E>
    inline
    void
    neighborhood_base<D,E>::sym()
    {
      // No-op.
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_NEIGHBORHOOD_BASE_HH
