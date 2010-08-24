// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
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

#ifndef MLN_FUN_INTERNAL_X2X_LINEAR_IMPL_HH
# define MLN_FUN_INTERNAL_X2X_LINEAR_IMPL_HH

/// \file
///
/// \brief Implementation class for every linear function from vector
/// to vector in an homogeneous coordinates system.

# include <mln/core/concept/function.hh>
# include <mln/algebra/h_mat.hh>
# include <mln/algebra/h_vec.hh>

namespace mln
{

  namespace fun
  {

    namespace internal
    {

      template <typename V, typename C, typename E>
      struct x2x_linear_impl_
      {
        static const unsigned dim = V::dim; // To please g++ when comparing dim in metal bexpr.

	typedef V argument;
	typedef V result;
	typedef algebra::h_mat<dim, C> matrix;

	V operator()(const V& x) const
	{
	  algebra::h_vec<dim, C> tmp = m_ * x.to_h_vec();
	  return tmp.to_vec();
	}

        const matrix& mat() const;

      protected:
	x2x_linear_impl_();

	matrix m_;
      };



# ifndef MLN_INCLUDE_ONLY

      template <typename V, typename C, typename E>
      inline
      x2x_linear_impl_<V,C,E>::x2x_linear_impl_()
      {
      }


      template <typename V, typename C, typename E>
      inline
      const typename x2x_linear_impl_<V,C,E>::matrix&
      x2x_linear_impl_<V,C,E>::mat() const
      {
	return m_;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::internal

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_INTERNAL_X2X_LINEAR_IMPL_HH
