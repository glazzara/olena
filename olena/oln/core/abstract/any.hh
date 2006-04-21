// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006 EPITA Research and
// Development Laboratory
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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
// 02110-1301 USA
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

#ifndef OLENA_CORE_ABSTRACT_ANY_HH
# define OLENA_CORE_ABSTRACT_ANY_HH

# include <mlc/any.hh>
# include <mlc/bool.hh>
# include <mlc/is_a.hh>


namespace oln {

  namespace abstract {


    template <typename E>
    struct any : public mlc::any<E, mlc::dispatch_policy::simple>
    {
    protected:
      typedef mlc::any<E, mlc::dispatch_policy::simple> super;
      any(E* exact_ptr) : super(exact_ptr) {}
      any() {}
    };


    template <typename E>
    struct any_best_speed :
      public mlc::any<E, mlc::dispatch_policy::best_speed>
    {
    protected:
      typedef mlc::any<E, mlc::dispatch_policy::best_speed> super;
      any_best_speed(E* exact_ptr = 0) : super(exact_ptr) {}
    };


  } // end of namespace oln::abstract

} // end of namespace oln


# define oln_is_any(Type)					\
  mlc::or_< mlc_is_a(Type, oln::abstract::any),			\
	    mlc_is_a(Type, oln::abstract::any_best_speed) >


#endif // ! OLENA_CORE_ABSTRACT_ANY_HH
