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
# include <mln/io/pbm/load.hh>
# include <mln/io/pgm/save.hh>
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

using namespace mln;

int
main(int argc, char** argv)
{
  using value::int_u8;

  typedef image2d<bool> I;
  typedef image2d<unsigned> I_LABEL;
  if (argc != 3)
    {
      std::cerr << "Usage : " << argv[0]
		<< " input.pbm output.pbm" << std::endl;
      exit(1);
    }

  std::string path_input = argv[1];
  std::string path_output = argv[2];

  image2d<bool> input = io::pbm::load (path_input);

  /// Inversion video.
  morpho::complementation_inplace(input);


  /// FIXME : Some code was deleted here :-) (Remove noise)
//   win::disk2d win (2);
//   I output = morpho::closing(input, win);
//   io::pbm::save (output, path_output);

/// Labeling.
  unsigned nb_labels;
  I_LABEL label_image = labeling::blobs(input, c4(), nb_labels);
  std::cout << "nb_labels = " << nb_labels << std::endl;


  /// Extraction of informations in the image (BoundingBox, Cardinality, ... ).

//   std::vector< accu::mean_ <point2d> > vec_mean (nb_labels + 1);
//   std::vector< accu::p<accu::mean<point2d> >::ret > vec_mean (nb_labels + 1);
  std::vector< accu::bbox  <point2d> > vec_bbox (nb_labels + 1);
  std::vector< accu::count_<point2d> > vec_card (nb_labels + 1);


  mln_piter_(I_LABEL) p (label_image.domain ());
  for_all (p)
  {
//     vec_mean[label_image(p)].take (p);
    vec_bbox[label_image(p)].take (p);
    vec_card[label_image(p)].take (p);
  }

  I_LABEL text_ima (label_image.domain());

  for (unsigned i = 0; i <= nb_labels; ++i)
    {
      float ratio = 0;
      unsigned rows = vec_bbox[i].to_result().nrows();
      unsigned cols = vec_bbox[i].to_result().ncols();
      unsigned size = rows * cols;
      unsigned card = vec_card[i].to_result();

//       if (rows < cols)
// 	ratio = cols / rows;
//       else
// 	ratio = rows / cols;

//       if (ratio > 3)
// 	continue;

//       if (vec_card[i].to_result () /*rows * cols*/ < 200)

//       if (size / card < 0.3)
// 	continue;

//       if (((float)card / (float)size) > 0.7)
// 	continue;

//       if (((float)card / (float)size) < 0.3)
// 	continue;


      if (cols < 5)
	continue;

      if (cols > 20)
	continue;


	{
	  typedef sub_image<I_LABEL, box2d> I_SUB;
	  typedef image_if_value<I_SUB> I_VAL;

	  I_SUB sub_ima = (label_image | vec_bbox[i].to_result());
	  I_VAL val_ima = (sub_ima | i);

	  mln_piter_(I_VAL) p(val_ima.domain());

	  for_all (p)
	    text_ima(p) = val_ima(p);

// 	  /// Hack
//  	  level::paste (val_ima, text_ima);
	}
    }






  /// Generation of the graph.
//   I_LABEL zi_image = geom::seeds2tiling(label_image, c4 ());
//   mesh_p<point2d> m = make::voronoi(label_image, zi_image, c4());

//   I_LABEL gr_image (label_image.domain ());
//   draw::mesh(gr_image, m, 255, 128);

  I ref = io::pbm::load ("ref_text.pbm");
  image2d<value::rgb8> mask (ref.domain());
  mln_piter_(I_LABEL) p_out (text_ima.domain ());

  unsigned cpt = 0;
  unsigned ok = 0;

  for_all (p_out)
    {
      if (text_ima(p_out))
	if (ref(p_out))
	  mask(p_out) = literal::red;
	else
	  {
	    mask(p_out) = literal::white;
	    ++ok;
	  }
      else
	if (ref(p_out))
	  {
	    ++ok;
	    mask(p_out) = literal::black;
	  }
	else
	  mask(p_out) = literal::green;
      ++cpt;
    }

  std::cout << "Reussite : " << (float)ok / (float)cpt * 100 << "% ("
	    << ok << "/" << cpt << ")." << std::endl;

  image2d<int_u8> out (label_image.domain ());
  level::stretch (text_ima, out);
  io::pgm::save(mask, "rgb.ppm");
  io::pgm::save(out, "out.pgm");

  std::cout << path_output << " generated" << std::endl;

}

