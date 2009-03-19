// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TRAIT_VALUE_INTERNAL_COMP_HH
# define MLN_TRAIT_VALUE_INTERNAL_COMP_HH

/// \file mln/trait/value/internal/comp.hh
///
/// Include all internals for value traits.

# include <mln/metal/bool.hh>
# include <mln/metal/if.hh>



# define mln_trait_value_comp(T, i)  typename mln::trait::value::internal::comp< T, i >::ret



namespace mln
{

  namespace trait
  {


    // Forward declaration.
    template <typename V> struct value_;


    namespace value
    {

      namespace internal
      {

	// bind_comp< T, i >

	template <typename Tr, unsigned i>
	struct bind_comp;

	template <typename Tr> struct bind_comp< Tr, 0 > { typedef typename Tr::comp_0 ret; };
	template <typename Tr> struct bind_comp< Tr, 1 > { typedef typename Tr::comp_1 ret; };
	template <typename Tr> struct bind_comp< Tr, 2 > { typedef typename Tr::comp_2 ret; };
	template <typename Tr> struct bind_comp< Tr, 3 > { typedef typename Tr::comp_3 ret; };
	template <typename Tr> struct bind_comp< Tr, 4 > { typedef typename Tr::comp_4 ret; };
	template <typename Tr> struct bind_comp< Tr, 5 > { typedef typename Tr::comp_5 ret; };
	template <typename Tr> struct bind_comp< Tr, 6 > { typedef typename Tr::comp_6 ret; };
	template <typename Tr> struct bind_comp< Tr, 7 > { typedef typename Tr::comp_7 ret; };
	template <typename Tr> struct bind_comp< Tr, 8 > { typedef typename Tr::comp_8 ret; };
	template <typename Tr> struct bind_comp< Tr, 9 > { typedef typename Tr::comp_9 ret; };


	// get_comp< T, i, dim >

	template <typename C, typename Tr, unsigned i>
	struct get_comp_helper
	{
	  typedef C ret;
	};

	template <typename Tr, unsigned i>
	struct get_comp_helper< void, Tr, i >
	{
	  typedef typename bind_comp<Tr, i>::ret ret;
	};

	template <typename T, unsigned i, unsigned dim>
	struct get_comp
	{
	  typedef mln::trait::value_<T> Tr;
	  typedef typename Tr::comp C;
	  typedef typename get_comp_helper<C, Tr, i>::ret ret;
	};

	template <typename T>
	struct get_comp< T, 0, 1 >
	{
	  typedef T ret;
	};
	

	// comp< T, i >

	template <typename T, unsigned i>
	struct comp
	  : private metal::bool_< (i < mln::trait::value_<T>::dim) >::check_t
	{
	  enum { dim = mln::trait::value_<T>::dim };
	  typedef typename get_comp<T, i, dim>::ret ret;
	};

      } // end of namespace mln::trait::value::internal

    } // end of namespace mln::trait::value

  } // end of namespace mln::trait

} // end of namespace mln



#endif // ! MLN_TRAIT_VALUE_INTERNAL_COMP_HH
