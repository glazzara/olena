// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TRAIT_VALUE_INTERNAL_COMP_HH
# define MLN_TRAIT_VALUE_INTERNAL_COMP_HH

/// \file
///
/// Include all internals for comp value trait.
///
/// \todo Revamp to directly access to the most internal helper
/// instead of bumping from one helper to another.

# include <mln/metal/bool.hh>
# include <mln/metal/if.hh>




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

	template <typename T, typename Tr, unsigned i>
	struct bind_comp;

	template <typename T, typename Tr>
	struct bind_comp< T, Tr, 0 >
	{
	  typedef typename Tr::comp_0 ret;
	  static ret on(const T& v)
	  {
	    return Tr::get_comp_0(v);
	  }
	};

	template <typename T, typename Tr>
	struct bind_comp< T, Tr, 1 >
	{
	  typedef typename Tr::comp_1 ret;
	  static ret on(const T& v)
	  {
	    return Tr::get_comp_1(v);
	  }
	};

	template <typename T, typename Tr>
	struct bind_comp< T, Tr, 2 >
	{
	  typedef typename Tr::comp_2 ret;
	  static ret on(const T& v)
	  {
	    return Tr::get_comp_2(v);
	  }
	};

	template <typename T, typename Tr>
	struct bind_comp< T, Tr, 3 >
	{
	  typedef typename Tr::comp_3 ret;
	  static ret on(const T& v)
	  {
	    return Tr::get_comp_3(v);
	  }
	};

	template <typename T, typename Tr>
	struct bind_comp< T, Tr, 4 >
	{
	  typedef typename Tr::comp_4 ret;
	  static ret on(const T& v)
	  {
	    return Tr::get_comp_4(v);
	  }
	};

	template <typename T, typename Tr>
	struct bind_comp< T, Tr, 5 >
	{
	  typedef typename Tr::comp_5 ret;
	  static ret on(const T& v)
	  {
	    return Tr::get_comp_5(v);
	  }
	};

	template <typename T, typename Tr>
	struct bind_comp< T, Tr, 6 >
	{
	  typedef typename Tr::comp_6 ret;
	  static ret on(const T& v)
	  {
	    return Tr::get_comp_6(v);
	  }
	};

	template <typename T, typename Tr>
	struct bind_comp< T, Tr, 7 >
	{
	  typedef typename Tr::comp_7 ret;
	  static ret on(const T& v)
	  {
	    return Tr::get_comp_7(v);
	  }
	};

	template <typename T, typename Tr>
	struct bind_comp< T, Tr, 8 >
	{
	  typedef typename Tr::comp_8 ret;
	  static ret on(const T& v)
	  {
	    return Tr::get_comp_8(v);
	  }
	};

	template <typename T, typename Tr>
	struct bind_comp< T, Tr, 9 >
	{
	  typedef typename Tr::comp_9 ret;
	  static ret on(const T& v)
	  {
	    return Tr::get_comp_9(v);
	  }
	};


	// get_comp< T, i, dim >

	template <typename T, typename C, typename Tr, unsigned i>
	struct get_comp_helper
	{
	  typedef C ret;
	  static ret on(const T& v)
	  {
	    return v[i];
	  }
	};

	template <typename T, typename Tr, unsigned i>
	struct get_comp_helper< T, void, Tr, i >
	{
	  typedef bind_comp<T, Tr, i> helper;
	  typedef typename helper::ret ret;
	  static ret on(const T& v)
	  {
	    return helper::on(v);
	  }
	};

	template <typename T, unsigned i, unsigned dim>
	struct get_comp
	{
	  typedef mln::trait::value_<T> Tr;
	  typedef typename Tr::comp C;
	  typedef get_comp_helper<T, C, Tr, i> helper;
	  typedef typename helper::ret ret;

	  static ret on(const T& v)
	  {
	    return helper::on(v);
	  }
	};


	template <typename T, unsigned i, unsigned dim>
	struct get_comp< T[dim], i, dim >
	{
	  typedef T ret;

	  static ret on(const T (&v)[dim])
	  {
	    return v[i];
	  }
	};

	template <typename T>
	struct get_comp< T, 0, 1 >
	{
	  typedef T ret;

	  static ret on(const T& v)
	  {
	    return v;
	  }
	};
	
	template <typename T>
	struct get_comp< T[1], 0, 1 > // Disambiguate between both
				      // previous specialization.
	{
	  typedef T ret;
	  static ret on(const T (&v)[1]) { return v[0]; }
	};


	// comp< T, i >

	template <typename T, unsigned i>
	struct comp
	  : private metal::bool_< (i < mln::trait::value_<T>::dim) >::check_t
	{
	  enum { dim = mln::trait::value_<T>::dim };
	  typedef get_comp<T, i, dim> helper;
	  typedef typename helper::ret ret;

	  static ret on(const T& v)
	  {
	    return helper::on(v);
	  }
	};

      } // end of namespace mln::trait::value::internal

    } // end of namespace mln::trait::value

  } // end of namespace mln::trait

} // end of namespace mln



#endif // ! MLN_TRAIT_VALUE_INTERNAL_COMP_HH
