// Copyright (C) 2006, 2007, 2009 EPITA Research and Development
// Laboratory
//
// This file is part of the Milena Library.  This library is free
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

/// \file tests/value/graylevel_f_full.cc
///
/// \brief Full tests on mln::value::graylevel_f.

#include <mln/value/graylevel.hh>
#include <mln/value/glf.hh>
#include <mln/value/gl8.hh>
#include <mln/value/gl16.hh>
#include <mln/literal/all.hh>


int main()
{
  using namespace mln;
  using value::glf;
  using value::gl8;
  using value::gl16;
  using value::graylevel;

  using literal::black;
  using literal::medium_gray;
  using literal::white;

  // Constructions
  {
    glf a;

    glf b = 0.42;
    mln_assertion(b.value() == 0.42f);

    glf c = 0.42;
    mln_assertion(c.value() == 0.42f);

    glf d = c;
    mln_assertion(d.value() == 0.42f);

    glf e = 0;
    mln_assertion(e.value() == 0.f);

    graylevel<24> f = mlc_pow_int(2, 24) - 1;
    glf g = f;
    mln_assertion(g.value() == 1.f);

    graylevel<24> h = 0;
    glf i = h;
    mln_assertion(i.value() == 0.f);
  }

  // Literals
  {
    glf a(white);
    mln_assertion(a == glf(white));
    mln_assertion(a.value() == 1.f);
    glf d = white;
    mln_assertion(d == glf(white));
    mln_assertion(d.value() == 1.f);

    glf b(black);
    mln_assertion(b == glf(black));
    mln_assertion(b.value() == 0.f);
    glf e = black;
    mln_assertion(e == glf(black));
    mln_assertion(e.value() == 0.f);

    glf c(medium_gray);
    mln_assertion(c == glf(medium_gray));
    mln_assertion(c.value() == 0.5f);
    glf f = medium_gray;
    mln_assertion(f == glf(medium_gray));
    mln_assertion(f.value() == 0.5f);
  }

  // FIXME : addition, multiplication, division.
}
