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

// This test focuses on the support of virtual types in hierachies
// with multiple inheritance.

// FIXME: To do: Check extended vtypes, too.

/// Helper macros.
/// \{
#define my_type_of(FromType, Typedef)		\
  typename my_type_of_(FromType, Typedef)

#define my_type_of_(FromType, Typedef)				\
  stc_local_type_of_(my::category::my_cat, FromType, Typedef)
/// \}


namespace my
{
  /*-----------.
  | Typedefs.  |
  `-----------*/

  mlc_decl_typedef(foo_type);
  mlc_decl_typedef(bar_type);
  mlc_decl_typedef(baz_type);


  /*----------------------.
  | Namespace equipment.  |
  `----------------------*/

  stc_equip_namespace_with_vtypes();


  /*----------------.
  | Various types.  |
  `----------------*/

  struct alpha;
  struct beta;


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
  struct packed_vtypes <category::my_cat, T>
  {
    typedef my_type_of(T, foo) foo_type;

    static void ensure()
    {
      mlc::assert_< mlc_is_ok(foo_type) >::check();
    }
  };


  /*----.
  | A.  |
  `----*/

  // Forward declaration.
  struct A;

  /// Types associated to my::A.
  template <>
  struct vtypes<category::my_cat, my::A>
  {
    typedef int   foo_type;
    typedef alpha bar_type;
  };

  struct A
  {
    // Aliases.
    typedef my_type_of_(A, foo) foo_type;
    typedef my_type_of_(A, bar) bar_type;

    // Check A's vtypes.
    ~A()
    {
      packed_vtypes<category::my_cat, A>::ensure();
    }
  };


  /*--.
  | B |
  `--*/

  // Forward declaration.
  struct B;

  /// Types associated to my::B.
  template <>
  struct vtypes<category::my_cat, B>
  {
    typedef float foo_type;
    typedef beta  baz_type;
  };

  struct B
  {
    // Aliases.
    typedef my_type_of_(B, foo) foo_type;
    typedef my_type_of_(B, baz) baz_type;

    // Check B's vtypes.
    ~B()
    {
      packed_vtypes<category::my_cat, B>::ensure();
    }
  };


  /*------.
  | A   B |
  | ^   ^ |
  |  \ /  |
  |   C   |
  `------*/

  // Forward declaration.
  struct C;

  // Super classes.
  stc_set_nth_super(C, 1, A);
  stc_set_nth_super(C, 2, B);

  /// Types associated to my::C.
  template <>
  struct vtypes<category::my_cat, C>
  {
    // This typedef remove the ambiguity on the `foo' vtype: this vtype
    // is provided by both A and B.  Here, let `foo' take the value from
    // A's vtype (but we could also have chosen another value, like `int').
    typedef my_type_of_(A, foo) foo_type;
  };

  struct C : stc_get_supers(C)
  {
    // Aliases.
    typedef my_type_of_(C, foo) foo_type;
    typedef my_type_of_(C, bar) bar_type;
    typedef my_type_of_(C, baz) baz_type;

    // Check C's vtypes.
    ~C()
    {
      packed_vtypes<category::my_cat, C>::ensure();
    }
  };

} // end of namespace my


int
main()
{
  // Check types associated to A.
  mlc::assert_<mlc_eq(my::A::foo_type, int)>::check();
  mlc::assert_<mlc_eq(my::A::bar_type, my::alpha)>::check();

  // Check types associated to B.
  mlc::assert_<mlc_eq(my::B::foo_type, float)>::check();
  mlc::assert_<mlc_eq(my::B::baz_type, my::beta)>::check();

  // Check types associated to C.
  mlc::assert_<mlc_eq(my::C::foo_type, my::A::foo_type)>::check();
  mlc::assert_<mlc_eq(my::C::bar_type, my::alpha)>::check();
  mlc::assert_<mlc_eq(my::C::baz_type, my::beta)>::check();
}
