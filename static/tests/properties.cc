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

#include <stc/properties.hh>
#include <mlc/cmp.hh>
#include <mlc/assert.hh>
#include <mlc/int.hh>

// This test focuses on the virtual types system, so the exact type of
// classes is not propagated here (stc::any is not used).

// Helper macros.
#define my_type_of(FromType, Typedef)		\
  typename my_type_of_(FromType, Typedef)

#define my_type_of_(FromType, Typedef)			\
  stc_typeof_(my::category::my_cat, FromType, Typedef)

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

  stc_equip_namespace_with_properties();


  /*-----------.
  | Category.  |
  `-----------*/

  // We only use one category here.
  namespace category
  {
    struct my_cat;
  }


  /*----.
  | A.  |
  `----*/

  // Forward declaration.
  struct A;

  /// Types associated to my::A.
  template<>
  struct vtypes<category::my_cat, my::A>
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

  /// Packing of virtual types of any A class.
  template <typename T>
  struct packed_vtypes <category::my_cat, T>
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

  struct A
  {
    // Aliases.
    typedef my_type_of_(A, foo) foo_type;
    typedef my_type_of_(A, bar) bar_type;
    typedef my_type_of_(A, baz) baz_type;

    ~A()
    {
      // packed_vtypes<category::my, A> is not checked here, since A's
      // baz_type virtual type is undefined.
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
  template<>
  struct vtypes<category::my_cat, B>
  {
    // (foo is left untouched.)

    // A type redefined here.
    typedef double bar_type;
    // A type defined here (but declared abstract in the super class).
    typedef char baz_type;
    // A type defined only here (and not in the super class).
    typedef long quux_type;
  };

  /// An external type associated to my::B.
  template<>
  struct ext_vtype<category::my_cat, B, typedef_::yin_type>
  {
    typedef unsigned long ret;
  };

  struct B : public stc_super_(B)
  {
    // Aliases.
    typedef my_type_of_(B, foo) foo_type;
    typedef my_type_of_(B, bar) bar_type;
    typedef my_type_of_(B, baz) baz_type;
    typedef my_type_of_(B, quux) quux_type;
    typedef my_type_of_(B, yin) yin_type;

    // Check B's vtypes.
    ~B()
    {
      packed_vtypes<category::my_cat, B>::ensure();
    }
  };


  /*---.
  | C. |
  `---*/

  // Forward declaration.
  struct C;

  // C doesn't derive from B, but we want its vtypes to ``inherit''
  // from B's vtypes (see the specialization
  // types<category::my_cat, C>).

  // Warning, this sugar might be removed in the future.
  /// Link to B (``pseudo'' inheritance).
  stc_set_pseudosuper(C, B);

  /// Types associated to my::C.
  template<>
  struct vtypes<category::my_cat, C>
  {
    // A type defined only here (and not in the super class).
    typedef double zorg_type;
  };

  struct C // no inheritance
  {
    // Aliases.
    typedef my_type_of_(C, foo) foo_type;
    typedef my_type_of_(C, quux) quux_type;
    typedef my_type_of_(C, zorg) zorg_type;

    // Check C's vtypes.
    ~C()
    {
      packed_vtypes<category::my_cat, C>::ensure();
    }
  };

}


int
main()
{
  // Check types associated to A.
  mlc::assert_<mlc_eq(my::A::foo_type, int)>::check();
  /* FIXME: Note that we use mlc_eqv, not mlc_eq, since the latter
     doesn't accept Metalic values.  Try to get rid of this
     limitation.  */
  mlc::assert_<mlc_eqv(my::A::bar_type, mlc::int_<42>)>::check();

  // Check types associated to B.
  /* FIXME: Note that we use mlc_neqv, not mlc_neq, since the latter
     doesn't accept Metalic values.  Try to get rid of this
     limitation.  */
  mlc::assert_<mlc_neqv(my::B::bar_type, my::A::bar_type)>::check();
  mlc::assert_<mlc_eq(my::B::baz_type, char)>::check();
  mlc::assert_<mlc_eq(my::B::quux_type, long)>::check();
  mlc::assert_<mlc_eq(my::B::yin_type, unsigned long)>::check();

  // Check types associated to C.
  mlc::assert_<mlc_eq(my::C::foo_type, int)>::check();
  mlc::assert_<mlc_eq(my::C::quux_type, long)>::check();
  mlc::assert_<mlc_eq(my::C::zorg_type, double)>::check();
}
