// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR F PARTICULAR PURPOSE.  See the GNU
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

#ifndef MLN_TRAIT_FUN_HH
# define MLN_TRAIT_FUN_HH

# include <mln/metal/bexpr.hh>
# include <mln/metal/if.hh>

#define mln_trait_fun_is_assignable(Fun) typename mln::trait::fun::is_assignable< Fun >::ret
#define mln_trait_fun_is_assignable_(Fun) mln::trait::fun::is_assignable< Fun >::ret
#define mln_trait_fun_is_assignable__1comma(A, B) typename mln::trait::fun::is_assignable< A, B >::ret
#define mln_trait_fun_is_assignable__1comma_(A, B) mln::trait::fun::is_assignable< A, B >::ret

#define mln_trait_fun_is_parametrable(Fun) typename mln::trait::fun::is_parametrable< Fun >::ret
#define mln_trait_fun_is_parametrable_(Fun) mln::trait::fun::is_parametrable< Fun >::ret

#define mln_trait_fun_lvalue(Fun) typename mln::trait::fun::get_lvalue< Fun >::ret
#define mln_trait_fun_param(Fun) typename mln::trait::fun::get_param< Fun >::ret

namespace mln
{

  namespace trait
  {

    namespace fun
    {

      namespace internal
      {

	namespace introspect
	{

	  template <typename T>
	  struct except_void_t
	  {
	    typedef void ret;
	  };

	  template <>
	  struct except_void_t<void>;

	  template <typename T, typename V>
	  struct has_lvalue_t
	  {
	    typedef metal::false_ ret;
	    typedef void type;
	  };

	  template <typename T>
	  struct has_lvalue_t<T, typename except_void_t<typename T::lvalue>::ret>
	  {
	    typedef metal::true_ ret;
	    typedef typename T::lvalue type;
	  };

	  template <typename T, typename V>
	  struct has_param_t
	  {
	    typedef metal::false_ ret;
	    typedef void type;
	  };

	  template <typename T>
	  struct has_param_t<T, typename except_void_t<typename T::param>::ret>
	  {
	    typedef metal::true_ ret;
	    typedef typename T::param type;
	  };

	} // end of namespace mln::trait::fun::internal::introspect

      } // end of namespace mln::trait::fun::internal

      template <typename F>
      struct is_assignable
      {
	typedef typename internal::introspect::has_lvalue_t<F, void>::ret ret;
      };

      template <typename F>
      struct is_parametrable
      {
	typedef typename internal::introspect::has_param_t<F, void>::ret ret;
      };

      template <typename F>
      struct get_lvalue
      {
	typedef typename internal::introspect::has_lvalue_t<F, void>::type ret;
      };

      template <typename F>
      struct get_param
      {
	typedef typename internal::introspect::has_param_t<F, void>::type ret;
      };

    } // end of namespace mln::trait::fun

  } // end of namespace mln::trait

} // end of namespace mln

#endif // ! MLN_TRAIT_FUN_HH
