// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TRAIT_FUNCTIONS_HH
# define MLN_TRAIT_FUNCTIONS_HH

# include <mln/metal/bexpr.hh>
# include <mln/metal/if.hh>
# include <mln/fun/param.hh>
# include <mln/trait/next/solve.hh>

# define mln_trait_fun_is_assignable(Fun) typename mln::trait::function::is_assignable< Fun >::ret
# define mln_trait_fun_is_assignable_(Fun) mln::trait::function::is_assignable< Fun >::ret
# define mln_trait_fun_is_assignable__1comma(A, B) typename mln::trait::function::is_assignable< A, B >::ret
# define mln_trait_fun_is_assignable__1comma_(A, B) mln::trait::function::is_assignable< A, B >::ret

# define mln_trait_fun_is_parametrable(Fun) typename mln::trait::function::is_parametrable< Fun >::ret
# define mln_trait_fun_is_parametrable_(Fun) mln::trait::function::is_parametrable< Fun >::ret

# define mln_trait_fun_lvalue(Fun) typename mln::trait::function::get_lvalue< Fun >::ret
# define mln_trait_fun_param(Fun) typename mln::trait::function::get_param< Fun >::ret
# define mln_trait_fun_storage(Fun) typename mln::trait::function::get_storage< Fun >::ret

namespace mln
{

  namespace trait
  {

    namespace function
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

	  // Lvalue solver

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

	  // Parameter solver
	  template <typename T, typename V>
	  struct param_solver;

	  template <typename T, typename V>
	  struct param_flag_solver
	  {
	    typedef typename mln::fun::parameter<T> ret;
	  };

	  template <typename T>
	  struct param_flag_solver<T, typename except_void_t<typename mln::fun::parameter<typename T::flag>::param>::ret>
	  {
	    typedef mln::fun::parameter<typename T::flag> ret;
	  };

	  template <typename T, typename V>
	  struct param_def_solver
	  {
	    typedef typename param_flag_solver<T, V>::ret ret;
	  };

	  template <typename T>
	  struct param_def_solver<T, typename except_void_t<typename mln::fun::parameter<typename T::def>::param>::ret>
	  {
	    typedef mln::fun::parameter<typename T::def> ret;
	  };

	  template <typename T, typename V>
	  struct param_solver : param_def_solver<T, V>
	  {
	  };

	  template <typename T>
	  struct param_solver<T, typename except_void_t<typename T::param>::ret>
	  {
	    typedef T ret;
	  };

	  template <typename T, typename V>
	  struct has_param_t
	  {
	    typedef metal::false_ ret;
	    typedef void type;
	  };

	  template <typename T>
	  struct has_param_t<T, typename except_void_t<typename param_solver<T,void>::ret::param>::ret>
	  {
	    typedef metal::true_ ret;
	    typedef typename param_solver<T,void>::ret::param type;
	  };

	  template <typename T, typename V>
	  struct storage_solver;

	  template <typename T, typename V>
	  struct storage_flag_solver
	  {
	    typedef typename mln::fun::parameter<T> ret;
	  };

	  template <typename T>
	  struct storage_flag_solver<T, typename except_void_t<typename mln::fun::parameter<typename T::flag>::storage>::ret>
	  {
	    typedef mln::fun::parameter<typename T::flag> ret;
	  };

	  template <typename T, typename V>
	  struct storage_def_solver
	  {
	    typedef typename storage_flag_solver<T, V>::ret ret;
	  };

	  template <typename T>
	  struct storage_def_solver<T, typename except_void_t<typename mln::fun::parameter<typename T::def>::storage>::ret>
	  {
	    typedef mln::fun::parameter<typename T::def> ret;
	  };

	  template <typename T, typename V>
	  struct storage_solver : storage_def_solver<T, V>
	  {
	  };

	  template <typename T>
	  struct storage_solver<T, typename except_void_t<typename T::storage>::ret>
	  {
	    typedef T ret;
	  };

	  template <typename T, typename V>
	  struct has_storage_t
	  {
	    typedef has_param_t<T, V> has_param;

	    typedef metal::false_ ret;
	    typedef typename has_param::type type;

	    template <typename U>
	    static inline
	    const U& compute(const U& t)
	    {
	      return t;
	    }

	  };

	  template <typename T>
	  struct has_storage_t<T, typename except_void_t<typename param_solver<T,void>::ret::storage>::ret>
	  {
	    typedef metal::true_ ret;
	    typedef typename param_solver<T,void>::ret def;

	    typedef typename def::storage type;

	    template <typename U>
	    static inline
	    type compute(const U& p)
	    {
	      return def::compute(p);
	    }

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

      template <typename F>
      struct get_storage
      {
	typedef typename internal::introspect::has_storage_t<F, void>::type ret;
      };

    } // end of namespace mln::trait::fun

  } // end of namespace mln::trait

} // end of namespace mln

#endif // ! MLN_TRAIT_FUNCTIONS_HH
