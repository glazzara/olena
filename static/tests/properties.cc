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

// FIXME: Split this test into several smaller tests?  For instance,
// we have to test inheritance, properties/associated types,
// ``external properties'', etc.  The best approach is probably to
// browse stc/properties.hh so as to make a list of the features to be
// checked.

// Helper macro.
#define my_type_of_(FromType, Typedef)		\
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
    typedef int            foo_type;
    typedef float          bar_type;
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
  };

}


int
main()
{
  // Check types associated to A.
  mlc::assert_<mlc_eq(my::A::foo_type, int)>::check();
  mlc::assert_<mlc_eq(my::A::bar_type, float)>::check();

  // Check types associated to B.
  mlc::assert_<mlc_neq(my::B::bar_type, my::A::bar_type)>::check();
  mlc::assert_<mlc_eq(my::B::baz_type, char)>::check();
  mlc::assert_<mlc_eq(my::B::quux_type, long)>::check();
  mlc::assert_<mlc_eq(my::B::yin_type, unsigned long)>::check();

  // Check types associated to C.
  mlc::assert_<mlc_eq(my::C::foo_type, int)>::check();
  mlc::assert_<mlc_eq(my::C::quux_type, long)>::check();
  mlc::assert_<mlc_eq(my::C::zorg_type, double)>::check();
}
