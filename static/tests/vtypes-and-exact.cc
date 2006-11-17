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

/// Check the virtual types system on a SCOOP hierarchy propagating the
/// exact (static) type throughout the inheritance tree (using
/// stc::any).

#include <mlc/cmp.hh>
#include <mlc/assert.hh>
#include <mlc/int.hh>

#include <stc/any.hh>
#include <stc/exact.hh>
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


  /*----------------------------------------.
  | A<Exact> --|> stc::any__simple<Exact>.  |
  `----------------------------------------*/

  // Forward declaration.
  template <typename Exact>
  struct A;

  /// Types associated to my::A.
  template <typename Exact>
  struct vtypes< A<Exact> >
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
    typedef A<Exact> self_t;
    typedef Exact exact_t;

    // Aliases.

    // META-FIXME: Update comment (stc/scoop.hh as changed since).

    /* FIXME: Work around a bug that affects both g++ 4.1 and
       gcc-snapshot (4.2) from Debian.  The versions of the compiler
       used at the moment this text was written were:

       g++ 4.1      : g++ (GCC) 4.1.2 20060920 (prerelease) (Debian 4.1.1-14)
       gcc-snapshot : g++ (GCC) 4.2.0 20060922 (experimental)


       Problem description:

       Using my_type_of (i.e., using stc_to_exact) breaks the
       assertions ``Ensure stc::is_any_ works properly.'' below (in
       main).  Using my_direct_type_of (on the exact type) solves
       this!  */
    typedef my_type_of(exact_t, foo) foo_t;
    typedef my_type_of(exact_t, bar) bar_t;
    typedef my_type_of(exact_t, baz) baz_t;
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
  struct vtypes< B<Exact> >
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
  struct single_vtype< B<Exact>, typedef_::yin_type >
  {
    typedef unsigned long ret;
  };

  template <typename Exact>
  struct B : public A<Exact>
  {
    typedef B<Exact> self_t;
    typedef Exact exact_t;

    // Aliases.

    // META-FIXME: Update comment (stc/scoop.hh as changed since).

    /* FIXME: Same as above; using my_type_of (i.e., using
       stc_to_exact) breaks the assertions ``Ensure stc::is_any_ works
       properly.'' below (in main).  Using my_direct_type_of (on the
       exact type) solves this!  */
    typedef my_type_of(exact_t, foo) foo_t;
    typedef my_type_of(exact_t, bar) bar_t;
    typedef my_type_of(exact_t, baz) baz_t;
    typedef my_type_of(exact_t, quux) quux_t;
    typedef my_type_of(exact_t, yin) yin_t;
  };


  /*--------------.
  | C --|> B<C>.  |
  `--------------*/

  // Forward declaration.
  struct C;

  // Super type.
  template <>
  struct set_super_type<C>
  {
    typedef B<C> ret;
  };

  /// Types associated to my::C.
  template <>
  struct vtypes<C>
  {
    // A type defined only here (and not in the super class).
    typedef double zorg_type;
  };

  struct C : public B<C>
  {
    typedef C self_t;
    typedef self_t exact_t;

    // Aliases.

    // META-FIXME: Update comment (stc/scoop.hh as changed since).

    /* FIXME: Same as above; using my_type_of_ (i.e., using
       stc_to_exact) breaks the assertions ``Ensure stc::is_any_ works
       properly.'' below (in main).  Using my_direct_type_of_ (on the
       exact type) solves this!  */
    typedef my_type_of_(exact_t, foo) foo_t;
    typedef my_type_of_(exact_t, quux) quux_t;
    typedef my_type_of_(exact_t, zorg) zorg_t;
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
  mlc::assert_<mlc_eq(A<C>::foo_t,  int)>::check();
  mlc::assert_<mlc_eq(A<C>::bar_t,  double)>::check();

  // Check types associated to B<C>.
  mlc::assert_<mlc_eq(B<C>::baz_t,  char)>::check();
  mlc::assert_<mlc_eq(B<C>::quux_t, long)>::check();
  mlc::assert_<mlc_eq(B<C>::yin_t,  unsigned long)>::check();

  mlc::assert_<mlc_eq(B<C>::bar_t,  A<C>::bar_t)>::check();

  // Check types associated to C.
  mlc::assert_<mlc_eq(C::foo_t,     int)>::check();
  mlc::assert_<mlc_eq(C::bar_t,     double)>::check();
  mlc::assert_<mlc_eq(C::baz_t,     char)>::check();
  mlc::assert_<mlc_eq(C::quux_t,    long)>::check();
  mlc::assert_<mlc_eq(C::yin_t,     unsigned long)>::check();
  mlc::assert_<mlc_eq(C::zorg_t,    double)>::check();
}
