// Copyright (C) 2006, 2007 EPITA Research and Development Laboratory
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

#include <cassert>

#include <mlc/assert.hh>
#include <mlc/cmp.hh>

#include <stc/scoop-alt.hh>

#include <mlc/int.hh>


// FIXME: Check single_vtype, too.

namespace my
{

  // ------------------------------------------------- Namespace equipment.

# include <stc/scoop-alt.inc>


  // ------------------------------------------------------------ Typedefs.

  mlc_decl_typedef(my_type);

  mlc_decl_typedef(foo);
  mlc_decl_typedef(bar);
  mlc_decl_typedef(baz);
  mlc_decl_typedef(quux);
  mlc_decl_typedef(hop);

  mlc_decl_typedef(value);

  mlc_decl_typedef(dim);
  mlc_decl_typedef(mydim);

  mlc_decl_typedef(grid);



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
  template <> struct super_trait_<ex1::A> { typedef stc::none ret; };

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
	mlc_eq(stc_find_type_(my::ex1::A, my_type), int)
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
  template <> struct super_trait_<ex2::A> { typedef stc::none ret; };
  template <> struct super_trait_<ex2::B> { typedef ex2::A    ret; };
  template <> struct super_trait_<ex2::C> { typedef ex2::B    ret; };

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
	mlc_eq(stc_find_type_(my::ex2::A, my_type), int)
	>::check();

      mlc::assert_<
	mlc_eq(stc_find_type_(my::ex2::B, my_type), float)
	>::check();

      mlc::assert_<
	mlc_eq(stc_find_type_(my::ex2::C, my_type), int)
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
  template <> struct super_trait_<ex3::A> { typedef stc::none ret; };
  template <> struct super_trait_<ex3::B> { typedef ex3::A    ret; };

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
	mlc_eq(stc_find_type_(my::ex3::B, my_type), int)
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
  template <> struct super_trait_<ex4::A> { typedef stc::none ret; };

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
	mlc_eq(stc_find_type_(my::ex4::A, my_type), int)
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
	 vtype delegatee = D;
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
  template <> struct super_trait_<ex5::A> { typedef stc::none ret; };
  template <> struct super_trait_<ex5::D> { typedef stc::none ret; };
  template <> struct super_trait_<ex5::C> { typedef ex5::A    ret; };

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
    typedef ex5::D delegatee;
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
	mlc_eq(stc_find_type_(my::ex5::C, my_type), stc::not_found)
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
       }                           |       vtype delegatee = Y;
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
  template <> struct super_trait_<ex6::A> { typedef stc::none ret; };
  template <> struct super_trait_<ex6::B> { typedef ex6::A    ret; };

  template <> struct super_trait_<ex6::X> { typedef stc::none ret; };
  template <> struct super_trait_<ex6::Y> { typedef ex6::X    ret; };

  template <> struct super_trait_<ex6::C> { typedef ex6::B    ret; };
  template <> struct super_trait_<ex6::D> { typedef ex6::C    ret; };
  template <> struct super_trait_<ex6::E> { typedef ex6::D    ret; };

  /// Virtual types associated to A.
  template <>
  struct vtypes<ex6::A>
  {
  };

  /// Virtual types associated to B.
  template <>
  struct vtypes<ex6::B>
  {
    typedef stc::abstract foo;
  };

  /// Virtual types associated to X.
  template <>
  struct vtypes<ex6::X>
  {
    typedef stc::abstract bar;
    typedef int           hop;
  };

  /// Virtual types associated to Y.
  template <>
  struct vtypes<ex6::Y>
  {
    typedef short baz;
    typedef char  bar;
  };

  /// Virtual types associated to C.
  template <>
  struct vtypes<ex6::C>
  {
    typedef ex6::Y             delegatee;
    typedef int                foo;
    typedef stc::not_delegated baz;
    typedef stc::not_delegated hop;
  };

  /// Virtual types associated to D.
  template <>
  struct vtypes<ex6::D>
  {
    typedef unsigned quux;
  };

  /// Virtual types associated to E.
  template <>
  struct vtypes<ex6::E>
  {
    typedef float baz;
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
	mlc_eq(stc_find_type_(my::ex6::E, foo), int)
	>::check();

      mlc::assert_<
	mlc_eq(stc_find_type_(my::ex6::E, bar), char)
	>::check();

      mlc::assert_<
	mlc_eq(stc_find_type_(my::ex6::E, baz), float)
	>::check();

      mlc::assert_<
	mlc_eq(stc_find_type_(my::ex6::E, quux), unsigned)
	>::check();

      mlc::assert_<
	mlc_eq(stc_find_type_(my::ex6::E, hop), stc::not_found)
	>::check();
    }
  }


  /* ---------------------------------------------------------------- Ex 7.

  *** Olena-like examples.

     class /image_entry/ < stc::none
     {
       vtype value = 0;
     }

     class image2d<int> < image_entry
     {
       vtypes value = int;
     }

     class /value_morpher/ < image_entry
     {
       vtype delegatee = 0;
       vtype value = stc::not_delegated;
     } 

     class value_cast<image2d<int>, float> < value_morpher
     {
       vtype delegatee = image2d<int>;
       vtype value = float;
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
  struct super_trait_<ex7::abstract::image_entry>
  {
    typedef stc::none ret;
  };
  template <>
  struct super_trait_<ex7::abstract::value_morpher>
  {
    typedef ex7::abstract::image_entry ret;
  };
  template <typename T>
  struct super_trait_< ex7::image2d<T> >
  {
    typedef ex7::abstract::image_entry ret;
  };
  template <typename I, typename T>
  struct super_trait_< ex7::value_cast<I, T> >
  {
    typedef ex7::abstract::value_morpher ret;
  };


  /// Virtual types associated to abstract::image_entry.
  template <>
  struct vtypes<ex7::abstract::image_entry>
  {
    typedef stc::abstract value;
  };

  /// Virtual types associated to abstract::value_morpher.
  template <>
  struct vtypes<ex7::abstract::value_morpher>
  {
    typedef stc::abstract      delegatee;
    typedef stc::not_delegated value;
  };

  /// Virtual types associated to image2d<T>.
  template <typename T>
  struct vtypes< ex7::image2d<T> >
  {
    typedef T value;
  };

  /// Virtual types associated to value_cast<I, T>.
  template <typename I, typename T>
  struct vtypes< ex7::value_cast<I, T> >
  {
    typedef I delegatee;
    typedef T value;
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
	mlc_eq(stc_find_type_(my::ex7::image2d<int>, value), int)
	>::check();

      typedef value_cast< image2d<int>, float> morphed_type;
      mlc::assert_<
	mlc_eq(stc_find_type_(morphed_type, value), float)
	>::check();
    }
  }


  /* ---------------------------------------------------------------- Ex 8.

     Another Olena-like example with recursive definitions.

       class internal_dpoint_nd < stc::none
       {
         vtype mydim = find dpoint2d "dim";
       }

       class dpoint2d < internal_dpoint_nd
       {
         // Dummy value (because our C++ types are limited in this prototype).
         vtype dim = int
       }

     This example is translated to C++ using the Exact-type-passing
     technique in order to perform a test closer to what can be found
     in Olena.  */

  namespace ex8
  {
    // Forward declarations.
    template <typename Exact> struct internal_dpoint_nd;
    struct dpoint2d;
  }

  // Set super types.
  template <typename Exact>
  struct super_trait_< ex8::internal_dpoint_nd<Exact> >
  {
    typedef stc::none ret;
  };
  template <>
  struct super_trait_<ex8::dpoint2d>
  {
    typedef ex8::internal_dpoint_nd< ex8::dpoint2d > ret;
  };

  /// Virtual types associated to internal_dpoint_nd<Exact>.
  template <typename Exact>
  struct vtypes< ex8::internal_dpoint_nd <Exact> >
  {
    typedef stc_find_type(Exact, dim) mydim;
  };
  /// Virtual types associated to dpoint2d.
  template <>
  struct vtypes< ex8::dpoint2d >
  {
    typedef int dim;
  };

  namespace ex8
  {
    template <typename Exact>
    struct internal_dpoint_nd : public stc::none
    {
    };
    struct dpoint2d : public internal_dpoint_nd< dpoint2d >
    {
    };

    void
    check()
    {
      mlc::assert_<
	mlc_eq(stc_find_type_(my::ex8::dpoint2d, mydim), int)
	>::check();
    }

  } // end of namespace my::ex8


  /* ---------------------------------------------------------------- Ex 9.

     Another Olena-inspired example.

     ...

     There are other virtual types in the Olena version of the
     following classes, but we don't need them for this simple test.  */

  // FIXME: I (Roland) don't know why this test succeeds, while the
  // equivalent SCOOP 2 hierarchy doesn't work in Olena.


  /*--------.
  | Point.  |
  `--------*/

  // Forward declaration.
  namespace ex9
  {
    template <typename Exact>
    struct Point
    {
      stc_typename(grid);
      stc_typename(dim);

      enum { n = mlc_value(dim) };

    }; // end of oln::Point<Exact>

  }



  /*------------------------.
  | internal::point_base_.  |
  `------------------------*/

  // Forward declaration.
  namespace ex9
  {
    namespace internal
    {
      template <typename Exact> struct point_base_;
    }
  }

  /// Set super type.
  template <typename Exact>
  struct super_trait_< ex9::internal::point_base_<Exact> >
  {
    // super_trait_ set to stc::none, since Point is a concept
    // (interface).
    typedef stc::none ret;
  };

  /// Virtual types associated to internal::point_base_<Exact>
  template <typename Exact>
  struct vtypes< ex9::internal::point_base_<Exact> >
  {
    typedef stc::abstract grid;

    typedef stc_deferred(grid) grid__;
    typedef stc_deferred_from(grid__, dim) dim__;
    typedef stc::final< dim__ > dim;
  };

  // Actual definition.
  namespace ex9
  {
    namespace internal
    {
      template <typename Exact>
      struct point_base_ : public stc::none
      {
	typedef Point<Exact> super;

	stc_using(dim);
  	static const unsigned dim_val = mlc_value(dim);
      };
    } // end of namespace my::ex9::internal
  }


  /*---------------------.
  | internal::point2d_.  |
  `---------------------*/

  namespace ex9
  {
    // Forward declarations.
    namespace internal
    {
      template <typename Exact> struct point2d_;
    }
  }

  /// Super type.
  template<typename Exact>
  struct super_trait_< ex9::internal::point2d_<Exact> >
  {
    typedef ex9::internal::point_base_<Exact> ret;
  };

  /// Virtual types associated to internal::point2d_<Exact>.
  template <typename Exact>
  struct vtypes< ex9::internal::point2d_<Exact> >
  {
  };
  
  namespace ex9
  {
    // Actual definition.
    namespace internal
    {

      template <typename Exact>
      struct point2d_ : public point_base_<Exact>
      {
	typedef point_base_<Exact> super;
      };
    }
  }


  /*---------.
  | grid2d.  |
  `---------*/

  // Forward declaration.
  namespace ex9
  {
    struct grid2d;
  }

  /// Set super type.
  template <>
  struct super_trait_< ex9::grid2d >
  {
    typedef stc::none ret;
  };

  /// Virtual types associated to grid2d.
  template <>
  struct vtypes< ex9::grid2d >
  {
    typedef mlc::uint_<2u> dim;
  };

  // Actual definition.
  namespace ex9
  {
    struct grid2d :  public stc::none
    {
    };
  }


  /*----------.
  | point2d.  |
  `----------*/

  // Forward declaration.
  namespace ex9
  {
    struct point2d;
  }

  /// Set super type.
  template <>
  struct super_trait_< ex9::point2d >
  {
    typedef ex9::internal::point2d_<ex9::point2d> ret;
  };


  /// Virtual types associated to point2d.
  template <>
  struct vtypes< ex9::point2d >
  {
    typedef ex9::grid2d grid;
  };

  // Actual definition.
  namespace ex9
  {
    struct point2d : public internal::point2d_< point2d >
    {
    };
  }


  /*---------------------.
  | Client (test) code.  |
  `---------------------*/

  namespace ex9
  {
    void
    check()
    {
      mlc::assert_<
 	mlc_eq(stc_find_type_(my::ex9::point2d, dim), mlc::uint_<2u>)
	>::check();
      
      assert(ex9::point2d::dim_val == 2);

      point2d p;
    }

  } // end of namespace my::ex9


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
  my::ex8::check();
  my::ex9::check();
}
