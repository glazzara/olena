// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

/*! \file tests/core/image2d.cc
 *
 * \brief Tests on mln::image2d.
 */

#include <mln/core/image/image2d.hh>
#include <mln/geom/size2d.hh>

#include <mln/core/routine/init.hh>
#include <mln/border/resize.hh>



int main()
{
  using namespace mln;

  const unsigned nrows = 1;
  const unsigned ncols = 66;
  const unsigned border = 4;
  const unsigned new_border = 3;

  image2d<int> f(nrows, ncols, border);

  mln_assertion(f.nrows() == nrows);
  mln_assertion(f.ncols() == ncols);

  {
    const void* f_id = f.id_();
    image2d<int> g;
    mln_assertion(g.id_() == 0);
    g = f;
    border::resize(g, new_border);
    mln_assertion(g.id_() == f_id);
  }

  mln_assertion(f.nsites() == geom::nrows(f) * geom::ncols(f));
  mln_assertion(f.nelements()  == (nrows + 2 * new_border) * (ncols + 2 * new_border));
}
