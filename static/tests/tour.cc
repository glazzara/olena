// Copyright (C) 2006  EPITA Research and Development Laboratory.
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

/// \file tests/tour.cc
/// \brief A tour of Static and SCOOP features.

#include <cassert>

#include <mlc/assert.hh>
#include <mlc/contract.hh>
#include <mlc/is_a.hh>
#include <mlc/cmp.hh>

#include <stc/any.hh>
#include <stc/exact.hh>
#include <stc/find_exact.hh>

// FIXME: ``Fil rouge'': building a small (but complete) static
// hierarchy, using all features from Static. Possibly, this
// construction could be progressive, i.e., made of several attempts,
// each enclosed in its own namespace.
//
// Be as simple as possible, and introduce features one by one. For
// instance, don't introduce the multiple inheritance system after
// having introduced the vtypes.

// FIXME: Things to show:
// - exact (more)
// - super
// - virtual types (and macros for super)
// - multiple super
// - entry


// Welcome to this tour of the Static library!

// FIXME: Intro.


/*----------------------------.
| A small dynamic hierarchy.  |
`----------------------------*/

/* Our first step in this tour will be the definition of a static
   library.  In classic (dynamic) OO programming, our example would
   look like this:  */

namespace dynamic_hierarchy
{

  // `A' is an abstract class.
  struct A
  {
    // A virtual method.
    virtual int foo() { return 1; }
    // A virtual pure method.
    virtual int bar() = 0;
  };

  // `B' is a concrete class.
  struct B
  {
    // A redefined method.
    virtual int foo() { return 2; }
    // `B::bar' is defined.
    virtual int bar() { return 3; }
  };

  // `C' is a concrete class.
  struct C
  {
    // `B::foo' is redefined.
    virtual int foo() { return 4; }
  };

} // end of namespace dynamic_hierarchy


/*---------------------------.
| A small static hierarchy.  |
`---------------------------*/

/* Let's start building a small static hierarchy similar to the above
   dynamic hierarchy.  */

namespace simple_static_hierarchy
{
  /* In traditional OO design, a object has two types :

     - a static type, known at compile time;
     - a dynamic type or exact type, known at run time.

     The dynamic type is more precise (or equally precise) than the
     static type: it's a subclass of the static type.

     The inclusion polymorphism allows the programmer to manipulate an
     object while knowing only it's compile time type: the type of the
     object only needs to satisfy an interface (through inheritance)
     required by the algorithm.


     In the Static C++ Object-Oriented Paradigm (SCOOP), an object
     still has two types, but they are both static:

     - the current type, which can be be an abstraction type;
     - the exact type.

     As everything is static, SCOOP must rely on the static type
     system of C++, i.e, the current type must hold the information of
     the exact type. This is achieved through the use of templates: a
     current type embeds the exact type as a template parameter.

     Consequently, in SCOOP all derivable classes of a hierarchy are
     turned into templates and have an `Exact' parameter, which is the
     exact static type of the object, à la ``Curiously Recurring
     Template Pattern'' (CRTP) [1].

     Let's have a look at small example of static inheritance, with
     only two classes, `A' and `B'.  */

  template <typename Exact>
  struct A
  {
  };

  struct B : public A<B>
  {
  };

  /* In the above code, `A' is turned into a template class and has an
     `Exact' parameter, and `B' doesn't derive from just `A', but from
     `A<B>'.  This trick allows us to write:

       A<B>* a = new B;

     As you can see, the current type still holds the exact type!
     Later, we'll see how to actually get back the exact type.  */

} // end of namespace simple_static_hierarchy


// ---------- //
// stc::any.  //
// ---------- //

namespace static_hierarchy_with_any
{
  /* To ease the retrieval of the exact type, we'll make use of
     `stc::any'.  This is an instrumentation of the hierarchy to ease
     the retrieval of the exact type and the conversion of the object
     to this type.  In fact, there is not a single `stc::any' class,
     but several, which have their advantages and drawbacks
     w.r.t. speed, memory print and support for diamond inheritance.
     We'll only deal with `stc::any' (an alias for `stc::any__simple')
     in this Static tour.

     `stc::any' is used as the base class of any top class of a static
     hierarchy.  It is passed the `Exact' type, just like any non-leaf
     class of the hierarchy (a leaf class is a class which is not
     derived, i.e., at the bottom of an inheritance tree).  */

  template <typename Exact>
  struct A : public stc::any<Exact>
  {
  };

  /* Now, let's turn `B' be into a derivable class: we only have to
     make it a template class, with an `Exact' parameter, and let it
     pass this exact type to `A'.  */

  template <typename Exact>
  struct B : public A<Exact>
  {
  };

  /* We now introduce a concrete leaf class, `C', which derives from
     `B', and passes its own type as exact type.  */

  struct C : public B<C>
  {
  };

} // end of namespace static_hierarchy_with_any


// -------------------------------- //
// Getting back to the exact type.  //
// -------------------------------- //

namespace static_hierarchy_with_any
{
  void test()
  {
    // Metalic assertions.
    mlc::assert_< mlc_is_a_(C, A) >::check();
    mlc::assert_< mlc_is_a_(C, B) >::check();


    // Getting the exact type.

    // `stc::any' has an `exact_type' typedef.
    typedef A<C>::exact_type exact_type_of_A_1;
    // But you can also use stc_to_exact_ to get this type.
    typedef stc_to_exact_(A<C>) exact_type_of_A_2;
    // Check these types.
    mlc::assert_< mlc_eq(exact_type_of_A_1, C) >::check();
    mlc::assert_< mlc_eq(exact_type_of_A_2, C) >::check();


    // Converting an object to its exact type.

    // Create a C.
    C c;
    // See it through an abstraction.
    A<C>& a = c;
    // And get it back, with no runtime overhead (i.e., dynamic_cast).
    C& c2 = a.exact();
    // Ensure we got back the same object.
    assert (&c2 == &c);

    // stc::exact provides the same service.
    C& c3 = stc::exact(a);
    assert (&c3 == &c);
  }

} // end of namespace static_hierarchy_with_any


// --------------------------- //
// Non-leaf concrete classes.  //
// --------------------------- //

/* All classes carrying an exact type are implicitly abstract classes.
   Thus, with our current modeling, non-leaf classes are
   compulsorily abstract.  This is in compliance with good OO design
   practices: in an class hierarchy, non-leaf classes should be
   abstract.  But what if we wanted (for a reason or another) B to be
   a concrete (i.e., instantiable) class?  The form `B<>' is not
   allowed: a value must be provided for the `Exact' parameter.  This is
   the role of the stc::itself tag.

   By convention, a class whose exact type is `B<stc::itself>' is
   considered concrete.  To shorten the coding style, non-leaf
   concrete classes take `stc::itself' as default value for their
   `Exact' parameter (see below).  This way, `B<>' becomes a correct
   type.

   The only remaining difficulty is to pass the base class the right
   exact type.  A small metacode is required here, hidden behind the
   `stc_find_exact' helper macro.  The code below explains the
   necessary changes.  */

namespace static_hierarchy_with_a_non_leaf_concrete_class
{

  /* A is left unchanged (it is still an abstract class).  */

  template <typename Exact>
  struct A : public stc::any<Exact>
  {
  };

  /* B is turned into a concrete class. To achieve this, we apply two
     changes:

     1. the parameter passed to A (B's base class) is no longer
        `Exact', but `stc_find_exact(B, Exact)'.  The `stc_find_exact'
        macro evaluates to `B<>' (i.e., `B<stc::itself>') or to
        `Exact', depending on whether `Exact' is equal to
        `stc::itself' or not;

     2. `Exact' takes a default value, `stc::itself', so that one can
        abbreviate `B<stc::itself>' to `B<>' (this change is a pure
        matter of style and is optional).  */

  template <typename Exact = stc::itself>
  struct B : public A< stc_find_exact(B, Exact) >
  {
  };

  /* C is left unchanged.  */

  struct C : public B<C>
  {
  };

  /* If we loot at the classes above C and B<> in the inheritance
     tree, we have:

                        A<C>        A< B<> >
                         ^             ^
                         |             |
                        B<C>          B<>
                         ^
                         |
                         C

     The exact type is correctly passed to super classes.  */

} // end of namespace static_hierarchy_with_a_non_leaf_concrete_class

/* To put it in a nutshell:

   - non-leaf classes are abstract by default in SCOOP;
   - to turn a non-leaf class `Foo' into a concrete class, you have to
     - pass it `stc_find_exact(Foo, Exact)' instead of `Exact' as
       exact type parameter to its super class;
     - optionally (but recommended), give its `Exact' parameter a
       default value, `stc::itself';
   - to use such a class as a concrete one, simply use it with an empty
     `Exact' parameter: `Foo<>'.  */


// --------- //
// Methods.  //
// --------- //

/* We have laid down the foundations of our classes.  Now it's time to
   add some methods.

   Unlike classic C++, SCOOP doesn't use the `virtual' keyword to
   define polymorphic methods.  As SCOOP performs a static resolution
   of method calls, it can't rely on `virtual' (which is dynamic per
   se).  Instead, a manual dispatch of method call is used: each
   polymorphic method is turned into a facade, and there is one
   implementation per class defining this method actually.  The facade
   retrieves the exact type of the object, and calls the
   implementation corresponding to this type.  A SCOOP abstract
   polymorphic method (the equivalent of C++ ``virtual pure'' method)
   has no implementation in the class where its facade is defined.

   The convention is to name `impl_m' the implementation(s) of the
   facade `m'.  */

namespace static_hierarchy_with_methods
{

  template <typename Exact>
  struct A : public stc::any<Exact>
  {
    /* Facade of a statically-dispatched method.  Notice there is no
       `virtual' keyword: the (static) dispatch is done manually
       through the delegation to impl_foo.  */
    int foo()      { return this->exact().impl_foo(); }
    // Implementation of the method.
    int impl_foo() { return 1; }

    // A ``virtual'' pure (i.e., abstract) method.
    int bar()      { return this->exact().impl_bar(); }
    // (No `impl_bar', since bar is abstract.)
  };

  template <typename Exact>
  struct B : public A< stc_find_exact(B, Exact) >
  {
    // A redefined method.
    int impl_foo() { return 2; }
    // B::bar (implementation) is defined.
    int impl_bar() { return 3; }
  };

  struct C : public B<C>
  {
    // Redefinition.
    int impl_foo() { return 4; }
  };

} // end of namespace static_hierarchy_with_methods


namespace static_hierarchy_with_methods
{

  void test1()
  {
    C c;
    assert (c.foo() == 4);
    assert (c.bar() == 3);

    B<C>& b = c;
    assert (b.foo() == 4);
    assert (b.bar() == 3);

    A<C>& a = c;
    assert (a.foo() == 4);
    assert (a.bar() == 3);
  }

} // end of namespace static_hierarchy_with_methods


// --------------------------- //
// Enforcing method checking.  //
// --------------------------- //

/* For a given `Exact' type, consider the `A<Exact>::foo' method.
   This method relies on the fact that `Exact' has an `impl_foo'
   method.  However, the compiler let you instantiate an a class
   `A<Exact>' with an `Exact' type not fulfilling this requirement,
   and even worse, you can instantiate an object of this ill-formed
   type!  The compiler will complain only when it tries to resolve the
   call to the (missing) `impl_foo' method.  */

namespace static_hierarchy_with_methods
{

  // For instance, let's introduce an ill-formed concrete subclass of A.
  struct D : public A<D>
  {
    // No `impl_foo'.
  };

  /* `D' is not a valid exact type for A.  Despite this fact, we can
     instantiate this template with `D', as well as an object of this
     type!  */
  void test2()
  {
    // Valid (!).
    typedef A<D> A_D;
    // Equally valid (!).
    A_D a;
    // This call would fail if it were not disabled.
#if 0
    a.foo();
#endif
  }

} // end of namespace static_hierarchy_with_methods


/* Metalic proposes a concept checking tool similar to the one used in
   the Boost Concept Check Library (BCCL) [2],
   `mlc_check_method_impl'.  We use it to check that SCOOP abstract
   polymorphic methods defined in an abstract class have a valid
   implementation in one of the subclasses (i.e., "between" this
   abstraction and the exact type).  */

namespace static_hierarchy_with_method_checks
{

  template <typename Exact>
  struct A : public stc::any<Exact>
  {
    int foo()      { return this->exact().impl_foo(); }
    int impl_foo() { return 1; }
    int bar()      { return this->exact().impl_bar(); }

    /* Method checks are placed in the destructor, since they must
       reside within a function body.  */
    ~A()
    {
      /* `foo' is abstract in `A'.  Ensure A's subclasses define an
	 implementation for this method.  */
      mlc_check_method_impl(Exact, int, bar, /* no arg */, /* no const */);
    }
  };

  template <typename Exact>
  struct B : public A< stc_find_exact(B, Exact) >
  {
    int impl_foo() { return 2; }
    int impl_bar() { return 3; }
  };

  struct C : public B<C>
  {
    int impl_foo() { return 4; }
  };

  struct D : public A<D>
  {
    // No `impl_foo'.
  };


  void test()
  {
    /* Invalid template instantiation, because of `impl_foo' missing.
       Upon this code, the compiler would emit an error message
       similar to this one:

         error: 'impl_bar' is not a member of
	 'static_hierarchy_with_method_checks::D'

    */
#if 0
    typedef A<D> A_D;
    A_D a;
#endif
  }

} // end of namespace static_hierarchy_with_method_checks



/*-------.
| Misc.  |
`-------*/

// Run dynamic tests.
int main()
{
  static_hierarchy_with_any::test();
  static_hierarchy_with_methods::test1();
}



/*-------------.
| References.  |
`-------------*/

/* [1] James O. Coplien.  ``Curiously Recurring Template
   Patterns''. C++ Report: 24-27. 1995, February.

   [2] Jeremy Siek, Andrew Lumsdaine.  ``Concept Checking: Binding
   Parametric Polymorphism in C++''.  In Proceedings of the ``Workshop
   on C++ Template Programming''.  2000.  */



/// Local Variables:
/// ispell-local-dictionary: "american"
/// End:

//  LocalWords:  inline stc namespace vtypes OO struct CRTP typename metacode
//  LocalWords:  instantiable impl ispell american
