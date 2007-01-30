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


/// This test focuses on the virtual types system, so the exact type of
/// classes is not propagated here (stc::any is not used).

/// These examples comes from doc/algorithm.ml.


#include <mlc/cmp.hh>
#include <mlc/assert.hh>

#include <stc/scoop-alt.hh>

// Namespace equipment.
stc_scoop_equipment_for_namespace(my);


// FIXME: Check single_vtype, too.

namespace my
{
  // ------------------------------------------------------------ Typedefs.

  mlc_decl_typedef(my_type);

  mlc_decl_typedef(foo_type);
  mlc_decl_typedef(bar_type);
  mlc_decl_typedef(baz_type);
  mlc_decl_typedef(quux_type);
  mlc_decl_typedef(hop_type);

  mlc_decl_typedef(value_type);



  /* ---------------------------------------------------------------- Ex 1.

     ** Virtual type definition

       Ex.

	 class A < stc::none =
	 {
	   vtype my_type = int;
	 }
  */

  namespace ex1
  {
    // Forward declaration.
    struct A;
  }

  // Set super type.
  template <> struct set_super_type<ex1::A> { typedef stc::none ret; };

  /// Virtual types associated to A.
  template <>
  struct vtypes<ex1::A>
  {
    typedef int my_type;
  };

  namespace ex1
  {
    struct A : public stc::none {};

    void
    check()
    {
      mlc::assert_<
	mlc_eq(stc_find_vtype_(my, my::ex1::A, my), int)
	>::check();
    }
  }
  
  /* ---------------------------------------------------------------- Ex 2.

     ** Virtual type re-definition.

       Ex.

	 class A < stc::none =
	 {
	   vtype my_type = int;
	 }
	 // Valid.
	 class B < A =
	 {
	   vtype my_type = float;
	 }
	 // Valid.
	 class C < A =
	 {
	   vtype my_type = int;
	 }
  */

  namespace ex2
  {
    // Forward declarations.
    struct A;
    struct B;
    struct C;
  }

  // Set super types.
  template <> struct set_super_type<ex2::A> { typedef stc::none ret; };
  template <> struct set_super_type<ex2::B> { typedef ex2::A    ret; };
  template <> struct set_super_type<ex2::C> { typedef ex2::B    ret; };

  /// Virtual types associated to A.
  template <>
  struct vtypes<ex2::A>
  {
    typedef int my_type;
  };

  /// Virtual types associated to B.
  template <>
  struct vtypes<ex2::B>
  {
    typedef float my_type;
  };

  /// Virtual types associated to C.
  template <>
  struct vtypes<ex2::C>
  {
    typedef int my_type;
  };

  namespace ex2
  {
    struct A : public stc::none {};
    struct B : public A {};
    struct C : public B {};

    void
    check()
    {
      mlc::assert_<
	mlc_eq(stc_find_vtype_(my, my::ex2::A, my), int)
	>::check();

      mlc::assert_<
	mlc_eq(stc_find_vtype_(my, my::ex2::B, my), float)
	>::check();

      mlc::assert_<
	mlc_eq(stc_find_vtype_(my, my::ex2::C, my), int)
	>::check();
    }
  }


  /* ---------------------------------------------------------------- Ex 3.

     ** Virtual type abstract declaration (i.e., deferred definition).
 
       Ex.:

	 class A < stc::none =
	 {
	   // my_type is abstract.
	   vtype my_type = 0;
	 }
	 class B < A =
	 {
	   vtype my_type = int;
	 }
  */

  namespace ex3
  {
    // Forward declarations.
    struct A;
    struct B;
  }

  // Set super types.
  template <> struct set_super_type<ex3::A> { typedef stc::none ret; };
  template <> struct set_super_type<ex3::B> { typedef ex3::A    ret; };

  /// Virtual types associated to A.
  template <>
  struct vtypes<ex3::A>
  {
    typedef stc::abstract my_type;
  };

  /// Virtual types associated to B.
  template <>
  struct vtypes<ex3::B>
  {
    typedef int my_type;
  };

  namespace ex3
  {
    struct A : public stc::none {};
    struct B : public A {};

    void
    check()
    {
      mlc::assert_<
	mlc_eq(stc_find_vtype_(my, my::ex3::B, my), int)
	>::check();
    }
  }


  /* ---------------------------------------------------------------- Ex 4.

     ** Final virtual type

     - A virtual type can be tagged as final.

       Ex.:

	 class A < stc::none =
	 {
	   final vtype my_type = int;
	 }
  */

  namespace ex4
  {
    // Forward declaration.
    struct A;
  }

  // Set super type.
  template <> struct set_super_type<ex4::A> { typedef stc::none ret; };

  /// Virtual types associated to A.
  template <>
  struct vtypes<ex4::A>
  {
    typedef stc::final<int> my_type;
  };

  namespace ex4
  {
    struct A : public stc::none {};

    void
    check()
    {
      mlc::assert_<
	mlc_eq(stc_find_vtype_(my, my::ex4::A, my), int)
	>::check();
    }
  }


  /* ---------------------------------------------------------------- Ex 5.

     ** General virtual type lookup (i.e., with delegation)

     *** Abstract

       class A < stc::none
       {
	 // my_vtype not defined.
       }

       class D < stc::none
       {
	 vtype my_type = 0;
       }

       // A
       // ^
       // |
       // C<>--D
       class C < stc::none
       {
	 vtype delegatee_type = D;
       }

       type t = C#my_type;
  */

  namespace ex5
  {
    // Forward declarations.
    struct A;
    struct D;
    struct C;
  }

  // Set super types.
  template <> struct set_super_type<ex5::A> { typedef stc::none ret; };
  template <> struct set_super_type<ex5::D> { typedef stc::none ret; };
  template <> struct set_super_type<ex5::C> { typedef ex5::A    ret; };

  /// Virtual types associated to A.
  template <>
  struct vtypes<ex5::A>
  {
  };

  /// Virtual types associated to D.
  template <>
  struct vtypes<ex5::D>
  {
    typedef stc::abstract my_type;
  };

  /// Virtual types associated to C.
  template <>
  struct vtypes<ex5::C>
  {
    typedef ex5::D delegatee_type;
  };

  namespace ex5
  {
    struct A : public stc::none {};
    struct D : public stc::none {};
    struct C : public A {};

    void
    check()
    {
      mlc::assert_<
	mlc_eq(stc_find_vtype_(my, my::ex5::C, my), stc::not_found)
	>::check();
    }
  }


  /* ---------------------------------------------------------------- Ex 6.

     * More examples.

     ** A complex example, with stc::not_delegated.

                                   A
                                   ^
                                   |    X
                                   B    ^
                                   ^    |
                                   |    Y
                                   C<>-´
                                   ^
                                   |
                                   D
                                   ^
                                   |
                                   E


       class A < stc::none         |     class C < B
       {                           |     {
       }                           |       vtype delegatee_type = Y;
       class B < A                 |       vtype foo = int;
       {                           |       vtype baz = not_delegated;
         vtype foo = 0;            |       vtype hop = not_delegated;
       }                           |     }
                                   |     class D < C
       class X < stc::none         |     {
       {                           |       vtype quux = unsigned;
         vtype bar = 0;            |     }
         vtype hop = int;          |     class E < D
       }                           |     {
       class Y < X                 |       vtype baz = float;     
       {                           |     }
         vtype baz = short;        |
         vtype bar = char;         |
       }                           |
  */

  namespace ex6
  {
    // Forward declarations.
    struct A;
    struct B;

    struct X;
    struct Y;

    struct C;
    struct D;
    struct E;
  }

  // Set super types.
  template <> struct set_super_type<ex6::A> { typedef stc::none ret; };
  template <> struct set_super_type<ex6::B> { typedef ex6::A    ret; };

  template <> struct set_super_type<ex6::X> { typedef stc::none ret; };
  template <> struct set_super_type<ex6::Y> { typedef ex6::X    ret; };

  template <> struct set_super_type<ex6::C> { typedef ex6::B    ret; };
  template <> struct set_super_type<ex6::D> { typedef ex6::C    ret; };
  template <> struct set_super_type<ex6::E> { typedef ex6::D    ret; };

  /// Virtual types associated to A.
  template <>
  struct vtypes<ex6::A>
  {
  };

  /// Virtual types associated to B.
  template <>
  struct vtypes<ex6::B>
  {
    typedef stc::abstract foo_type;
  };

  /// Virtual types associated to X.
  template <>
  struct vtypes<ex6::X>
  {
    typedef stc::abstract bar_type;
    typedef int           hop_type;
  };

  /// Virtual types associated to Y.
  template <>
  struct vtypes<ex6::Y>
  {
    typedef short baz_type;
    typedef char  bar_type;
  };

  /// Virtual types associated to C.
  template <>
  struct vtypes<ex6::C>
  {
    typedef ex6::Y             delegatee_type;
    typedef int                foo_type;
    typedef stc::not_delegated baz_type;
    typedef stc::not_delegated hop_type;
  };

  /// Virtual types associated to D.
  template <>
  struct vtypes<ex6::D>
  {
    typedef unsigned quux_type;
  };

  /// Virtual types associated to E.
  template <>
  struct vtypes<ex6::E>
  {
    typedef float baz_type;
  };

  namespace ex6
  {
    struct A : public stc::none {};
    struct B : public A {};

    struct X : public stc::none {};
    struct Y : public X {};

    struct C : public B {};
    struct D : public C {};
    struct E : public D {};

    void
    check()
    {
      mlc::assert_<
	mlc_eq(stc_find_vtype_(my, my::ex6::E, foo), int)
	>::check();

      mlc::assert_<
	mlc_eq(stc_find_vtype_(my, my::ex6::E, bar), char)
	>::check();

      mlc::assert_<
	mlc_eq(stc_find_vtype_(my, my::ex6::E, baz), float)
	>::check();

      mlc::assert_<
	mlc_eq(stc_find_vtype_(my, my::ex6::E, quux), unsigned)
	>::check();

      mlc::assert_<
	mlc_eq(stc_find_vtype_(my, my::ex6::E, hop), stc::not_found)
	>::check();
    }
  }


  /* ---------------------------------------------------------------- Ex 7.

  *** Olena-like examples.

     class /image_entry/ < stc::none
     {
       vtype value_type = 0;
     }

     class image2d<int> < image_entry
     {
       vtypes value_type = int;
     }

     class /value_morpher/ < image_entry
     {
       vtype delegatee_type = 0;
       vtype value_type = stc::not_delegated;
     } 

     class value_cast<image2d<int>, float> < value_morpher
     {
       vtype delegatee_type = image2d<int>;
       vtype value_type = float;
     }
  */

  namespace ex7
  {
    // Forward declarations.
    namespace abstract
    {
      struct image_entry;
      struct value_morpher;
    }

    template <typename T> struct image2d;
    template <typename I, typename T> struct value_cast;
  }

  // Set super types.
  template <>
  struct set_super_type<ex7::abstract::image_entry>
  {
    typedef stc::none ret;
  };
  template <>
  struct set_super_type<ex7::abstract::value_morpher>
  {
    typedef ex7::abstract::image_entry ret;
  };
  template <typename T>
  struct set_super_type< ex7::image2d<T> >
  {
    typedef ex7::abstract::image_entry ret;
  };
  template <typename I, typename T>
  struct set_super_type< ex7::value_cast<I, T> >
  {
    typedef ex7::abstract::value_morpher ret;
  };


  /// Virtual types associated to abstract::image_entry.
  template <>
  struct vtypes<ex7::abstract::image_entry>
  {
    typedef stc::abstract value_type;
  };

  /// Virtual types associated to abstract::value_morpher.
  template <>
  struct vtypes<ex7::abstract::value_morpher>
  {
    typedef stc::abstract      delegatee_type;
    typedef stc::not_delegated value_type;
  };

  /// Virtual types associated to image2d<T>.
  template <typename T>
  struct vtypes< ex7::image2d<T> >
  {
    typedef T value_type;
  };

  /// Virtual types associated to value_cast<I, T>.
  template <typename I, typename T>
  struct vtypes< ex7::value_cast<I, T> >
  {
    typedef I delegatee_type;
    typedef T value_type;
  };


  namespace ex7
  {
    namespace abstract
    {
      struct image_entry : public stc::none {};
      struct value_morpher : public image_entry {};
    }

    template <typename T>
    struct image2d : public abstract::image_entry {};

    template <typename I, typename T>
    struct value_cast : public abstract::value_morpher {};


    void
    check()
    {
      mlc::assert_<
	mlc_eq(stc_find_vtype_(my, my::ex7::image2d<int>, value), int)
	>::check();

      typedef value_cast< image2d<int>, float> morphed_type;
      mlc::assert_<
	mlc_eq(stc_find_vtype_(my, morphed_type, value), float)
	>::check();
    }
  }

} // end of namespace my


int
main()
{
  my::ex1::check();
  my::ex2::check();
  my::ex3::check();
  my::ex4::check();
  my::ex5::check();
  my::ex6::check();
  my::ex7::check();
}
