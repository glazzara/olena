// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/pw/all.hh>

#include <mln/io/pbm/load.hh>
#include <mln/data/compare.hh>

#include <mln/morpho/reconstruction/by_dilation/union_find.hh>

#include "tests/data.hh"



int main()
{
  using namespace mln;
  using namespace morpho::reconstruction::by_dilation;

  image2d<bool> lena, lena_2, out;
  io::pbm::load(lena, MLN_IMG_DIR "/tiny.pbm");

  {
    out = union_find(lena, lena, c4());
    mln_assertion(out == lena);
  }

  // FIXME: Is this part really meaningful as-is?
 {
    initialize(lena_2, lena);
    data::fill(lena_2, lena);
    out = union_find(lena_2, lena, c4());
  }
}
