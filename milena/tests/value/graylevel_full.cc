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

/*! \file tests/value/graylevel_full.cc
 *
 * \brief Tests on mln::value::graylevel. Tests operations between
 * graylevel of different encodings.
 *
 */

#include <mln/value/gl8.hh>
#include <mln/value/gl16.hh>
#include <mln/value/int_u8.hh>

#include <mln/literal/black.hh>
#include <mln/literal/white.hh>



int main()
{
  using namespace mln::value;

  using  mln::literal::white;
  using  mln::literal::black;

  gl8 a = 255;
  gl8 b = 255;

  // Constructions
  {
    gl8 x;

    gl8 a = 12;
    gl8 b(12);
    mln_assertion(a == b);

    {
      gl16 c = 2335;
      gl8 d = c;
      mln_assertion(c == d);
    }

    gl8 d = 250;
    gl16 c = d;
    mln_assertion(c == d);

    gl8 e = white;

    mln_assertion(e == white);

    gl8 f = 12;
    gl8 g = f;
    gl8 h(f);

    mln_assertion(f == g);
    mln_assertion(h == g);
  }

  // Literals
  {
    gl8  a(white);
    gl16 b(white);

    a = white;
    b = white;

    mln_assertion(a == b);
    mln_assertion(a.value() == float(255));
    mln_assertion(b.value() == float(65535));
    mln_assertion(a == white);
    mln_assertion(b == white);

    gl8 c(white);
    mln_assertion(c == white);
    mln_assertion(c.value() == float(255));

    a = black;
    b = black;

    mln_assertion(a == b);
    mln_assertion(a.value() == float(0));
    mln_assertion(b.value() == float(0));
  }

  // Assigment
  {
    gl8 a;
    gl16 b;

    a = white;
    mln_assertion(a == white);
    mln_assertion(a.value() == float(255));

    a = 23;
    mln_assertion(a != white);
    mln_assertion(a != black);
    mln_assertion(a.value() == float(23));

    b = 2;
    mln_assertion(b != white);
    mln_assertion(b != black);
    mln_assertion(b.value() == float(2));

    a = b;
    mln_assertion(a.value() == float(2 / 256));

    signed char c = 51;
    a = c;
    mln_assertion(a.value() == float(51));

    // bounds
    a = 255;
    mln_assertion(a.value() == float(255));
    a = 0;
    mln_assertion(a.value() == float(0));
  }

  // Addition
  {
    gl8 a;
    gl16 b;

    // gl8 <- gl8 + gl8
    a = 42;
    a += a;
    mln_assertion(a.value() == float(84));

    a = 42;
    a = a + a;
    mln_assertion(a.value() == float(84));

    // gl8 <- gl8 + gl16
    a = 42;
    b = 16969;
    a = a + b;
    mln_assertion(a.value() == float((42 + b.value() / 257) ));
    a = 42;
    b = 16969;
    a += b;
    mln_assertion(a.value() == float((42 + b.value() / 256) ));


    // gl16 <- gl8 + gl16
    a = 42;
    b = 16969;
    b += a;
    mln_assertion(b.value() == float((42 * 256 + 16969) ));

    a = 42;
    b = 16969;
    b = b + a;

    mln_assertion(b.value() == float((42 * 256 + 16969) ));

    a = 42;
    b = 16969;
    b = a + b;
    mln_assertion(b.value() == float((42 * 256 + 16969) ));

    // misc
    a = 255;
    b = 0;
    a = a + b;
    mln_assertion(a.value() == float(255));

    a = 0;
    b = 65535;
    a = a + b;
    mln_assertion(a.value() == float(255));
  }


  // Soustraction
  {
    gl8 a;
    gl16 b;

    // gl8 <- gl8 - gl8
    a = 42;
    a -= a;
    mln_assertion(a == black);

    a = 42;
    a = a - a;
    mln_assertion(a == black);

    // gl8 <- gl8 - gl16
    a = 42;
    b = 5969;

    a = b;

    {
      a = 42;
      gl16 t;

      t = a - b;
      t = t + b;
      mln_assertion(a == t);
    }

    a = 42;
    a = a - b;
    mln_assertion(a.value() == (42 * 256 - b.value()) / 256 );
    a = 42;
    b = 9969;
    a -= b;
    mln_assertion(a.value() == (42 * 256 - b.value()) / 256 );


    // gl16 <- gl8 - gl16
    a = 100;
    b = 30969;
    b -= a;
    mln_assertion(b.value() == float(30969 - 100 * 256));

    a = 100;
    b = 20969;
    b = a - b;
    mln_assertion(b.value() == float((100 * 256 - 20969) ));

    // misc
    a = 255;
    b = 0;
    a = a - b;
    mln_assertion(a.value() == float(255));

    gl8(255) - gl16(65535);
    mln_assertion( gl8(255) == gl16(65535) );
    a = 255;
    b = 65535;
    a = a - b;
    mln_assertion(a.value() == float(0));

    // ...
    {
      graylevel<2> a = 1;
      graylevel<3> b = 5;
      graylevel<2> c;
      graylevel<3> d;

      c = b - a;
      d = b - a;
      mln_assertion(c == d);
    }

  }

  // Multiplication
  {
    gl8 a;
    gl16 b;

    // gl8 <- gl8 * gl8
    a = 8;
    a *= a;
    mln_assertion(a.value() == 64);

    a = 7;
    a = a * a;
    mln_assertion(a.value() == 49);

    // gl8 <- gl8 * gl16
    a = 10;
    b = 20;
    a = a * b;
    mln_assertion(a.value() == float((10 * 256* b.value())/256));

    a = 10;
    b = 16;
    a *= b;
    mln_assertion(a.value() == float((10 * 256* b.value())/256));

    mln_assertion((gl8(12) * gl16(12345)).to_enc() == float((12 * 256* 12345)));


    // gl16 <- gl8 * gl16
    a = 10;
    b = 24;
    b *= a;
    mln_assertion(b.value() == float((10 * 256 * 24) ));

    a = 10;
    b = 24;
    b = a * b;
    mln_assertion(b.value() == float((10 * 256 * 24) ));

    // misc
    a = 255;
    b = 0;
    a = a * b;
    mln_assertion(a == black);

    a = 0;
    b = 65535;
    a = a * b;
    mln_assertion(a == black);

    // ...
    {
      graylevel<2> a = 1;
      graylevel<3> b = 2;
      graylevel<2> c;
      graylevel<3> d;

       c = a * b;
       d = a * b;
      mln_assertion(c == d);
    }

    {

      // ...
      gl8 a = 7;
      gl16 b = 596;

      gl8 p;
      p = b;

      gl8 q;
      gl8 r;

      q = a * p;
      r = a * b / 256;
    }

  }
  // FIXME : division
}
