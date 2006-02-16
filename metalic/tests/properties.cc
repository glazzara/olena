#include <ostream>

#include <mlc/properties.hh>
#include <mlc/to_string.hh>
#include <mlc/cmp.hh>


// FIXME: Split this test into several smaller tests?  For instance,
// we have to test inheritance, properties/associated types,
// ``external properties'', etc.  The best approach is probably to
// browse mlc/properties.hh so as to make a list of the features to be
// checked.


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

  template <>
  struct set_default_props < category::my_cat >
  {
    typedef mlc::undefined ptr_type;
  };

  /* FIXME: In the current version of SCOOP 2, this ``packing'' is no
     longer done at this stage, but thanks to the `get_types'
     mechanism.  */
  /// Retrieval of any image type properties (FIXME: say 'packing').
  template <typename T>
  struct get_props < category::my_cat, T >
  {
    typedef char ptr_type;

    // FIXME: Same remark as in the above FIXME: this echo() method
    // should be place lower in the prop/type hierarchy.
    static void echo(std::ostream& ostr)
    {
      ostr << "props_of( oln::category::point, "
	   << mlc_to_string(T) << " ) =" << std::endl
	   << "{" << std::endl
	   << "\t ptr_type = " << mlc_to_string(ptr_type) << std::endl
	   << "}" << std::endl;
    }

    static void ensure()
    {
      mlc::is_ok< ptr_type >::ensure();
    }
  };

}

// Helper macro.
#define my_type_of_(FromType, Alias)		\
  mlc_type_of_(my, my::category::my_cat, FromType, Alias)

namespace my
{
  /*----.
  | A.  |
  `----*/

  // Forward declaration.
  struct A;

  // FIXME: Rename as set_types<> when mlc/properties.hh is updated.
  // Associated types.
  template<>
  struct set_props<category::my_cat, my::A>
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

  // FIXME: Is there any `set_super_type(T)'' sugar available?
  template<>
  struct set_super_type<my::B>
  {
    typedef A ret;
  };

  template<>
  struct set_props<category::my_cat, B>
  {
    // Note: foo_type is untouched here.

    // A type redefined here.
    typedef double bar_type;
    // A type defined here (but declared abstract in the super class).
    typedef char baz_type;
    // A type defined only here (and not in the super class).
    typedef long quux_type;
  };

  // FIXME: Is there any `set_super_type(T)'' sugar available?
  struct B : public internal::get_super_type<B>
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
