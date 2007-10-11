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

/*! \file tests/value_graylevel.cc
 *
 * \brief Tests on mln::value::graylevel. Tests operations between
 * graylevel of different encodings.
 *
 */


#include <mln/literal/black.hh>

#include <mln/literal/white.hh>

#include <mln/value/graylevel.hh>


int main()
{
  using namespace mln::value;

  using  mln::literal::white;
  using  mln::literal::black;

  // Literals
  {
    gl8  a = white;
    gl16 b = white;

    mln_assertion(a == b);
    mln_assertion(a.value() == 255);
    mln_assertion(b.value() == 65535);

    gl8 c = (white + white) / 2;
    mln_assertion(c == white);
    mln_assertion(c.value() == 255);

    a = black;
    b = black;

    mln_assertion(a == b);
    mln_assertion(a.value() == 0);
    mln_assertion(b.value() == 0);

    c = (black + black) / 2;
    mln_assertion(c == black);
    mln_assertion(c.value() == 0);
  }

  // Assigment
  {
    gl8 a;
    gl8 b;

    a = white;
    mln_assertion(a == white);
    mln_assertion(a.value() == 255);

    a = 23;
    mln_assertion(a != white);
    mln_assertion(a != black);
    mln_assertion(a.value() == 23);

    b = 2;
    mln_assertion(b != white);
    mln_assertion(b != black);
    mln_assertion(b.value() == 2);

    a = b;
    mln_assertion(a.value() == 2);

    signed char c = 51;
    a = c;
    mln_assertion(a.value() == 51);

    // bounds
    a = 255;
    mln_assertion(a.value() == 255);
    a = 0;
    mln_assertion(a.value() == 0);
  }

  // Addition
  {
    gl8 a;
    gl16 b;

    // gl8 <- gl8 + gl8
    a = 42;
    b = 16969;
    a += a;
    mln_assertion(a.value() == 84);

    a = 42;
    b = 16969;
    a = a + a;
    mln_assertion(a.value() == 84);

    // gl8 <- gl8 + gl16
    a = 42;
    b = 16969;
    a = a + b;
    mln_assertion(a.value() == (42 + b.value() / 257) );
    a = 42;
    b = 16969;
    a += b;
    mln_assertion(a.value() == (42 + b.value() / 257) );


    // gl16 <- gl8 + gl16
    a = 42;
    b = 16969;
    b += a;
    mln_assertion(b.value() == (42 * 257 + 16969) );

    a = 42;
    b = 16969;
    b = a + b;
    mln_assertion(b.value() == (42 * 257 + 16969) );

    // misc
    a = 255;
    b = 0;
    a = a + b;
    mln_assertion(a.value() == 255);

    a = 0;
    b = 65535;
    a = a + b;
    mln_assertion(a.value() == 255);
  }



  // Not exhaustive

  //   gl8  a = white;
  //   gl16 b = white;
  //   mln_assertion(a == b);
  //   //  mln_assertion(-a == -b);

  //   mln_assertion(a.value() != b.value());

  //   {
  //     gl8 c = (a + b) / 2;
  //     mln_assertion(c == white);
  //     mln_assertion(c.value() == 255);

  //     c = a;
  //     mln_assertion(c == white);

  //     c = (a * 2) / 2;
  //     mln_assertion(c == white);

  //     c = c / 6;
  //   }

  //   {
  //     gl8 c = (white + white) / 2;
  //     mln_assertion(c == white);
  //     mln_assertion(c.value() == 255);
  //   }

  //   gray g = black;
}
