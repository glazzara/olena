// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

/// \file
/// \brief Alternate Sequential Filters in graph space implemented on
/// mln::image2d-based cubical complexes.

#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>

#include "apps/graph-morpho/morpho.hh"

#include "apps/graph-morpho/make_complex2d.hh"
#include "apps/data.hh"


int main()
{
  using namespace mln;

  // Remove borders so as to avoid side effects.
  border::thickness = 0;

  // Input image.
  image2d<bool> input =
    io::pbm::load(MLN_APPS_DIR "/graph-morpho/zebra-noisy.pbm");

  // Create an binary 2d image representing a 4-connexity graph image
  // (actually, a cubical 2-complex) by doubling the resolution of the
  // input image to insert edges (and squares) between pixels.
  image2d<bool> input_x2 = make_complex2d(input);

  // ASF.
  io::pbm::save(unmake_complex2d(asf(input_x2, 2)), "asf_ima_2.pbm");
}
