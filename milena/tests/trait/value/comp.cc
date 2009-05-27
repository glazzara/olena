// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

/// \file tests/trait/value/comp.cc
///
/// \brief Tests on mln_trait_value_comp.
///
/// \todo Add tests on some actual heterogeneous types.

#include <mln/trait/value/comp.hh>

#include <mln/value/int_u8.hh>
#include <mln/algebra/vec.hh>




template <typename T, unsigned i, typename C>
void check_equal()
{
  mlc_equal(mln_trait_value_comp(T, i), C)::check();
}


template <typename T>
void check_scalar()
{
  mlc_equal(mln_trait_value_comp(T, 0), T)::check();
}


template <typename T, typename C>
void check_homogeneous()
{
  mlc_equal(mln_trait_value_comp(T, 0), C)::check();
}


namespace mln
{

  template <typename R, typename G, typename B>
  struct my_rgb;

  namespace trait
  {

    template <typename R, typename G, typename B>
    struct value_< my_rgb<R,G,B> >
    {
      enum { dim = 3 };
      typedef void comp;
      typedef R comp_0;
      typedef G comp_1;
      typedef B comp_2;
    };

  } // mln::trait

} // mln



template <typename T,
	  typename C0, typename C1, typename C2>
void check_heterogeneous()
{
  mlc_equal(mln_trait_value_comp(T, 0), C0)::check();
  mlc_equal(mln_trait_value_comp(T, 1), C1)::check();
  mlc_equal(mln_trait_value_comp(T, 2), C2)::check();
}



int main()
{
  using namespace mln;


  // Scalars.

  {
    typedef mln::value::int_u8 T;
    check_scalar<T>();
    check_equal<T, 0, T>();
  }
  {
    typedef int T;
    check_scalar<T>();
    check_equal<T, 0, T>();
  }


  // Homogeneous vectors.

  {
    typedef float C;
    typedef algebra::vec<2, C> T;
    check_homogeneous< T, C >();
    check_equal< T, 0, C >();
    check_equal< T, 1, C >();
  }
  {
    typedef double C;
    typedef C T[2];
    check_homogeneous< T, C >();
    check_equal< T, 0, C >();
    check_equal< T, 1, C >();
  }


  // Heterogeneous types.

  {
    check_heterogeneous< my_rgb< bool, unsigned, float* >,
                                 bool, unsigned, float* >();
  }
}
