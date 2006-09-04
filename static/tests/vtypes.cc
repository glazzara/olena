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

#include <stc/vtypes.hh>
#include <mlc/cmp.hh>
#include <mlc/assert.hh>
#include <mlc/int.hh>

// This test focuses on the virtual types system, so the exact type of
// classes is not propagated here (stc::any is not used).

// Helper macros.
#define my_type_of(FromType, Typedef)		\
  typename my_type_of_(FromType, Typedef)

#define my_type_of_(FromType, Typedef)				\
  stc_local_type_of_(my::category::my_cat, FromType, Typedef)

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


  /*----------------------.
  | Namespace equipment.  |
  `----------------------*/

  stc_equip_namespace_with_vtypes();


  /*-----------.
  | Category.  |
  `-----------*/

  // We only use one category here.
  namespace category
  {
    struct my_cat;
  }

  /// Packing of virtual types of any class belonging to the category my::cat.
  template <typename T>
  struct packed_vtypes_in_category<category::my_cat, T>
  {
    typedef my_type_of(T, foo) foo_type;
    typedef my_type_of(T, bar) bar_type;
    typedef my_type_of(T, baz) baz_type;

    static void ensure()
    {
      mlc::assert_< mlc_is_ok(foo_type) >::check();
      mlc::assert_< mlc_is_ok(bar_type) >::check();
      mlc::assert_< mlc_is_ok(baz_type) >::check();
    }
  };


  /*----.
  | A.  |
  `----*/

  // Forward declaration.
  struct A;

  /// Types associated to my::A.
  template <>
  struct vtypes_in_category<category::my_cat, my::A>
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

    ~A()
    {
      // packed_vtypes_in_category<category::my, A> is not checked here,
      // since A's baz_type virtual type is undefined.
    }
  };


  /*------------.
  | B ---|> A.  |
  `------------*/

  // Forward declaration.
  struct B;

  // Warning, this sugar might be removed in the future.
  stc_set_super(B, A);

  /// Types associated to my::B.
  template <>
  struct vtypes_in_category<category::my_cat, B>
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
  struct ext_vtype_in_category<category::my_cat, B, typedef_::yin_type>
  {
    typedef unsigned long ret;
  };

  struct B : public stc_get_supers(B)
  {
    // Aliases.
    typedef my_type_of_(B, foo)  foo_type;
    typedef my_type_of_(B, bar)  bar_type;
    typedef my_type_of_(B, baz)  baz_type;
    typedef my_type_of_(B, quux) quux_type;
    typedef my_type_of_(B, yin)  yin_type;

    // Check B's vtype.
    ~B()
    {
      packed_vtypes_in_category<category::my_cat, B>::ensure();
    }
  };


  /*---.
  | Z. |
  `---*/

  // Forward declaration.
  struct Z;

  // Z doesn't derive from B, but we want its vtypes to ``inherit''
  // from B's vtypes (see the specialization
  // types<category::my_cat, Z>).

  // Warning, this sugar might be removed in the future.
  /// Link to B (``pseudo'' inheritance).
  stc_set_pseudosuper(Z, B);

  /// Types associated to my::Z.
  template <>
  struct vtypes_in_category<category::my_cat, Z>
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

    // Check C's vtypes.
    ~Z()
    {
      packed_vtypes_in_category<category::my_cat, Z>::ensure();
    }
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
