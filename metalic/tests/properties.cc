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

  mlc_equip_namespace_with_properties();


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

  // Warning, this sugar might me remove from properties.hh.
  mlc_set_super(B, A);

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

  struct B : public mlc_super_(B)
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

  // C do not derive from B, but we want its vtypes to ``inherit''
  // from B's vtypes (see the specilization
  // vtypes<category::my_cat, C>.

  /// Types associated to my::C.
  template<>
  struct vtypes<category::my_cat, C>
  {
    // FIXME: Having this link here is not elegant when you consider
    // ext_vtype<>: this means that even if you have only a vtype
    // declared as ext_vtype, you'll still have to define a
    // corresponding vtypes<>, at least to define the pseudosuper
    // class.  What about externalizing this information, maybe with
    // set_pseudosuper_type<>, and a macro set_pseudosuper() as sugar?

    /// Link to B (``pseudo'' inheritance).
    typedef B pseudosuper_type;

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
  mlc_eq(my::A::foo_type, int)::ensure ();
  mlc_eq(my::A::bar_type, float)::ensure ();

  // Check types associated to B.
  mlc_neq(my::B::bar_type, my::A::bar_type)::ensure ();
  mlc_eq(my::B::baz_type, char)::ensure ();
  mlc_eq(my::B::quux_type, long)::ensure ();
  mlc_eq(my::B::yin_type, unsigned long)::ensure ();

  // Check types associated to C.
  mlc_eq(my::C::foo_type, int)::ensure ();
  mlc_eq(my::C::quux_type, long)::ensure ();
  mlc_eq(my::C::zorg_type, double)::ensure ();
}
