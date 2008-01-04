// Copyright (C) 2007 EPITA Research and Development Laboratory
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

/*! \file tests/value/graylevel.cc
 *
 * \brief Tests on mln::value::graylevel. Tests operations between
 * graylevel of different encodings.
 *
 */

#include <mln/value/gl8.hh>
#include <mln/value/gl16.hh>
#include <mln/value/glf.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/float01_f.hh>
#include <mln/value/float01_.hh>


#include <mln/literal/black.hh>
#include <mln/literal/white.hh>



// FIXME : make a more explicit error message.
// template <typename T>
// void foo()
// {
//   typedef mln::value::gl8 g;
// //   mln_trait_op_times(int, mln::value::Integer<g>) tmp;
//   mln_trait_op_times(int, mln::value::Integer<g>) tmp;
// }


#define test_convertion(T1, T2, VAL)		\
{						\
  T1(T2(VAL));					\
  T1 test = T2(VAL);				\
  test = T2(VAL);				\
}

int main()
{
  using namespace mln::value;
  using namespace mln::value::internal;

  using  mln::literal::white;
  using  mln::literal::black;


  // FIXME :  make all the test pass.

  gl8  a(white);
  gl8  b(white);

  a = b;
  mln_assertion(a == b);

  {
    gl8 a(10);
    gl8 b(10);

    gl8 c = a + b;
  }

  {
    gl8 a(white);
    gl8 b(white);
    gl8 c;

    // gl8 * int
    a * 2;
    2 * a;


    // gl8 * double
    a * 2.0;
    2.0 * a;

    // gl8 * bool
    a * false;
    false * a;

    // gl8 * Integer
    a * int_u8(23);
    int_u8(23) * a;

    // gl8 * Floating
    a * float01_f(.23);
    float01_f(.23) * a;

    float01_<16>(.23) * a;
    a * float01_<16>(.23);


    // gl8 / int
    a / 1.5;

    // gl8 / double
    mln_assertion(a / 2.0 == glf(0.5));

    // gl8 / bool
    mln_assertion(a / true == a);

    // gl8 / Integer
    a / int_u8(23);

    // gl8 / Floating
    a / float01_f(.23);
    a / float01_<16>(.23);

  }

  {
    // Convertions.

    test_convertion(gl8, gray_<8>, 255);
    test_convertion(gl8, gray_f, 0.4);
    test_convertion(gl8, glf, 0.4);

    test_convertion(glf, gray_<8>, 255);
    test_convertion(glf, gray_f, 0.4);
    test_convertion(glf, gl8, 142);

    test_convertion(gray_f, gray_<8>, 4);
    test_convertion(glf, gray_f, 0.4);
  }

  {
    // FIXME : comparaison with literals doesn't work
//     c = a;
//     mln_assertion(c == white);

//     c = (a * 2) / 2;
//     mln_assertion(c == white);

//    c = c / 6;
   }

//   {
//     gl8 c = white;
//     mln_assertion(c == white);
//     mln_assertion(c.value() == float(255));
//   }

}
