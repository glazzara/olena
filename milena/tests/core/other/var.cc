// Copyright (C) 2008 EPITA Research and Development Laboratory
// (LRDE)
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

/// \file tests/core/other/var.cc
///
/// Tests all macros located in mln/core/var.hh

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/histo/data.hh>
#include <mln/util/array.hh>

#include <mln/core/var.hh>

template <typename T>
void test_template()
{
  using namespace mln;

  image2d<T> ima(1,1);
  neighb2d neighb;
  window2d win;
  {
    mln_PITER(p, ima.domain());
    mln_NITER(n, neighb, p);
    mln_QITER(q, win, p);
  }
  {
    mln_FWD_PITER(p, ima.domain());
    mln_FWD_NITER(n, neighb, p);
    mln_FWD_QITER(q, win, p);
  }
  {
    mln_BKD_PITER(p, ima.domain());
    mln_BKD_NITER(n, neighb, p);
    mln_BKD_QITER(q, win, p);
  }

  util::array<point2d> arr;
  {
    mln_EITER(e, arr);
  }
  {
    mln_FWD_EITER(e, arr);
  }
  {
    mln_BKD_EITER(e, arr);
  }

  histo::data<bool> d;
  {
    mln_VITER(v, d.vset());
  }
  {
    mln_FWD_VITER(v, d.vset());
  }
  {
    mln_BKD_VITER(v, d.vset());
  }

}

void test()
{
  using namespace mln;

  image2d<int> ima(1,1);
  neighb2d neighb;
  window2d win;
  {
    mln_PITER_(p, ima.domain());
    mln_NITER_(n, neighb, p);
    mln_QITER_(q, win, p);
  }
  {
    mln_FWD_PITER_(p, ima.domain());
    mln_FWD_NITER_(n, neighb, p);
    mln_FWD_QITER_(q, win, p);
  }
  {
    mln_BKD_PITER_(p, ima.domain());
    mln_BKD_NITER_(n, neighb, p);
    mln_BKD_QITER_(q, win, p);
  }

  util::array<point2d> arr;
  {
    mln_EITER_(e, arr);
  }
  {
    mln_FWD_EITER_(e, arr);
  }
  {
    mln_BKD_EITER_(e, arr);
  }

  histo::data<bool> d;
  {
    mln_VITER_(v, d.vset());
  }
  {
    mln_FWD_VITER_(v, d.vset());
  }
  {
    mln_BKD_VITER_(v, d.vset());
  }
}

int main()
{
  test();
  test_template<int>();
}
