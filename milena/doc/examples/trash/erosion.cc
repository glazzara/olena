// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

/// \file doc/examples/erosion.cc
///
/// Test on mln::morpho::hit_or_miss.

# include <mln/core/image/image2d.hh>

# include <mln/core/image/image_if.hh>
# include <mln/core/image/sub_image.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/core/var.hh>
# include <mln/core/site_set/p_vaccess.hh>

# include <mln/binarization/threshold.hh>
# include <mln/morpho/hit_or_miss.hh>
# include <mln/border/fill.hh>
# include <mln/io/pbm/load.hh>
# include <mln/io/pgm/load.hh>
# include <mln/io/pbm/save.hh>
# include <mln/io/pgm/save.hh>
# include <mln/debug/println.hh>
# include <mln/morpho/opening.hh>
# include <mln/trait/value_.hh>
# include <mln/value/int_u8.hh>
# include <mln/data/paste.hh>
# include <mln/labeling/blobs.hh>
# include <mln/pw/all.hh>
# include <mln/convert/to_fun.hh>
# include <mln/geom/bbox.hh>


void clean_lines(mln::image2d<bool>& in,
		 const mln::image2d<bool>& ima,
		 unsigned bbox_larger)
{
  using namespace mln;
  using value::label_8;

  label_8 nlabels;
  image2d<label_8> lbl = labeling::blobs(ima, c4(), nlabels);

  for (unsigned i = nlabels; i > 0; --i)
  {
    data::paste(pw::cst(false)
		    | geom::bbox(lbl | (pw::value(lbl) == pw::cst(i))),//.to_larger(bbox_larger),
		    in);
  }
}


int main(int argc, char*argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc < 2)
  {
    std::cout << argv[0] << " <image.pgm> <height> <width>" << std::endl;
    return 1;
  }

  image2d<bool> in;
  io::pbm::load(in, argv[1]);

  unsigned h = atoi(argv[2]);
  unsigned w = atoi(argv[3]);
  unsigned n = atoi(argv[4]);

  // Remove vertical lines.
  win::rectangle2d vwin(h, w);
  image2d<bool> vout = morpho::opening(in, vwin);
  io::pbm::save(vout, "./vout.pbm");
  clean_lines(in, vout, n);

  // Remove Horizontal lines.
  win::rectangle2d hwin(w, h);
  image2d<bool> hout = morpho::opening(in, hwin);
  io::pbm::save(hout, "./hout.pbm");
  clean_lines(in, hout, n);

  io::pbm::save(in, "./out.pbm");
}
