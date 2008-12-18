// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_PROJ_HH
# define MLN_PROJ_HH

#include <mln/io/pgm/save.hh>
#include <mln/level/paste.hh>
#include <mln/accu/mean.hh>
#include <mln/literal/white.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image1d.hh>
#include <mln/level/fill.hh>
#include <mln/opt/at.hh>

namespace mln
{
  template <typename T>
  image1d<float>
  proj_nat(const image2d<T>& input)
  {
    image1d<float> out(geom::nrows(input));
    level::fill(out, 0);

    mln_piter(image2d<T>) p(input.domain());
    for_all(p) // 2d
      opt::at(out, p.row()) += input(p);

    mln_piter(image1d<T>) p2(out.domain());
    unsigned count = geom::ncols(input);
    for_all(p2) {
      out(p2) /= count;
    }
    return out;
  }

} // end of namespace mln

#endif /* MLN_PROJ_HH */

