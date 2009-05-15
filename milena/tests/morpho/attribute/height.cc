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

/// \file tests/morpho/attribute/height.cc
///
/// Test on mln::morpho::attribute::height.
#include <mln/core/image/image1d.hh>
#include <mln/morpho/attribute/height.hh>
#include <iostream>

int main()
{
  using namespace mln;

  int t[5] = {3, 3, 8, 9, 10};

  typedef morpho::attribute::height< image1d<int> > A;

  A accu;
  for (int i = 0; i < 5; i++)
      accu.take(t[i]);

  mln_assertion(accu.to_result() == 7u);


  A accu2;
  accu = A();
  for (int i = 4; i >= 0; i--)
    {
      accu.take(t[i] + 2);
      accu2.take(t[i] - 2);
    }
  mln_assertion(accu.to_result() == 7u);
  mln_assertion(accu2.to_result() == 7u);
  accu.take(accu2);
  mln_assertion(accu.to_result() == 11u);
}

