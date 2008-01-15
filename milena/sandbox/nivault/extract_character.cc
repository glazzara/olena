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
# include <mln/morpho/opening_area.hh>

using namespace mln;

bool
is_character(box2d box, unsigned cardinality)
{
  unsigned rows = box.nrows();
  unsigned cols = box.ncols();
  unsigned size = rows * cols;
//   float ratio = (float) (rows * cols) / cardinality;

  /// Remove noise.
  if ((rows < 5) && (cols < 5))
    return true;

//   if (rows < cols)
//     ratio = cols / rows;
//   else
//     ratio = rows / cols;

//   if (ratio > 3)
//     return true;

//   if (vec_card[i].to_result () /*rows * cols*/ < 200)

//     if (size / card < 0.3)
//       return true;

//   if (((float)card / (float)size) > 0.7)
//     return true;

//   if (((float)card / (float)size) < 0.3)
//     return true;

//   if (cols > 20)
//     return true;

//   if (cols < 10)
//     return true;

  float min = 0.2;
  float max = 0.7;

  if (cardinality < size * min)
    return true;

  if (cardinality > size * max)
    return true;

  return false;
}

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

template <typename I, typename O>
void
extract_lines(I& text_image, O& output,
	      std::vector< accu::bbox  <point2d> >& v_bbox)
{
  typedef image2d<unsigned> I_LABEL;

  typedef std::list< accu::bbox  <point2d> > L;

  L list_lines;

  I image = clone(text_image);

  point2d start = text_image.domain().pmin ();
  point2d end = text_image.domain().pmax ();
  int limits = 50;

  for (int r = start[0]; r < end[0]; ++r)
    {
      for (int c = start[1]; c < end[1]; ++c)
	{
	  bool ok = false;
	  int i = 0;
	  accu::bbox<point2d> accu;
	  {
	    bool okk = true;
	    while (okk)
	      {
		okk = false;
		for (i = c; i < end[1] && i < c + limits; ++i)
		  {
 		    unsigned value = image(point2d(r, i));
		    if (value != 0)
		      {
			typedef sub_image<I_LABEL, box2d> I_SUB;
			typedef image_if_value<I_SUB> I_VAL;

			I_SUB sub_image = (text_image | v_bbox[value].to_result());
			I_VAL val_image = (sub_image | value);

			mln_piter_(I_VAL) p(val_image.domain());

			for_all (p)
			{
			  accu.take(p);
 			  image(p) = 0;
			}
			ok = true;
			okk = true;
			c = i;
		      }
		  }
	      }
	  }
	  if (ok)
	    list_lines.push_back(accu);
	}
    }

  L::const_iterator it = list_lines.begin();
  for (; it != list_lines.end(); ++it)
    {
      box2d b = (*it).to_result ();
      draw_bbox (output, b, literal::green);
    }
}



template <typename I>
void
check_with_ref(I& image, std::string path_ref, std::string path_output)
{
  image2d<bool> ref = io::pbm::load (path_ref);
  image2d<value::rgb8> mask (image.domain());
  unsigned cpt = 0;
  unsigned ok = 0;

  mln_piter(I) p (image.domain ());

  for_all (p)
    {
      if (image(p))
	if (ref(p))
	  mask(p) = literal::red;
	else
	  {
	    mask(p) = literal::white;
	    ++ok;
	  }
      else
	if (ref(p))
	  {
	    ++ok;
	    mask(p) = literal::black;
	  }
	else
	  mask(p) = literal::green;
      ++cpt;
    }
  io::ppm::save(mask, path_output + ".check.ppm");
  std::cout << "Reussite : " << (float)ok / (float)cpt * 100 << "% ("
	    << ok << "/" << cpt << ")." << std::endl;
}


int
main(int argc, char** argv)
{
  using value::int_u8;
  using value::rgb8;

  typedef point2d P;
  typedef image2d<bool> I;
  typedef image2d<unsigned> I_LABEL;
  typedef image2d<value::rgb8> O;

  if (argc != 3)
    {
      std::cerr << "Usage : " << argv[0]
		<< " input.pbm output" << std::endl;
      exit(1);
    }

  std::string path_input = argv[1];
  std::string path_output = argv[2];

  I input = io::pbm::load (path_input);
  O output (input.domain ());

  /// Inversion video.
  morpho::complementation_inplace(input);

  /// Clean input image (remove noise with area opening)
  image2d<bool> input2 (input.domain());
  morpho::opening_area(input, c4(), 5, input2);

//   image2d<int_u8> process (input.domain ());
//   level::stretch (output_2, process);
//   io::pgm::save(process, "process.pgm");


  /// Labeling the input image.
  unsigned nb_labels;
  I_LABEL label_image = labeling::blobs(input2, c8(), nb_labels);
  std::cout << "nb_labels = " << nb_labels << std::endl;

  /// Extraction of informations in the image (BoundingBox, Cardinality, ... ).
  std::vector< accu::bbox  <point2d> > vec_bbox (nb_labels + 1);
  std::vector< accu::count_<point2d> > vec_card (nb_labels + 1);

  mln_piter_(I_LABEL) p (label_image.domain ());
  for_all (p)
  {
    if (input(p))
      output(p) = literal::white;
    vec_bbox[label_image(p)].take (p);
    vec_card[label_image(p)].take (p);
  }

  /// Generation of the text image (text_image).
  I_LABEL text_image (label_image.domain());
  for (unsigned i = 0; i <= nb_labels; ++i)
    {
      /// Evaluation of kind of data (character or not).
      if (is_character(vec_bbox[i].to_result(), vec_card[i].to_result()))
	continue;

      /// Copy text in an image (text_image).
      {
	typedef sub_image<I_LABEL, box2d> I_SUB;
	typedef image_if_value<I_SUB> I_VAL;

	I_SUB sub_image = (label_image | vec_bbox[i].to_result());
	I_VAL val_image = (sub_image | i);

	mln_piter_(I_VAL) p(val_image.domain());


	/// Hack : problem with traits in image_if.
	for_all (p)
	  text_image(p) = val_image(p);

 	/// Here is the good way to do the previous action.
//  	  level::paste (val_ima, text_image
      }
    }


  /// Extraction of lines.
  extract_lines(text_image, output, vec_bbox);


  /// Generation of the graph from text image.
  I_LABEL zi_image = geom::seeds2tiling(text_image, c4 ());
  mesh_p<point2d> m = make::voronoi(zi_image, text_image, c4());
  I_LABEL gr_image (label_image.domain ());
  I_LABEL gr_image2 (label_image.domain ());
  draw::mesh(gr_image, m, 255, 128);
  draw::mesh(gr_image2, m, 255, 0);


  /// Dilation of node of the graph.
  win::disk2d win (30);
  I_LABEL pr = morpho::dilation(gr_image2, win);
  I bool_ima (label_image.domain ());
  level::paste(pr, bool_ima);

  /// Labeling the dilated image.
  unsigned nb_dilated_node_labels;
  I_LABEL label_image2 = labeling::blobs(bool_ima, c8(), nb_dilated_node_labels);
  std::cout << "nb_dilated_node_labels = " << nb_dilated_node_labels << std::endl;
  std::vector< accu::bbox  <point2d> > vec_bbox2 (nb_dilated_node_labels + 1);

  /// Extract area of text.
  mln_piter_(I_LABEL) pl (label_image.domain ());
  for_all (pl)
    vec_bbox2[label_image2(pl)].take (pl);


  /// Draw the bounding box of area of text in output.
  for (unsigned i = 0; i <= nb_dilated_node_labels; ++i)
    draw_bbox(output, vec_bbox2[i].to_result(), literal::red);

  image2d<int_u8> out (label_image.domain ());

  /// Save the influence area of the node of the graphe (seed2tiling result).
  level::stretch (zi_image, out);
  io::pgm::save(out, path_output + ".text_area.approx.pgm");
  std::cout << path_output + ".text_area.approx.pgm" << " generated" << std::endl;

  /// Save the graph of the text.
  level::stretch (gr_image, out);
  io::pgm::save(out, path_output + ".text_graph.pgm");
  std::cout << path_output + ".text_graph.pgm" << " generated" << std::endl;

  /// Save the graph of the text.
  level::stretch (text_image, out);
  io::pgm::save(out, path_output + ".text.pgm");
  std::cout << path_output + ".text.pgm" << " generated" << std::endl;

  /// Save the output image (input image + bounding box text in red).
  io::ppm::save(output, path_output + ".ppm");
  std::cout << path_output + ".ppm" << " generated" << std::endl;

}

