// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_FUN_X2V_L1_NORM_HH
# define MLN_FUN_X2V_L1_NORM_HH

/// \file
///
/// Define the L1-norm of an algebraic vector.

# include <mln/core/concept/function.hh>
# include <mln/algebra/vec.hh>
# include <mln/math/abs.hh>


namespace mln
{

  namespace fun
  {

    namespace x2v
    {

      template <typename V>
      struct l1_norm : public Function_v2v< l1_norm<V> >
      {
	typedef mln_coord(V) C;
        typedef mln_sum(C) result;

        result operator()(const V& v) const;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename V>
      inline
      typename l1_norm<V>::result
      l1_norm<V>::operator()(const V& v) const
      {
	result res = 0;
	for (unsigned i = 0; i < V::dim; ++i)
	  res += mln::math::abs(v[i]);
	return res;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::x2v

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_X2V_L1_NORM_HH
