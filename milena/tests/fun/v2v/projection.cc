// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

/// \file tests/fun/v2v/projection.cc
///
/// Test on mln::fun::v2v::projection.

#include <mln/fun/v2v/projection.hh>

#include <mln/core/alias/point3d.hh>
#include <mln/core/alias/point2d.hh>
#include <mln/core/alias/point1d.hh>


int main()
{
  using namespace mln;

  {
    typedef point2d P;
    P p(0,1);
    {
      fun::v2v::projection<P,0> f;
      mln_assertion(f(p) == point1d(1));
    }
    {
      fun::v2v::projection<P,1> f;
      mln_assertion(f(p) == point1d(0));
    }
  }

  {
    typedef point3d P;
    P p(0,1,2);
    {
      fun::v2v::projection<P,0> f;
      mln_assertion(f(p) == point2d(1,2));
    }
    {
      fun::v2v::projection<P,1> f;
      mln_assertion(f(p) == point2d(0,2));
    }
    {
      fun::v2v::projection<P,2> f;
      mln_assertion(f(p) == point2d(0,1));
    }
  }

}
