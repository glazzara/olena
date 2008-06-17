// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_POINT2D_HH
# define MLN_CORE_POINT2D_HH

/*! \file mln/core/point2d.hh
 *
 * \brief Definition of the mln::point2d alias and of its construction
 * routine.
 */

# include <mln/core/point.hh>
# include <mln/core/concept/site_proxy.hh> // For site_const_impl and site_mutable_impl.
# include <mln/core/internal/force_exact.hh>


namespace mln
{

  /*! \brief Type alias for a point defined on the 2D square grid with
   * integer coordinates.
   */
  typedef point_<grid::square, int> point2d;


  namespace internal
  {

    // Specialization.

    template <typename C, typename E>
    struct site_const_impl< point_<grid::square, C>, E >
    {
      C row() const;
      C col() const;
      C operator[](unsigned i) const;
    private:
      const E& exact_() const;
    };


    // Specialization for point_<M,C>.

    template <typename C, typename E>
    struct site_mutable_impl< point_<grid::square, C>, E > :
           site_const_impl  < point_<grid::square, C>, E >
    {
      C& row();
      C& col();
      C& operator[](unsigned i);
    private:
      typedef site_const_impl< point_<grid::square, C>, E > super;
      E& exact_();
    };



# ifndef MLN_INCLUDE_ONLY

    // Either unproxy() or to_site() can be used below.  In the
    // former case, the unproxied features ind() because it is a
    // point_ or another proxy to a point_.

    // site_const_impl

    template <typename C, typename E>
    C
    site_const_impl< point_<grid::square, C>, E >::row() const
    {
      return exact_().to_site().row();
    }

    template <typename C, typename E>
    C
    site_const_impl< point_<grid::square, C>, E >::col() const
    {
      return exact_().to_site().col();
    }

    template <typename C, typename E>
    C
    site_const_impl< point_<grid::square, C>, E >::operator[](unsigned i) const
    {
      mln_precondition(i < 2);
      return exact_().to_site()[i];
    }

    template <typename C, typename E>
    const E&
    site_const_impl< point_<grid::square, C>, E >::exact_() const
    {
      return internal::force_exact<const E>(*this);
    }

    // site_mutable_impl

    template <typename C, typename E>
    C&
    site_mutable_impl< point_<grid::square, C>, E >::row()
    {
      return exact_().to_site().row();
    }

    template <typename C, typename E>
    C&
    site_mutable_impl< point_<grid::square, C>, E >::col()
    {
      return exact_().to_site().col();
    }

    template <typename C, typename E>
    C&
    site_mutable_impl< point_<grid::square, C>, E >::operator[](unsigned i)
    {
      mln_precondition(i < 2);
      return exact_().to_site()[i];
    }
    
    template <typename C, typename E>
    E&
    site_mutable_impl< point_<grid::square, C>, E >::exact_()
    {
      return internal::force_exact<E>(*this);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal
  

} // end of namespace mln


# include <mln/make/point2d.hh>
# include <mln/core/dpoint2d.hh>


#endif // ! MLN_CORE_POINT2D_HH
