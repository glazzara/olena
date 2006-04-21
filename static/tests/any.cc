// Copyright (C) 2006 EPITA Research and Development Laboratory
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

/// Test stc::any.

#include <stc/any.hh>
#include <mlc/cmp.hh>
#include <mlc/assert.hh>
#include <mlc/logic.hh>
#include <mlc/is_a.hh>

// FIXME: Test other flavors of stc::any.

/* Linear hierachy.

   - Abstractions.

                           stc::any<Exact>
                                  ^
                                  |
                               A<Exact>
                                  ^
                                  |
                               B<Exact>
                                  ^
                                  |
                                Exact


   -  Concrete class C, deriving from B.

                             stc::any<C>
                                  ^
                                  |
                                 A<C>
                                  ^
                                  |
                                 B<C>
                                  ^
                                  |
                                  C

*/

template <typename Exact>
struct A : public stc::any__simple< Exact > {};

template <typename Exact>
struct B : public A< Exact > {};

struct C : public B<C> {};


/* A more complex hierarchy (several subclasses, diamond inheritance).

   - Abstractions.

                           stc::any<Exact>
                                  ^
                                  |
                               T<Exact>
                               ^  ^   ^
                              /   |    \
                             /    |     \
                     U<Exact>  X<Exact>  Y<Exact>
                        ^         ^         ^
                        |         |         |
                      Exact     Exact     Exact

   - Concrete classes V and Z, deriving from U and from X and Y resp.


                 stc::any<Z>          stc::any<Z>
                     ^                    ^
                     |                    |
                    T<Z>                 T<Z>
                     ^                    ^
                     |                  /   \
                    U<Z>              X<Z>  Y<Z>
                     ^                 ^     ^
                     |                  \   /
                     V                    Z

*/

template <typename Exact>
struct T : public stc::any__simple< Exact > {};

template <typename Exact>
struct U : public T< Exact > {};

struct V : public U<V> {};

template <typename Exact>
struct X : virtual public T< Exact > {};

template <typename Exact>
struct Y : virtual public T< Exact > {};

struct Z : public X<Z>, public Y<Z> {};

// The struct enclosing these two functions (methods) is used to
// factor the code w.r.t. the tested hierarchy.
template <template < class > class Top>
struct assuming_top_class_is
{
  template <template < class > class T, typename E>
  static void
  test_exact_is_a(Top<E>& a)
  {
    typedef typeof(a.exact()) exact_type;
    
    mlc::assert_< mlc::eq_<exact_type, E> >::check();
    mlc::assert_< mlc_is_a(exact_type, T) >::check();
  };

  template <template <class> class T, typename E>
  static void
  test_exact_is_not_a(Top<E>& a)
  {
    typedef typeof(a.exact()) exact_type;
    
    mlc::assert_< mlc::eq_<exact_type, E> >::check();
    mlc::assert_< mlc_is_not_a(exact_type, T) >::check();
  }
};

int
main()
{
  C c;
  assuming_top_class_is<A>::test_exact_is_a<A>(c);
  assuming_top_class_is<A>::test_exact_is_a<B>(c);

  Z z;
  assuming_top_class_is<T>::test_exact_is_a<T>(z);
  assuming_top_class_is<T>::test_exact_is_a<X>(z);
  assuming_top_class_is<T>::test_exact_is_a<Y>(z);

  X<Z> x;
  assuming_top_class_is<T>::test_exact_is_a<T>(x);
  // Bad branch.
  assuming_top_class_is<T>::test_exact_is_not_a<U>(x);
  // Bad hierachy.
  assuming_top_class_is<T>::test_exact_is_not_a<B>(x);
}
