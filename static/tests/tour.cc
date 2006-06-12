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

# include <stc/any.hh>

// FIXME: ``Fil rouge'': building a small (but complete) static
// hierarchy, using all features from Static. Possibly, this
// construction could be progressive, i.e., made of several attempts,
// each enclosed in its own namespace.
//
// Be as simple as possible, and introduce features one by one. For
// instance, don't introduce the multiple inheritance system after
// having introduced the vtypes.

// FIXME: Things to show:
// - any
// - exact
// - find_exact
// - super
// - virtual types (and macros for super)
// - multiple super
// - entry


// Welcome to this tour of the Static library!

// FIXME: Intro.


/*----------------------------.
| A small dynamic hierarchy.  |
`----------------------------*/

// Normally, we'd be writing something like this:

namespace dynamic_hierarchy
{

  // A is abstract.
  struct A
  {
    // A virtual method.
    virtual void foo()
    {
      // ...
    }
    // A virtual pure method.
    virtual void bar() = 0;
  };

  // B is abstract.
  struct B
  {
    // A redefined method.
    virtual void foo()
    {
      // ...
    }
    // B::bar is defined.
    virtual void bar()    
    {
      // ...
    }
  };

  // C is concrete
  struct C
  {
    // A redefined method.
    virtual void foo()
    {
      // ...
    }
  };

} // end of namespace dynamic_hierarchy


/*---------------------------.
| A small static hierarchy.  |
`---------------------------*/

// Let's start building a small static hierarchy. Converting the first
// dynamic hierarchy results in:

// FIXME: Comments.

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
     turned into templates and have an ``Exact'' parameter, which is
     the exact static type of the object, à la Curiously Recurring
     Template Pattern (CRTP) [1].

     Let's have a look at small example of static inheritance, with
     only two classes, A and B.  */

  template <typename Exact>
  struct A
  {
  };

  struct B : public A<B>
  {
  };

  /* For instance, in the code above, A is turned into a template
     class and has an Exact parameter, and B doesn't derive from just
     A, but from A<B>.  This trick allows us to write:

       A<B>* a = new B;

     As you can see, the current type still holds the exact type!  In
     the next section, we'll see how to actually get back the exact
     type.  */

} // end of namespace simple_static_hierarchy

/*-----------.
| stc::any.  |
`-----------*/

namespace static_hierarchy_with_any
{
  /* To ease the retrieval of the exact type, we'll make use of
     stc::any.  This is an instrumentation of the hierarchy to ease
     the retrieval of the exact type and the conversion of the object
     to this type.  In fact, there is not a single stc::any class, but
     several, which have their advantages and drawbacks w.r.t. speed,
     memory print and support for diamond inheritance.  We'll only deal
     with stc::any__simple in this Static tour.

     stc::any is used as the base class of any top class of a static
     hierarchy: it is passed the Exact type, just like any non-leaf
     class of the hierarchy.  */

  template <typename Exact>
  struct A : public stc::any__simple<Exact>
  {
  };

  /* Now, let's turn be into a derivable class: we only have to make
     it a template class, with an Exact parameter, and let it pass
     this exact type to A.  */

  template <typename Exact>
  struct B : public A<Exact>
  {
  };

  /* We now introduce a concrete leaf class, C, which derives from B,
     and passes its own type as exact type.  */

  struct C : public B<C>
  {
  };

} // end of namespace static_hierarchy

// FIXME: stc/exact.hh.

// FIXME: stc/find_exact.hh and the non-leaf concrete classes.
//
// « Implicitly, our non-leaf classes are abstract. (...) »


// --------- //
// Methods.  //
// --------- //

// Add the methods.

// FIXME: Comments.

namespace static_hierarchy_with_methods
{

  template <typename Exact>
  struct A : public stc::any__simple<Exact>
  {
    // A static ``virtual'' method.  Notice there is no virtual
    // keyword: the (static) dispatch is done manually through the
    // delegation to impl_foo.
    void foo()
    {
      this->exact().impl_foo();
    }
    void impl_foo()
    {
      // Empty.
    }

    // A virtual pure method.
    virtual void bar()
    {
      this->exact().impl_bar();
    }
    // No impl_bar, since bar is virtual pure;
  };

  template <typename Exact>
  struct B : public A<Exact>
  {
    // Redefinition.
    void impl_foo()
    {
      // Empty.
    }
    // Definition.
    void impl_bar()
    {
      // Implementation goes here.
    }
  };

  struct C : public B<C>
  {
    // Redefinition.
    void impl_foo()
    {
      // Empty.
    }
  };

} // end of namespace static_hierarchy_with_methods


// FIXME: Introduce static checks of methods (kind of concept checking).

int main() {}

/* References:

   [1] Coplien, James O. (1995, February). "Curiously Recurring
   Template Patterns". C++ Report: 24-27.  */
