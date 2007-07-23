// Copyright (C) 2006, 2007 EPITA Research and Development Laboratory
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
#define my_type_of_(FromType, Alias)			\
  my::find_vtype< FromType, my::typedef_:: Alias >::ret

#define my_type_of(FromType, Alias)		\
  typename my_type_of_(FromType, Alias)




namespace my
{
  /*----------------------.
  | Namespace equipment.  |
  `----------------------*/

#include <stc/scoop.hxx>


  /*-----------.
  | Typedefs.  |
  `-----------*/

  mlc_decl_typedef(foo);
  mlc_decl_typedef(bar);
  mlc_decl_typedef(baz);
  mlc_decl_typedef(quux);
  mlc_decl_typedef(yin);
  mlc_decl_typedef(zorg);


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
    typedef int            foo;
    // A Metalic value, used here is to ensure that
    // mlc::abstract::values are accepted as virtual types, as well as
    // any other type).
    typedef mlc::int_<42>  bar;
    // An undefined type.
    typedef mlc::undefined baz;
  };

  struct A
  {
    // Aliases.
    typedef my_type_of_(A, foo) foo;
    typedef my_type_of_(A, bar) bar;
    typedef my_type_of_(A, baz) baz;
  };


  /*------------.
  | B ---|> A.  |
  `------------*/

  // Forward declaration.
  struct B;

  // Set super type.
  template<>
  struct super_trait_<B>
  {
    typedef A ret;
  };

  /// Types associated to my::B.
  template <>
  struct vtypes<B>
  {
    // (foo is left untouched.)

    // A type redefined here.
    typedef double bar;
    // A type defined here (but declared abstract in the super class).
    typedef char baz;
    // A type defined only here (and not in the super class).
    typedef long quux;
  };

  /// An extended type associated to my::B.
  template <>
  struct single_vtype<B, typedef_::yin>
  {
    typedef unsigned long ret;
  };

  struct B : public A
  {
    // Aliases.
    typedef my_type_of_(B, foo)  foo;
    typedef my_type_of_(B, bar)  bar;
    typedef my_type_of_(B, baz)  baz;
    typedef my_type_of_(B, quux) quux;
    typedef my_type_of_(B, yin)  yin;
  };


  /*---.
  | Z. |
  `---*/

  // Forward declaration.
  struct Z;

  // Z doesn't derive from B, but we want its vtypes to ``inherit''
  // from B's vtypes.

  /// Types associated to my::Z.
  template <>
  struct vtypes<Z>
  {
    /// Delegation to B (used to be called ``pseudo'' inheritance).
    typedef B delegatee;

    // A type defined only here (and not in the super class).
    typedef double zorg;
  };

  struct Z // no inheritance
  {
    // Aliases.
    typedef my_type_of_(Z, foo)  foo;
    typedef my_type_of_(Z, bar)  bar;
    typedef my_type_of_(Z, baz)  baz;
    typedef my_type_of_(Z, quux) quux;
    typedef my_type_of_(Z, yin)  yin;
    typedef my_type_of_(Z, zorg) zorg;
  };

} // end of namespace my


int
main()
{
  // Check types associated to A.
  mlc::assert_<mlc_eq(my::A::foo,  int)>::check();
  mlc::assert_<mlc_eq(my::A::bar,  mlc::int_<42>)>::check();

  // Check types associated to B.
  mlc::assert_<mlc_eq(my::B::baz,  char)>::check();
  mlc::assert_<mlc_eq(my::B::quux, long)>::check();
  mlc::assert_<mlc_eq(my::B::yin,  unsigned long)>::check();

  mlc::assert_<mlc_neq(my::B::bar, my::A::bar)>::check();

  // Check types associated to Z.
  mlc::assert_<mlc_eq(my::Z::foo,  int)>::check();
  mlc::assert_<mlc_eq(my::Z::bar,  double)>::check();
  mlc::assert_<mlc_eq(my::Z::baz,  char)>::check();
  mlc::assert_<mlc_eq(my::Z::quux, long)>::check();
  mlc::assert_<mlc_eq(my::Z::yin,  unsigned long)>::check();
  mlc::assert_<mlc_eq(my::Z::zorg, double)>::check();
}
