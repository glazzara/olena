// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006  EPITA Research and
// Development Laboratory.
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


#ifndef STATIC_ANY_HH
# define STATIC_ANY_HH

/// \file stc/any.hh
/// \brief Implementation of stc::any classes (top of static hierachies).


# include <mlc/contract.hh>


// \def Shortcut.
#define any_mem(E) any<E, dispatch_policy::best_memory>


namespace stc
{

  /// Tags selecting a dispatch for stc::any.
  struct dispatch_policy
  {
    struct best_speed;
    struct best_memory;
    struct simple;
  };


  /// Top class of a static hierachy.
  template <typename E, typename Dispatch_Policy = dispatch_policy::simple>
  struct any;


  /*-----------------------.
  | stc::any__best_speed.  |
  `-----------------------*/

  /// "Best speed" version of `any'.
  template <typename E>
  struct any <E,
              dispatch_policy::best_speed>
  {
    typedef E exact_t;

    E& exact() {
      precondition(exact_ptr != 0);
      return *exact_ptr;
    }
    const E& exact() const {
      precondition(exact_ptr != 0);
      return *exact_ptr;
    }

    /// Assignment (op=).
    any& operator=(const any& rhs)
    {
      // do nothing
      // so that "this->exact_ptr" is unchanged
      return *this;
    }

    /// Copy Ctor.
    any(const any& rhs) :
      // For safety purpose.
      exact_ptr(0)
    {
      // this->exact_ptr is *not* copied; its value has to be set in
      // subclasses copy ctors.
    }

  protected:
    any(E* exact_ptr) : exact_ptr(exact_ptr) {}
    virtual ~any() {}

    /// Sole attribute.
    E* exact_ptr;
  };


  template <typename E>
  struct any__best_speed : public any<E, dispatch_policy::best_speed>
  {
  protected:
    typedef any<E, dispatch_policy::best_speed> super;
    any__best_speed(E* exact_ptr = 0) : super(exact_ptr) {}
  };


  /*------------------------.
  | stc::any__best_memory.  |
  `------------------------*/

  /// "Best memory" version of 'any'
  /* FIXME:
     We should test with autotools if the compiler allows us to write a
     static_cast or not (diamond inheritance problem). gcc-2.95 gives a
     different adress but not gcc-3.
     On Mac OS X, the compilation fails with a strange link error if you
     try to use the offset computation.

     2006-03-24 Roland: Investigate this bug/behavior of Apple's GCC.
     This is not the first time that Apple GCC 3.3 behaves strangely. */
  template <typename E>
  struct any <E,
	      dispatch_policy::best_memory>
  {
    typedef E exact_t;

    E& exact() {
# if defined __GNUC__ && __GNUC__ >= 3
      return static_cast<E&>(*this);
# else
      return *(E*)((char*)this - exact_offset);
# endif
    }
    const E& exact() const {
# if defined __GNUC__ && __GNUC__ >= 3
      return static_cast<const E&>(*this);
# else
      return *(const E*)((const char*)this - exact_offset);
# endif
    }

# if not defined  __GNUC__ || __GNUC__ < 3
    static const int exact_offset;
    static const E exact_obj;
    static const any_mem(E)& ref_exact_obj;
# endif

  protected:
    any() {}
    // For safety purpose.
    any(E* exact_ptr_does_not_exist_in_this_version);
    // FIXME: virtual for a "best memory" version!?
    virtual ~any() {}
  };

  template <typename E>
  struct any__best_memory : public any<E, dispatch_policy::best_memory>
  {
  protected:
    typedef any<E, dispatch_policy::best_memory> super;
    any__best_memory() : super() {}
  };

# if not defined  __GNUC__ || __GNUC__ < 3
  template <typename E> const E any_mem(E)::exact_obj = E();
  template <typename E> const any_mem(E)& any_mem(E)::ref_exact_obj =
  any_mem(E)::exact_obj;
  template <typename E>const int any_mem(E)::exact_offset =
  (const char*)(void*)(&any_mem(E)::ref_exact_obj)
    - (const char*)(void*)(&any_mem(E)::exact_obj);
# endif


  /*-------------------.
  | stc::any__simple.  |
  `-------------------*/

  /// "Simple" version of `any'.
  template <typename E>
  struct any <E,
              dispatch_policy::simple>
  {
    typedef E exact_t;

    E& exact() {
      return *(E*)(void*)this;
    }
    const E& exact() const {
      return *(const E*)(const void*)this;
    }
  protected:
    any() {}
    // For safety purpose.
    any(E* exact_ptr_does_not_exist_in_this_version);
  };

  template <typename E>
  struct any__simple : public any<E, dispatch_policy::simple>
  {
  protected:
    typedef any<E, dispatch_policy::simple> super;
    any__simple() : super() {}
  };

} // end of namespace stc


#endif // ! STATIC_ANY_HH
