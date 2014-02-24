// Copyright (C) 2008, 2009, 2014 EPITA Research and Development
// Laboratory (LRDE).
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

#ifndef MLN_FUN_VV2V_VEC_HH
# define MLN_FUN_VV2V_VEC_HH

/// \file
///
/// Forming a vector (mln::algebra::vec) from two values of the same
/// type using a functor.

# include <mln/core/concept/function.hh>
# include <mln/make/vec.hh>


namespace mln
{

  namespace fun
  {

    namespace vv2v
    {

      /// \brief A functor forming a vector (mln::algebra::vec) from
      /// two values of the same type.
      template <typename V>
      struct vec : public Function_vv2v< vec<V> >
      {
	typedef algebra::vec<2,V> result;
	algebra::vec<2,V> operator()(const V& v1, const V& v2) const;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename V>
      inline
      algebra::vec<2,V>
      vec<V>::operator()(const V& v1, const V& v2) const
      {
	return make::vec(v1, v2);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::vv2v

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_VV2V_VEC_HH
