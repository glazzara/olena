// Copyright (C) 2007 EPITA Research and Development Laboratory
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

  namespace internal
  {

    // Specialization.

    template <typename C, typename E>
    struct site_const_impl< point_<grid::square, C>, E >
    {
      // Either unproxy() or to_site() can be used below.  In the
      // former case, the unproxied features ind() because it is a
      // point_ or another proxy to a point_.
      C row() const
      {
	return internal::force_exact<const E>(*this).to_site().row();
      }
      C col() const
      {
	return internal::force_exact<const E>(*this).to_site().col();
      }
      C operator[](unsigned i) const
      {
	mln_precondition(i < 2);
	return internal::force_exact<const E>(*this).to_site()[i];
      }
    };


    // Specialization for point_<M,C>.

    template <typename C, typename E>
    struct site_mutable_impl< point_<grid::square, C>, E > :
           site_const_impl  < point_<grid::square, C>, E >
    {
    private:
      typedef site_const_impl< point_<grid::square, C>, E > super;
    public:

      using super::row;
      using super::col;

      C& row()
      {
	return internal::force_exact<E>(*this).to_site().row();
      }
      C& col()
      {
	return internal::force_exact<E>(*this).to_site().col();
      }
    };

  } // end of namespace mln::internal



  /*! \brief Type alias for a point defined on the 2D square grid with
   * integer coordinates.
   */
  typedef point_<grid::square, int> point2d;


} // end of namespace mln


# include <mln/make/point2d.hh>
# include <mln/core/dpoint2d.hh>


#endif // ! MLN_CORE_POINT2D_HH
