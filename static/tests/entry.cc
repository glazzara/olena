/// Test the extensible entry points in a class hierarchy.

#include <stc/entry.hh>

#include <mlc/case.hh>
#include <mlc/assert.hh>
#include <mlc/is_a.hh>

// FIXME: Fake.  Remove this when mlc::abort_ is added to the project.
namespace mlc
{
  template <typename T>
  class abort_ {};
}


/*----------.
| Library.  |
`----------*/

/* Simplified class diagram.


				/top/
				  ^
				  |
		     ,------------+------------.
		     |                         |
	       /property1/                 /property2/
		     ^                         ^
		     |                         |
	       ,-----+-----.             ,-----+-----.
	       |           |             |           |
      /property1a/   /property1b/   /property2a/   /property2b/
	       |           |             |           |
	       o           o             o           o
		     o                         o
		     |                         |
	 stc::set_entry_node<C,1>    stc::set_entry_node<C,1>
	     (first selector)           (second selector)
		     ^                         ^
		     |                         |

		     `------------+------------'
				  |
				entry
				  ^
				  |
				 foo


   In fact, this is closer to this :

				/top/
				  ^
				  |
				  |
			     /property2/
				  ^
				  |
			    ,-----+-----.
			    |           |
		     /property2a/   /property2b/
			    |           |
			    o           o
				  o
				  |
		      stc::set_entry_node<foo,2>
			  (second selector)
				  ^
				  |
			     /property1/
				  ^
				  |
			    ,-----+-----.
			    |           |
		     /property1a/   /property1b/
			    |           |
			    o           o
				  o
				  |
		      stc::set_entry_node<foo,1>
			   (first selector)
				  ^
				  |
				entry
				  ^
				  |
				 foo

*/

namespace my_lib
{
  // Virtual types.
  // We don't use all the equipment of stc/properties.hh for
  // simplicity purpose.
  template <typename T>
  struct vtypes
  {
    // Nothing.
  };

  /// Top of the hierachy.
  struct top {};

  /// Property 1.
  /// \{
  struct property1 : virtual public top {};
  struct property1a : public property1 {};
  struct property1b : public property1 {};
  /// \}

  /// Property 2.
  /// \{
  struct property2 : virtual public top {};
  struct property2a : public property2 {};
  struct property2b : public property2 {};
  /// \}
}

mlc_case_equipment_for_namespace(my_lib);


// -------------------------- //
// Equipment for property 1.  //
// -------------------------- //

namespace my_lib
{
  // Switch on property 1.
  /// \{
  struct prop1_tag;

  template <typename prop1_type>
  struct case_<prop1_tag, prop1_type, 1> :
    // Test.
    public mlc::where_< mlc_is_a(prop1_type, int) >
  {
    // Super class if test succeed.
    typedef property1a ret;
  };

  template <typename prop1_type>
  struct case_<prop1_tag, prop1_type, 2> :
    // Test.
    public mlc::where_< mlc_is_a(prop1_type, long) >
  {
    // Super class if test succeed.
    typedef property1b ret;
  };

  template <typename prop1_type>
  struct default_case_<prop1_tag, prop1_type>
  {
    typedef mlc::abort_<prop1_tag> ret;
  };
  /// \}

} // End of namespace my_lib

// FIXME: Maybe set/get_entry_node should be moved inside a macro?
namespace stc
{
  /// Property 1.
  /// E is for Exact.
  template <typename E>
  struct set_entry_node<E, 1> :
    public my_lib::switch_<my_lib::prop1_tag,
			   typename my_lib::vtypes<E>::prop1>::ret
  {
  };
}

// -------------------------- //
// Equipment for property 2.  //
// -------------------------- //

namespace my_lib
{
  // Switch on property 2.
  /// \{
  struct prop2_tag;

  template <typename prop2_type>
  struct case_<prop2_tag, prop2_type, 1> :
    // Test.
    public mlc::where_< mlc_is_a(prop2_type, float) >
  {
    // Super class if test succeed.
    typedef property2a ret;
  };

  template <typename prop2_type>
  struct case_<prop2_tag, prop2_type, 2> :
    // Test.
    public mlc::where_< mlc_is_a(prop2_type, double) >
  {
    // Super class if test succeed.
    typedef property2b ret;
  };

  template <typename prop2_type>
  struct default_case_<prop2_tag, prop2_type>
  {
    typedef mlc::abort_<prop2_tag> ret;
  };
  /// \}

} // End of namespace my_lib

// FIXME: Maybe set/get_entry_node should be moved inside a macro?
namespace stc
{
  /// Property 2.
  /// E is for Exact.
  template <typename E>
  struct set_entry_node<E, 2> :
    public my_lib::switch_<my_lib::prop2_tag,
			   typename my_lib::vtypes<E>::prop2>::ret
  {
  };
}


/*--------------.
| Client code.  |
`--------------*/

// Fwd decl.
namespace client
{
  struct foo;
}

// client::foo's vtypes.
namespace my_lib
{
  template <>
  struct vtypes<client::foo>
  {
    typedef int prop1;
    typedef double prop2;
  };
}

namespace client
{
  struct foo : public stc::entry<foo>
  {
  };
}


int
main ()
{
  mlc::assert_<mlc_is_a_(client::foo, my_lib::top)>::check();

  mlc::assert_<mlc_is_a_(client::foo, my_lib::property1)>::check();
  mlc::assert_<mlc_is_a_(client::foo, my_lib::property1a)>::check();
  mlc::assert_<mlc_is_not_a_(client::foo, my_lib::property1b)>::check();

  mlc::assert_<mlc_is_a_(client::foo, my_lib::property2)>::check();
  mlc::assert_<mlc_is_not_a_(client::foo, my_lib::property2a)>::check();
  mlc::assert_<mlc_is_a_(client::foo, my_lib::property2b)>::check();
}
