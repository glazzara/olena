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

#include <stc/any.hh>
#include <stc/vtypes.hh>
#include <mlc/cmp.hh>
#include <mlc/assert.hh>
#include <mlc/int.hh>

// Check the virtual types system on a SCOOP hierarchy propagating the
// exact (static) type throughout the inheritance tree (using
// stc::any).

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


  /*----------------------------------------.
  | A<Exact> --|> stc::any__simple<Exact>.  |
  `----------------------------------------*/

  // Forward declaration.
  template <typename Exact>
  struct A;

  /// Types associated to my::A.
  template <typename Exact>
  struct vtypes_in_category< category::my_cat, A<Exact> >
  {
    // A native type.
    typedef int            foo_type;
    // A Metalic value, used here to ensure that mlc::abstract::values
    // are accepted as virtual types, as well as any other type).
    typedef mlc::int_<42>  bar_type;
    // An undefined type.
    typedef mlc::undefined baz_type;
  };

  template <typename Exact>
  struct A : public stc::any<Exact>
  {
    // Aliases.
    typedef my_type_of(A, foo) foo_type;
    typedef my_type_of(A, bar) bar_type;
    typedef my_type_of(A, baz) baz_type;
  };


  /*--------------------------.
  | B<Exact> ---|> A<Exact>.  |
  `--------------------------*/

  // Forward declaration.
  template <typename Exact>
  struct B;

  // Super type.
  template <typename Exact>
  struct set_super_type< B<Exact> >
  {
    typedef A<Exact> ret;
  };

  /// Types associated to my::B.
  template <typename Exact>
  struct vtypes_in_category< category::my_cat, B<Exact> >
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
  template <typename Exact>
  struct ext_vtype_in_category<category::my_cat, B<Exact>, typedef_::yin_type>
  {
    typedef unsigned long ret;
  };

  template <typename Exact>
  struct B : public stc_get_supers(B<Exact>)
  {
    // Aliases.
    typedef my_type_of(B, foo) foo_type;
    typedef my_type_of(B, bar) bar_type;
    typedef my_type_of(B, baz) baz_type;
    typedef my_type_of(B, quux) quux_type;
    typedef my_type_of(B, yin) yin_type;
  };


  /*--------------.
  | C --|> B<C>.  |
  `--------------*/

  // Forward declaration.
  struct C;

  // Super type.
  stc_set_super(C, B<C>);

  /// Types associated to my::C.
  template <>
  struct vtypes_in_category<category::my_cat, C>
  {
    // A type defined only here (and not in the super class).
    typedef double zorg_type;
  };

  struct C : public stc_get_supers(C)
  {
    // Aliases.
    typedef my_type_of_(C, foo) foo_type;
    typedef my_type_of_(C, quux) quux_type;
    typedef my_type_of_(C, zorg) zorg_type;
  };

} // end of namespace my


int
main()
{
  using my::A;
  using my::B;
  using my::C;

  // Check super classes.
  mlc::assert_<mlc_is_a_(C, A)>::check();
  mlc::assert_<mlc_is_a_(C, B)>::check();

  // Ensure stc::is_any_ works properly.
  mlc::assert_< stc::is_any_< A<C> > >::check();
  mlc::assert_< stc::is_any_< B<C> > >::check();
  mlc::assert_< stc::is_any_< C >    >::check();    

  // Check exact types of A<C> and B<C>.
  mlc::assert_<mlc_eq(stc_to_exact_(A<C>), C)>::check();
  mlc::assert_<mlc_eq(stc_to_exact_(B<C>), C)>::check();


  // Check types associated to A<C>.
  mlc::assert_<mlc_eq(A<C>::foo_type,  int)>::check();
  mlc::assert_<mlc_eq(A<C>::bar_type,  double)>::check();

  // Check types associated to B<C>.
  mlc::assert_<mlc_eq(B<C>::baz_type,  char)>::check();
  mlc::assert_<mlc_eq(B<C>::quux_type, long)>::check();
  mlc::assert_<mlc_eq(B<C>::yin_type,  unsigned long)>::check();

  mlc::assert_<mlc_eq(B<C>::bar_type,  A<C>::bar_type)>::check();

  // Check types associated to C.
  mlc::assert_<mlc_eq(C::foo_type,     int)>::check();
  mlc::assert_<mlc_eq(C::bar_type,     double)>::check();
  mlc::assert_<mlc_eq(C::baz_type,     char)>::check();
  mlc::assert_<mlc_eq(C::quux_type,    long)>::check();
  mlc::assert_<mlc_eq(C::yin_type,     unsigned long)>::check();
  mlc::assert_<mlc_eq(C::zorg_type,    double)>::check();
}
