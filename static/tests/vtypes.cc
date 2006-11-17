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

/// This test focuses on the virtual types system, so the exact type of
/// classes is not propagated here (stc::any is not used).

#include <mlc/cmp.hh>
#include <mlc/assert.hh>
#include <mlc/int.hh>

#include <stc/scoop.hh>


// Helper macros.
#define my_type_of_(FromType, Alias)					\
  my::direct_type_of_<FromType, my::typedef_:: Alias##_type>::ret

#define my_type_of(FromType, Alias)		\
  typename my_type_of_(FromType, Alias)


// Namespace equipment.
stc_scoop_equipment_for_namespace(my);


namespace my
{
  /*-----------.
  | Typedefs.  |
  `-----------*/

  mlc_decl_typedef(foo_type);
  mlc_decl_typedef(bar_type);
  mlc_decl_typedef(baz_type);
  mlc_decl_typedef(quux_type);
  mlc_decl_typedef(yin_type);
  mlc_decl_typedef(zorg_type);


  /*----.
  | A.  |
  `----*/

  // Forward declaration.
  struct A;

  /// Types associated to my::A.
  template <>
  struct vtypes<my::A>
  {
    // A native type.
    typedef int            foo_type;
    // A Metalic value, used here is to ensure that
    // mlc::abstract::values are accepted as virtual types, as well as
    // any other type).
    typedef mlc::int_<42>  bar_type;
    // An undefined type.
    typedef mlc::undefined baz_type;
  };

  struct A
  {
    // Aliases.
    typedef my_type_of_(A, foo) foo_type;
    typedef my_type_of_(A, bar) bar_type;
    typedef my_type_of_(A, baz) baz_type;
  };


  /*------------.
  | B ---|> A.  |
  `------------*/

  // Forward declaration.
  struct B;

  // Set super type.
  template<>
  struct set_super_type<B>
  {
    typedef A ret;
  };

  /// Types associated to my::B.
  template <>
  struct vtypes<B>
  {
    // (foo is left untouched.)

    // A type redefined here.
    typedef double bar_type;
    // A type defined here (but declared abstract in the super class).
    typedef char baz_type;
    // A type defined only here (and not in the super class).
    typedef long quux_type;
  };

  /// An extended type associated to my::B.
  template <>
  struct single_vtype<B, typedef_::yin_type>
  {
    typedef unsigned long ret;
  };

  struct B : public A
  {
    // Aliases.
    typedef my_type_of_(B, foo)  foo_type;
    typedef my_type_of_(B, bar)  bar_type;
    typedef my_type_of_(B, baz)  baz_type;
    typedef my_type_of_(B, quux) quux_type;
    typedef my_type_of_(B, yin)  yin_type;
  };


  /*---.
  | Z. |
  `---*/

  // Forward declaration.
  struct Z;

  // Z doesn't derive from B, but we want its vtypes to ``inherit''
  // from B's vtypes (see the specialization
  // types<category::my_cat, Z>).

  /// Link to B (``pseudo'' inheritance).
  template<>
  struct set_super_type<Z>
  {
    typedef B ret;
  };

  /// Types associated to my::Z.
  template <>
  struct vtypes<Z>
  {
    // A type defined only here (and not in the super class).
    typedef double zorg_type;
  };

  struct Z // no inheritance
  {
    // Aliases.
    typedef my_type_of_(Z, foo)  foo_type;
    typedef my_type_of_(Z, bar)  bar_type;
    typedef my_type_of_(Z, baz)  baz_type;
    typedef my_type_of_(Z, quux) quux_type;
    typedef my_type_of_(Z, yin)  yin_type;
    typedef my_type_of_(Z, zorg) zorg_type;
  };

} // end of namespace my


int
main()
{
  // Check types associated to A.
  mlc::assert_<mlc_eq(my::A::foo_type,  int)>::check();
  mlc::assert_<mlc_eq(my::A::bar_type,  mlc::int_<42>)>::check();

  // Check types associated to B.
  mlc::assert_<mlc_eq(my::B::baz_type,  char)>::check();
  mlc::assert_<mlc_eq(my::B::quux_type, long)>::check();
  mlc::assert_<mlc_eq(my::B::yin_type,  unsigned long)>::check();

  mlc::assert_<mlc_neq(my::B::bar_type, my::A::bar_type)>::check();

  // Check types associated to Z.
  mlc::assert_<mlc_eq(my::Z::foo_type,  int)>::check();
  mlc::assert_<mlc_eq(my::Z::bar_type,  double)>::check();
  mlc::assert_<mlc_eq(my::Z::baz_type,  char)>::check();
  mlc::assert_<mlc_eq(my::Z::quux_type, long)>::check();
  mlc::assert_<mlc_eq(my::Z::yin_type,  unsigned long)>::check();
  mlc::assert_<mlc_eq(my::Z::zorg_type, double)>::check();
}
