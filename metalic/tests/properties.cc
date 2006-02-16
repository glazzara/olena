#include <mlc/properties.hh>
#include <mlc/cmp.hh>


// FIXME: Split this test into several smaller tests?  For instance,
// we have to test inheritance, properties/associated types,
// ``external properties'', etc.  The best approach is probably to
// browse mlc/properties.hh so as to make a list of the features to be
// checked.

// Helper macro.
#define my_type_of_(FromType, Typedef)		\
  mlc_typeof_(my::category::my_cat, FromType, Typedef)

namespace my
{
  /*----------------------.
  | Namespace equipment.  |
  `----------------------*/

  mlc_equip_namespace_with_properties();


  /*-----------.
  | Typedefs.  |
  `-----------*/

  mlc_decl_typedef(ptr_type);
  mlc_decl_typedef(foo_type);
  mlc_decl_typedef(bar_type);
  mlc_decl_typedef(baz_type);
  mlc_decl_typedef(quux_type);


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

  // FIXME: Rename as set_types<> when mlc/properties.hh is updated.
  // Associated types.
  template<>
  struct set_types<category::my_cat, my::A>
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

  // Warning, this sugar might me remove from properties.hh.
  mlc_set_super(B, A);

  /// \brief Redefined types associated to \a B.
  ///
  /// Keeping the inheritance is absolutely capital here (i.e., when
  /// you redefine an associated type with redefine_types).
  template<>
  struct redefine_types<category::my_cat, B> :
    mlc_super_types_(category::my_cat, B)
  {
    // A type redefined here.
    typedef double bar_type;
    // A type defined here (but declared abstract in the super class).
    typedef char baz_type;
  };

  /// \brief New types associated to \a B.
  template<>
  struct set_types<category::my_cat, B>
  {
    // A type defined only here (and not in the super class).
    typedef long quux_type;
  };

  struct B : public mlc_super(B)
  {
    // Aliases.
    typedef my_type_of_(B, foo) foo_type;
    typedef my_type_of_(B, bar) bar_type;
    typedef my_type_of_(B, baz) baz_type;
    typedef my_type_of_(B, quux) quux_type;
  };
}

int
main()
{
  // Check associated types.
  mlc_eq(my::A::foo_type, int)::ensure ();
  mlc_eq(my::A::bar_type, float)::ensure ();

  // Check associated types.
  mlc_neq(my::B::bar_type, my::A::bar_type)::ensure ();
  mlc_eq(my::B::baz_type, char)::ensure ();
  mlc_eq(my::B::quux_type, long)::ensure ();
}
