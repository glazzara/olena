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

# include <vector>

# include <mln/core/image2d.hh>
# include <mln/io/pgm/save.hh>
# include <mln/io/pgm/load.hh>
# include <mln/io/pbm/save.hh>
# include <mln/io/ppm/save.hh>
# include <mln/debug/println.hh>
# include <mln/logical/not.hh>
# include <mln/logical/or.hh>
# include <mln/io/pbm/save.hh>

# include <mln/morpho/erosion.hh>
# include <mln/morpho/dilation.hh>
# include <mln/morpho/complementation.hh>
# include <mln/morpho/closing.hh>
# include <mln/morpho/opening.hh>
# include <mln/make/w_window2d.hh>
# include <mln/win/rectangle2d.hh>
# include <mln/win/vline2d.hh>
# include <mln/win/hline2d.hh>
# include <mln/win/disk2d.hh>
# include <mln/level/fill.hh>
# include <mln/labeling/blobs.hh>
# include <mln/core/neighb2d.hh>

# include <mln/accu/mean.hh>
# include <mln/accu/bbox.hh>
# include <mln/accu/p.hh>
# include <mln/accu/count.hh>

# include <mln/value/scalar.hh>
# include <mln/make/voronoi.hh>
# include <mln/geom/seeds2tiling.hh>
# include <mln/core/mesh_p.hh>
# include <mln/draw/mesh.hh>

# include <mln/level/stretch.hh>

# include <mln/core/image_if_value.hh>
# include <mln/core/sub_image.hh>
# include <mln/level/paste.hh>
# include <mln/literal/all.hh>
# include <mln/value/rgb8.hh>
# include <mln/morpho/opening_area.hh>

using namespace mln;

using value::int_u8;
using value::rgb8;

typedef point2d P;
typedef image2d<bool> IB;
typedef image2d<int_u8> IU;
typedef image2d<value::rgb8> O;

template <typename I>
void
draw_bbox(I& image, box2d box, value::rgb8 val)
{
  point2d pmin = box.pmin();
  point2d pmax = box.pmax();

  point2d p1 (pmin[0], pmin[1]);
  point2d p2 (pmax[0], pmin[1]);
  point2d p3 (pmin[0], pmax[1]);
  point2d p4 (pmax[0], pmax[1]);

  draw::line (image, p1, p2, val);
  draw::line (image, p2, p4, val);
  draw::line (image, p4, p3, val);
  draw::line (image, p3, p1, val);
}

IB to_binary(IU in, mln_value_(IU) val)
{
//   Divise L'image en deux :
//   box2d gb = score_u.domain();
//   box2d sb(gb.nrows() / 2, gb.ncols() / 2);

//   image2d< accu::mean_<int_u8> > ima_mean(sb);

//   {
//     mln_piter_(IU) p(gb);

//     for_all(p)
//       ima_mean(point2d(p[0] / 2, p[1] / 2)).take(score_u(p));
//   }

  IB score_b(in.domain());

  {
    mln_piter_(IU) p(in.domain());

    for_all(p)
      score_b(p) = in(p) < val;
//       score_b(p) = ima_mean(p).to_result() < 175;
  }
  return score_b;
}

int
main(int argc, char** argv)
{

  if (argc != 3)
    {
      std::cerr << "Usage : " << argv[0]
		<< " input.pgm output.pbm" << std::endl;
      exit(1);
    }

  IU score_u;

  io::pgm::load(score_u, argv[1]);

  IB score_b = to_binary(score_u, 175);

  io::pbm::save(score_b, argv[2]);
}

