// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TRAIT_OP_ORD_HH
# define MLN_TRAIT_OP_ORD_HH

/// \file mln/trait/op/ord.hh
///
/// Declaration of an ordering operator trait.

# include <mln/trait/op/decl.hh>



namespace mln
{

  // Forward declarations.
  namespace internal {
    template <typename T> struct ord_less;
    template <typename T> struct ord_vec;
  }
  namespace util {
    template <typename T> bool ord_strict(const T& lhs, const T& rhs);
  }


  namespace trait
  {

    namespace op
    {

      template <typename T>
      struct ord : public solve_unary<ord, T>
      {
      };

    } // end of namespace mln::trait::op


    template <template <class> class Category, typename T>
    struct set_unary_< op::ord, Category, T >
    {
      typedef mln::internal::ord_less<T> ret;
    };


  } // end of namespace mln::trait


  namespace internal
  {

    template <typename T>
    struct ord_less
    {
      bool strict(const T& lhs, const T& rhs) const;
      bool weak(const T& lhs, const T& rhs) const;
    };

    template <typename T>
    struct ord_vec
    {
      bool strict(const T& lhs, const T& rhs) const;
      bool weak(const T& lhs, const T& rhs) const;
    };


# ifndef MLN_INCLUDE_ONLY

    // ord_less

    template <typename T>
    inline
    bool
    ord_less<T>::strict(const T& lhs, const T& rhs) const
    {
      return lhs < rhs;
    }

    template <typename T>
    inline
    bool
    ord_less<T>::weak(const T& lhs, const T& rhs) const
    {
      return lhs <= rhs;
    }

    // ord_vec

    template <typename T>
    inline
    bool
    ord_vec<T>::strict(const T& lhs, const T& rhs) const
    {
      for (unsigned i = 0; i < T::dim; ++i)
	{
	  if (lhs[i] == rhs[i])
	    continue;
	  return mln::util::ord_strict(lhs[i], rhs[i]);
	}
      return false;
    }

    template <typename T>
    inline
    bool
    ord_vec<T>::weak(const T& lhs, const T& rhs) const
    {
      for (unsigned i = 0; i < T::dim; ++i)
	{
	  if (lhs[i] == rhs[i])
	    continue;
	  return mln::util::ord_strict(lhs[i], rhs[i]);
	}
      return true;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::trait::internal

} // end of namespace mln


# include <mln/util/ord.hh>
# include <mln/trait/solve.hh>


#endif // ! MLN_TRAIT_OP_ORD_HH
