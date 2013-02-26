// Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#include <mln/value/gl8.hh>
#include <mln/value/gl16.hh>
#include <mln/value/glf.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_s8.hh>
#include <mln/value/float01_f.hh>
#include <mln/value/float01_.hh>

#include <mln/literal/black.hh>
#include <mln/literal/white.hh>


template<typename From, typename To, typename T>
void
test_conversion (const T& val)
{
  (void)(To)(From) val;
  To test = (From)(val);
  test = (From)(val);
}


int main()
{
  using namespace mln::value;
  using namespace mln::value::internal;

  using  mln::literal::white;
  using  mln::literal::black;


  gl8  a(white);
  gl8  b(white);

  a = b;
  mln_assertion(a == b);

  {
    gl8 a(10);
    gl8 b(10);

    gl8 c = a + b;
    (void) c;
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
    /* FIXME: The compiler emits a warning about a comparison between
       signed and unsigned for the first line, but not for the second.
       This behavior is strange, since

         a * int_u(23);

       triggers no warning.  See whether traits and conversions are
       handled symmetrically for `*' and `/'.  */
    a / int_u8(23);
    a / int_s8(23);

    // gl8 / Floating
    a / float01_f(.23);
    a / float01_<16>(.23);
  }

  {
    // Conversions.

    typedef mln::value::internal::gray_<8> i_gray_8;

    // gray_<n> -> graylevel<n>
    test_conversion<i_gray_8, gl8>(255);

    // gray_f -> graylevel<n>
    test_conversion<gray_f, gl8>(0.4);
    // graylevel_f -> graylevel<n>
    test_conversion<glf, gl8>(0.4);

    // gray_<n> -> graylevel_f
    test_conversion<i_gray_8, glf>(255);
    // gray_f -> graylevel_f
    test_conversion<gray_f, glf>(0.4);
    // graylevel<n> -> graylevel_f
    test_conversion<gl8, glf>(142);

    // gray_<n> -> gray_f
    test_conversion<i_gray_8, gray_f>(4);


    // gray_f -> gray_<n>
    test_conversion<gray_f, i_gray_8>(0.4);

    // graylevel_f -> gray_f
    test_conversion<glf, gray_f>(0.4);

    // graylevel<n> -> gray_<n>
    test_conversion<gl8, i_gray_8>(142);
  }
}
