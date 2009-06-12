// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
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

# include <vector>

# include <mln/core/image/image2d.hh>
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
# include <mln/morpho/closing/structural.hh>
# include <mln/morpho/opening/structural.hh>
# include <mln/make/w_window2d.hh>
# include <mln/win/rectangle2d.hh>
# include <mln/win/vline2d.hh>
# include <mln/win/hline2d.hh>
# include <mln/win/disk2d.hh>
# include <mln/data/fill.hh>
# include <mln/labeling/blobs.hh>
# include <mln/core/alias/neighb2d.hh>

# include <mln/accu/stat/mean.hh>
# include <mln/accu/shape/bbox.hh>
# include <mln/accu/p.hh>
# include <mln/accu/math/count.hh>

# include <mln/value/scalar.hh>
# include <mln/make/voronoi.hh>
# include <mln/geom/seeds2tiling.hh>
# include <mln/core/mesh_p.hh>
# include <mln/draw/mesh.hh>

# include <mln/data/stretch.hh>
# include <mln/data/threshold.hh>
# include <mln/linear/gaussian.hh>

# include <mln/core/image_if_value.hh>
# include <mln/core/image/dmorph/sub_image.hh>
# include <mln/data/paste.hh>
# include <mln/literal/all.hh>
# include <mln/value/rgb8.hh>
# include <mln/morpho/opening_area.hh>

using namespace mln;

using value::int_u8;
using value::rgb8;

typedef point2d P;
typedef image2d<bool> IB;
typedef image2d<int_u8> IU8;
typedef image2d<unsigned int> IU;
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

IB to_binary(IU8 in, mln_value_(IU8) val)
{
//   Divise L'image en deux :
//   box2d gb = score_u.domain();
//   box2d sb(gb.nrows() / 2, gb.ncols() / 2);

//   image2d< accu::mean_<int_u8> > ima_mean(sb);

//   {
//     mln_piter_(	IU8) p(gb);

//     for_all(p)
//       ima_mean(point2d(p[0] / 2, p[1] / 2)).take(score_u(p));
//   }

  IB score_b(in.domain());

  {
    mln_piter_(	IU8) p(in.domain());

    for_all(p)
      score_b(p) = in(p) < val;
//       score_b(p) = ima_mean(p).to_result() < 175;
  }
  return score_b;
}

void label_and_stat(IB& ima)
{
  unsigned nlabels;
  IU labelled(ima.domain());

  labelled = labeling::blobs(ima, c4(), nlabels);

  
}

IU8 projection(IB ima, int coord)
{
  box2d smb = ima.domain();

  point2d& pt = smb.pmax();
  pt[coord] = 0;

  IU projected(smb);
  IU8 projected_stretched(smb);
  data::fill(projected, literal::zero);

  mln_piter_(IB) p(ima.domain());

  point2d pt2;
  for_all(p)
  {
    pt2 = p;
    pt2[coord] = 0;
    projected(pt2) += ima(p);
  }
  data::stretch(projected, projected_stretched);
  return projected_stretched;
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

  IU8 score_u;

  io::pgm::load(score_u, argv[1]);

  IB score_b = to_binary(score_u, 175);

  io::pbm::save(score_b, argv[2]);


//   { // Obtenir un histogramme vertical pour reperer les lignes de portée.
//     IU8 histo_col = projection(score_b, 1);
    
//     IU8 histo_col_blur(histo_col.domain());
//     linear::gaussian(histo_col, 5, histo_col_blur);
//     io::pgm::save(histo_col_blur, "histo_col_blur.pgm");

//     data::stretch(histo_col, histo_col);
//     io::pgm::save(histo_col, "histo_col.pgm");
//     IB histo_col_b(histo_col.domain());
//     histo_col_b = data::threshold(histo_col, mln_value_(IU8)(175));

//     io::pbm::save(histo_col_b, "histo_col_b.pbm");
//   }

//   { // Labelling des grosses composant connexes pour étiqueter les differentes portées.
//     IB only_big(score_b.domain());
//     morpho::opening_area(score_b, c4(), 5000, only_big);

//     unsigned nlabels;
//     IU inter_labeled = labeling::blobs(only_big, c4(), nlabels);  // labelling blobs don't use algo(input, output) formalism
//     IU8 labeled(inter_labeled.domain());                         // and it should return int_u8 ?
//     data::stretch(inter_labeled, labeled);
//     io::pgm::save(labeled, "labeled.pgm");
//   }

  { // fermeture avec un disk et un element vertial pour reperer les notes.
    IB closed1 = morpho::opening::structural(score_b, win::vline2d(5));
    IB closed2 = morpho::opening::structural(score_b, win::disk2d(5));
    morpho::opening_area(closed1, c4(), 20, closed1);
    morpho::opening_area(closed2, c4(), 15, closed2);
    io::pbm::save(closed1, "closed1.pbm");

    label_and_stat(closed2);
    io::pbm::save(closed2, "closed2.pbm");
  }

  { // Fermeture avec un element struturant en horizontale pour obtenir les lignes de portée.
//     IB hclosed = morpho::closing::structural(score_b, win::hline2d(73));
//     io::pbm::save(hclosed, "hclosed.pbm");
  }
}

