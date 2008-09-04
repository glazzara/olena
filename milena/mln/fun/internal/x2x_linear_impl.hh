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

#ifndef MLN_FUN_INTERNAL_X2X_LINEAR_IMPL_HH
# define MLN_FUN_INTERNAL_X2X_LINEAR_IMPL_HH

/*! \file mln/fun/internal/x2x_linear_impl.hh
 *
 * \brief Implementation class for every linear Function_x2x.
 */

# include <mln/core/concept/function.hh>
# include <mln/algebra/h_mat.hh>
# include <mln/algebra/h_vec.hh>

namespace mln
{

  namespace fun
  {

    namespace internal
    {

      template <typename V, typename E>
      struct x2x_linear_impl_
      {
        static const unsigned dim = V::dim; // To please g++ when comparing dim in metal bexpr.

	typedef V argument;
	typedef V result;
	typedef typename V::coord coord;
	typedef algebra::h_mat<dim, coord> matrix;

	V operator()(const V& x) const
	{
	  algebra::h_vec<dim, coord> tmp = m_ * x.to_h_vec();
	  return tmp.to_vec();
	}

        const matrix& mat() const;

      protected:
	x2x_linear_impl_();

	matrix m_;
      };



# ifndef MLN_INCLUDE_ONLY

      template <typename V, typename E>
      inline
      x2x_linear_impl_<V,E>::x2x_linear_impl_()
      {
      }


      template <typename V, typename E>
      inline
      const typename x2x_linear_impl_<V,E>::matrix&
      x2x_linear_impl_<V,E>::mat() const
      {
	return m_;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::internal

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_INTERNAL_X2X_LINEAR_IMPL_HH
