// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
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

#ifndef DEMAT_HH_
# define DEMAT_HH_

# define for_all_ncomponents(C, NCOMP) \
  for (unsigned C = 1; C <= NCOMP; ++C)

# define for_all_components(C, S) \
  for (unsigned C = 1; C < S.nelements(); ++C)

# define for_all_elements(E, S) \
  for (unsigned E = 0; E < S.nelements(); ++E)

# include <libgen.h>
# include <sstream>
# include <queue>

# include <mln/essential/1d.hh>
# include <mln/essential/2d.hh>
# include <mln/morpho/elementary/dilation.hh>
# include <mln/labeling/background.hh>
# include <mln/transform/influence_zone_geodesic.hh>
# include <mln/debug/draw_graph.hh>
# include <mln/make/graph.hh>
# include <mln/make/region_adjacency_graph.hh>
# include <mln/util/graph.hh>
# include <mln/util/line_graph.hh>
# include <mln/io/txt/save.hh>

# include <mln/canvas/browsing/depth_first_search.hh>
# include <mln/transform/distance_and_influence_zone_geodesic.hh>
# include <mln/fun/l2l/wrap.hh>
# include <mln/fun/meta/all.hh>

# include <tesseract/baseapi.h>

namespace scribo
{

  namespace internal
  {

    using namespace mln;
    using value::label_16;
    using value::label_8;
    using value::rgb8;
    using value::int_u8;


    struct settings_t
    {
      settings_t()
      {
	ero_line_width = 51;
	rank_filter = 6;
	bbox_enlarge = 1 + ero_line_width / 2 - rank_filter;
	bbox_distance = 20;
	min_comp_size = 5;
	max_comp_size = 1000;
	max_dist_lines = 10;
	max_txt_box_height = 100;
	max_cos = 0.994;
	repair_max_dist = 51;
      }

      unsigned bbox_enlarge;
      unsigned ero_line_width;
      unsigned bbox_distance;
      unsigned min_comp_size;
      unsigned max_comp_size;
      unsigned max_dist_lines;
      unsigned repair_max_dist;
      int max_txt_box_height;
      unsigned rank_filter;
      bool treat_tables;
      float max_cos;
    };



    settings_t settings;
    char *input_file = 0;



    /// Returns a formated output file name.
    std::string output_file(const char *name)
    {
      static int file_id = 1;

      std::ostringstream os;
      os << "./"
	 << input_file
	 << "_";

      if (file_id < 10)
	os << "0";

      os << file_id++
	 << "_"
	 << name;
      return os.str();
    }


    /// Returns the edge central sites of a box
    ///
    /// \param[in] b the bbbox
    /// \param[in] dim the dimension used to compute the site.
    /*!
    **
    ** If dim == 0, returns the left and right central sites.
    **
    **	|-------|
    **	X       X
    **	|-------|
    **
    ** If dim == 1, returns the top and bottom central sites.
    **
    **	|---X---|
    **	|       |
    **	|---X---|
    **
    */
    util::couple<point2d, point2d>
    central_sites(const box2d& b, unsigned dim)
    {
      unsigned n = b.pmax()[dim] - b.pmin()[dim];

      point2d p1 = b.center();
      p1[dim] -= n / 2;
      point2d p2 = b.center();
      p2[dim] += n / 2;

      return make::couple(p1, p2);
    }



    /// Draw a list of bboxes and their center in a RGB8 image.
    void draw_component_boxes(image2d<rgb8>& output, const util::array<box2d>& boxes)
    {
      for_all_components(i, boxes)
        if (boxes[i].is_valid())
	{
	  output(boxes[i].center()) = literal::red;
	  draw::box(output, boxes[i], literal::red);
	}
    }



    /// Colorize and save a labeled image.
    template <typename V>
    void save_lbl_image(const image2d<V>& lbl, unsigned nlabels,
		        const char *filename)
    {
      image2d<rgb8> output = labeling::colorize(rgb8(), lbl, nlabels);
      io::ppm::save(output, output_file(filename));
    }







    //-*****************************************
    /// Functions related to Text Recognition.
    //-*****************************************
    /// \{

    /// Passes the text bboxes to Tesseract and store the result in an image of
    /// char.
    /// \param[in] in image from where the text bboxes are extracted.
    /// \param[in] lbl labeled image.
    /// \param[in] tboxes array of text bboxes.
    ///
    /// FIXME: For each text bbox, we create a new image. We may like to avoid that.
    void
    text_recognition(const image2d<bool>& in, const image2d<label_16>& lbl,
		     const util::array<box2d>& tboxes)
    {
      /// Use txt bboxes here with Tesseract
      std::cout << "Text recognition..." << std::endl;

      TessBaseAPI::InitWithLanguage(NULL, NULL, "fra", NULL, false, 0, NULL);
      image2d<char> txt(in.domain());
      data::fill(txt, ' ');

      for_all_components(i, tboxes)
      {
	if (tboxes[i].is_valid())
	{
	  image2d<bool> b(tboxes[i], 0);
	  data::fill(b, false);
	  data::fill((b | (pw::value(lbl) == pw::cst(i))).rw(), true);

	  char* s = TessBaseAPI::TesseractRect(
	      (unsigned char*) b.buffer(),
	      sizeof (bool),		  // Pixel size.
	      b.ncols() * sizeof (bool),  // Row_offset
	      0,			  // Left
	      0,			  // Top
	      b.ncols(),		  // n cols
	      b.nrows());		  // n rows



	  point2d p = tboxes[i].center();
	  p.col() -= (tboxes[i].pmax().col() - tboxes[i].pmin().col()) / 2;
	  if (s != 0)
	    debug::put_word(txt, p, s);
	  free(s);
	}
      }
      io::txt::save(txt, "out.txt");
    }

    /// \}
    //--------------------------------------------------
    /// End of functions related to Text Recognition.
    //--------------------------------------------------





    //-*********************************************
    /// Functions related to the table extraction
    //-*********************************************
    /// \{

    /// Align table lines bboxes according to a given dimension.
    ///
    /// \return A list of the resulting aligned cols. Each integer is actually
    ///		a col number.
    /*
    **
    **	  0 1 3 4 5 6
    **	  ------------	  -------
    ** 0 |- - - - -   |	 | {0,1} |
    ** 1 |        - - |	 | {0,1} |
    ** 2 |            |	 | {1}	 |
    ** 3 |	      |	 |	 |
    ** 4 |	      |	 | {2}	 |
    ** 5 |- -	      |	 | {2}	 |
    ** 6 |	      |	 | {2}	 |
    **
    ** \p hboxes contains all the table lines bboxes. Each bbox is
    ** associated with an id, its location in the array.
    **
    ** For each bbox, its id is marked in a vector. The location is defined,
    ** according to the given parameter \p dim, either by the row or the col
    ** value of the pmin site.
    **
    ** Ids are then propagated in the vector according a small delta value.
    ** if bbox ids are at the same location in the vector, their related bboxes
    ** are likely to be on the same line.
    **
    ** Finally, iterate over the vector until all bboxes have been treated.
    ** For each iteration, the set with a specific number of elements is found
    ** and all bboxes referenced in this set are aligned on the same row or col.
    **
    */
    template <typename P>
    util::array<int>
    align_lines(unsigned nsites,
		int min_coord,
		int max_coord,
		util::array<box<P> >& line_boxes,
		unsigned dim)
    {
      trace::entering("scribo::internal::align_lines");

      mln_precondition(nsites > 0);

      std::cout << "extracting table lines..." << std::endl;
      util::array< util::set<unsigned> > lines;
      lines.resize(nsites);

      // Map components with actual lines.
      for_all_components(i, line_boxes)
      {
	int minline = line_boxes[i].pmin()[dim] - 5;
	minline = (minline < min_coord ? min_coord : minline);
	int maxline = line_boxes[i].pmax()[dim] + 5;
	maxline = (maxline > max_coord ? max_coord : maxline);

	for (int line = minline;
	      line <= maxline; ++line)
	  lines[line].insert(i);
      }

      // Init box2line
      util::array<int> box2line;
      box2line.resize(line_boxes.nelements());
      for_all_elements(i, box2line)
	box2line[i] = -1;

      // Find the line with the highest element count.
      unsigned max_nelts = 0;
      for_all_elements(i, lines)
	if (max_nelts < lines[i].nelements())
	  max_nelts = lines[i].nelements();

      // Aligning lines
      // FIXME: not optimal... Make it faster!
      // We may do too much iterations (while loop) and some of them may
      // be done for nothing...
      util::array<int> newlines;
      while (max_nelts > 0)
      {
	for_all_elements(i, lines)
	  if (lines[i].nelements() == max_nelts)
	  {
	    accu::mean<unsigned> mean;
	    for_all_elements(j, lines[i])
	      if (box2line[lines[i][j]] == -1)
		mean.take(line_boxes[lines[i][j]].center()[dim]);

	    if (mean.is_valid())
	    {
	      for_all_elements(j, lines[i])
		if (box2line[lines[i][j]] == -1)
		{
		  line_boxes[lines[i][j]].pmin()[dim] = mean.to_result();
		  line_boxes[lines[i][j]].pmax()[dim] = mean.to_result();
		  box2line[lines[i][j]] = mean.to_result();
		}
	      newlines.append(mean.to_result());
	    }
	  }
	--max_nelts;
      }

      trace::exiting("scribo::internal::align_lines");
      return newlines;
    }


    /// Align line bboxes verticaly.
    ///
    /// \param[in]	input	     Image from which the line bboxes are
    ///				     extracted from.
    /// \param[in, out] lines_bboxes vertical lines bounding boxes.
    ///
    /// \return A list of the resulting aligned cols. Each integer is actually
    ///		a col number.
    template <typename I>
    util::array<int>
    align_lines_verticaly(const Image<I>& input,
			  util::array<box<mln_site(I)> >& lines_bboxes)
    {
      trace::entering("scribo::internal::align_lines_horizontaly");

      mln_precondition(exact(input).is_valid());
      util::array<int> res = align_lines(geom::ncols(input), geom::min_col(input),
					 geom::max_col(input), lines_bboxes, 1);

      trace::exiting("scribo::internal::align_lines_horizontaly");
      return res;

    }

    /// Align line bboxes horizontaly.
    ///
    /// \param[in]	input	     Image from which the line bboxes are
    ///				     extracted from.
    /// \param[in, out] lines_bboxes horizontal lines bounding boxes.
    ///
    /// \return A list of the resulting aligned rows. Each integer is actually
    ///		a row number.
    template <typename I>
    util::array<int>
    align_lines_horizontaly(const Image<I>& input,
			    util::array<box<mln_site(I)> >& lines_bboxes)
    {
      trace::entering("scribo::internal::align_lines_horizontaly");

      mln_precondition(exact(input).is_valid());
      util::array<int> res =  align_lines(geom::nrows(input), geom::min_row(input),
					  geom::max_row(input), lines_bboxes, 0);

      trace::exiting("scribo::internal::align_lines_horizontaly");
      return res;
    }


    /// Connect vertical and horizontal lines if they are close to each other.
    ///
    ///  ------                 ------
    ///			--->          |
    ///        |                      |
    ///        |                      |
    ///
    template <typename P>
    void
    connect_lines(const util::array<int>& aligned_lines,
		  util::array< box<P> >& boxes,
		  unsigned dim,
		  unsigned dim_size)
    {
      trace::entering("scribo::internal::connect_lines");

      image1d<int> l(dim_size);
      data::fill(l, -1);

      for_all_elements(i, aligned_lines)
	opt::at(l, aligned_lines[i]) = i;

      for (unsigned i = 0; i < settings.max_dist_lines; ++i)
	l = morpho::elementary::dilation(l, c2());

      for_all_components(i, boxes)
      {
	util::couple<point2d, point2d> cp = central_sites(boxes[i], dim);
	if (opt::at(l, cp.first()[dim]) != -1)
	  boxes[i].pmin()[dim] = aligned_lines[opt::at(l, cp.first()[dim])];
	if (opt::at(l, cp.second()[dim]) != -1)
	  boxes[i].pmax()[dim] = aligned_lines[opt::at(l, cp.second()[dim])];
      }

      trace::exiting("scribo::internal::connect_lines");
    }


    /// Connect vertical lines with the new aligned rows.
    template <typename I>
    void
    connect_vertical_lines(const util::array<int>& aligned_rows,
			   util::couple<util::array<box<mln_site(I)> >,
					util::array<box<mln_site(I)> > > tblboxes,
			   const Image<I>& input)
    {
      trace::entering("scribo::internal::connect_vertical_lines");
      mln_precondition(exact(input).is_valid());

      connect_lines(aligned_rows, tblboxes.first(), 0, exact(input).nrows());

      trace::exiting("scribo::internal::connect_vertical_lines");
    }


    /// Connect horizontal lines with the new aligned columns.
    template <typename I>
    void
    connect_horizontal_lines(const util::array<int>& aligned_cols,
			     util::couple<util::array<box<mln_site(I)> >,
					  util::array<box<mln_site(I)> > > tblboxes,
			     const Image<I>& input)
    {
      trace::entering("scribo::internal::connect_horizontal_lines");
      mln_precondition(exact(input).is_valid());

      connect_lines(aligned_cols, tblboxes.second(), 1, exact(input).ncols());

      trace::exiting("scribo::internal::connect_horizontal_lines");
    }


    /// Repair lines with small discontinuities.
    /// FIXME: buggy. Sometimes few lines move or shrink!
    template <unsigned axis, typename I>
    void
    repair_lines(const Image<I>& input_,
		 util::array<box<mln_site(I)> >& tblboxes)
    {
      std::cout << "repairing lines" << std::endl;

      const I& input = exact(input_);
      typedef mln_site(I) P;
      typedef win::line<mln_grid(P), axis, mln_coord(P)> line_t;

      // Initialization
      image2d<unsigned> l(input.domain());
      data::fill(l, 0u);
      for_all_components(i, tblboxes)
      {
	util::couple<point2d, point2d> cp = central_sites(tblboxes[i], axis);
	l(cp.first()) = i;
	l(cp.second()) = i;
      }

      // Repair
      extension_val<image2d<unsigned> > l_ext(l, 0u);

      util::array<box<P> > result;
      std::vector<bool> to_keep(tblboxes.nelements(), true);

      mln_VAR(tbb_ima, extend(l | pw::value(l) != 0u, l));
      line_t vl(settings.repair_max_dist); //FIXME: use a half window, just the bottom of the vertical line.
      mln_piter(tbb_ima_t) p(tbb_ima.domain());
      mln_qiter(line_t) q(vl, p);
      for_all(p)
	for_all(q)
	  if (l_ext(q) != 0u && l_ext(q) != l_ext(p))
	  {
	    to_keep[l_ext(q)] = false;

	    std::cout << "Merging " << tblboxes[l_ext(p)] << " with " << tblboxes[l_ext(q)] << std::endl;
	    tblboxes[l_ext(p)].pmax() = tblboxes[l_ext(q)].pmax();

	    util::couple<P,P> cp = central_sites(tblboxes[l_ext(q)], axis);
	    l_ext(cp.first()) = l_ext(p);
	    l_ext(cp.second()) = l_ext(p);
	  }


      // Remove merged boxes.
      for_all_elements(i, tblboxes)
	if (to_keep[i])
	  result.append(tblboxes[i]);

      std::cout << tblboxes[0] << " - " << result[0] << std::endl;
      std::cout << "previous box count = " << tblboxes.nelements() << " - " << " now = " << result.nelements() << std::endl;
      tblboxes = result;
    }


    template <typename I>
    void
    repair_vertical_lines(const Image<I>& input,
			  util::couple<util::array<box<mln_site(I)> >,
				       util::array<box<mln_site(I)> > >& tblboxes)
    {
      repair_lines<0>(input, tblboxes.first());
    }


    template <typename I>
    void
    repair_horizontal_lines(const Image<I>& input,
			    util::couple<util::array<box<mln_site(I)> >,
					 util::array<box<mln_site(I)> > >& tblboxes)
    {
      repair_lines<1>(input, tblboxes.second());
    }


//    void
//    connect_lines2(const util::array<int>& aligned_lines,
//		   util::array<box2d>& boxes,
//		   unsigned dim,
//		   unsigned dim_size)
//    {
//      image1d<int> l(dim_size);
//      data::fill(l, -1);
//
//      for_all_components(i, boxes)
//      {
//	opt::at(l, boxes[i].pmin()[dim]) = i;
//	opt::at(l, boxes[i].pmax()[dim]) = i;
//      }
//
//      for (unsigned i = 0; i < settings.max_dist_lines; ++i)
//	l = morpho::elementary::dilation(l, c2());
//
//      for_all_components(i, boxes)
//      {
//	util::couple<point2d, point2d> cp = central_sites(boxes[i], dim);
//
//	win::segment1d seg(11);
//	{
//	  mln_qiter_(win::segment1d) q(seg, point1d(cp.first()[dim]));
//	  for_all(q)
//	    if (opt::at(l, q[0]) != -1)
//	    {
//	      boxes[i].pmin()[dim] = boxes[opt::at(l, q[0])].pmin()[dim];
//	      break;
//	    }
//	}
//	{
//	  mln_qiter_(win::segment1d) q(seg, point1d(cp.second()[dim]));
//	  for_all(q)
//	    if (opt::at(l, q[0]) != -1)
//	    {
//	      boxes[i].pmax()[dim] = boxes[opt::at(l, q[0])].pmax()[dim];
//	      break;
//	    }
//	}
//      }
//    }


    /// Save lines bounding boxes in an image filled with \p bg_color.
    /// Bounding boxes are displayed with \p bbox_color.
    template <typename I>
    void
    save_table(const Image<I>& input,
	       util::couple<util::array<box<mln_site(I)> >,
			 util::array<box<mln_site(I)> > > tblboxes,
	       const std::string& filename,
	       const value::rgb8& bg_color = literal::black,
	       const value::rgb8& bbox_color = literal::red)
    {
      trace::entering("scribo::internal::save_table");
      mln_precondition(exact(input).is_valid());

      image2d<rgb8> out2(exact(input).domain());
      data::fill(out2, bg_color);
      for_all_components(i, tblboxes.first())
      {
	util::couple<mln_site(I), mln_site(I)> cp = central_sites(tblboxes.first()[i], 0);
	out2(cp.first()) = literal::green;
	out2(cp.second()) = literal::green;
	draw::box(out2, tblboxes.first()[i], bbox_color);
      }
      for_all_components(i, tblboxes.second())
      {
	util::couple<mln_site(I), mln_site(I)> cp = central_sites(tblboxes.second()[i], 1);
	out2(cp.first()) = literal::green;
	out2(cp.second()) = literal::green;
	draw::box(out2, tblboxes.second()[i], bbox_color);
      }
      io::ppm::save(out2, output_file(filename.c_str()));

      trace::exiting("scribo::internal::save_table");
    }

    /// Align line bboxes vertically and horizontally. Then, try to join
    /// vertical and horizontal lines in order to rebuild the table.
    template <typename I>
    mln_ch_value(I,bool)
    rebuild_table(const Image<I>& in_,
		  util::couple<util::array<box2d>,
			    util::array<box2d> >& tblboxes)
    {
      const I& in = exact(in_);

      mlc_equal(mln_value(I), bool)::check();
      mln_precondition(in.is_valid());

      std::cout << "Rebuild table" << std::endl;

      util::array<int> rows = align_lines_horizontaly(in, tblboxes.second());
      util::array<int> cols = align_lines_verticaly(in, tblboxes.first());

# ifndef NOUT
      save_table(in, tblboxes, "after-alignment.ppm");
# endif

//      repair_vertical_lines(in, tblboxes);
//      repair_horizontal_lines(in, tblboxes);

      // Connect vertical lines with horizontal lines.
      connect_vertical_lines(rows, tblboxes, in);
      connect_horizontal_lines(cols, tblboxes, in);

      image2d<bool> res;
      initialize(res, in);
      data::fill(res, false);
      for_all_components(i, tblboxes.first())
	draw::box(res, tblboxes.first()[i], true);
      for_all_components(i, tblboxes.second())
	draw::box(res, tblboxes.second()[i], true);

# ifndef NOUT
      save_table(in, tblboxes, "table.ppm");
# endif

      return res;
    }



    void draw_line(image2d<rgb8>& ima,
		   unsigned dim,
		   const box2d& box,
		   const rgb8& v)
    {
      util::couple<point2d, point2d> cp = central_sites(box, dim);

      draw::line(ima, cp.first(), cp.second(), v);
    }



    void draw_row(image2d<rgb8>& ima,
	      unsigned line,
	      const rgb8& v)
    {
      draw::line(ima, point2d(line, 0), point2d(line, ima.ncols()), v);
    }



    void draw_col(image2d<rgb8>& ima,
	      unsigned line,
	      const rgb8& v)
    {
      draw::line(ima, point2d(0, line), point2d(ima.nrows(), line), v);
    }

    /// \}
    //-****************************************************
    /// End of functions related to the table extraction
    //-****************************************************





    //-*****************************************
    /// Functions related to the table removal
    //-*****************************************
    /// \{



    /// Extract the components bboxes.
    util::array<box2d>
    component_boxes(const image2d<bool>& filter)
    {
      std::cout << "component boxes" << std::endl;
      label_16 nlabels = 0;
      image2d<label_16> lbl = labeling::blobs(filter, c8(), nlabels);

      return labeling::compute(accu::meta::bbox(), lbl, nlabels);
    }



    /// Remove table bboxes from an image.
    void erase_table_boxes(image2d<bool>& output,
		     util::array<box2d>& boxes,
		     unsigned dim)
    {
      for_all_components(i, boxes)
      {
	boxes[i].enlarge(dim, settings.bbox_enlarge);
	boxes[i].crop_wrt(output.domain());
	data::paste((pw::cst(false) | boxes[i] |
		(pw::value(output) == true)), output);
      }
    }



    /// Find table bboxes and remove them from the image.
    /// Use rank filter.
    ///
    /// \return pair of array of bounding boxes. The first array holds the
    ///		vertical lines bboxes and the second one the horizontal lines
    ///		bboxes.
    template <typename I, typename HW, typename VW>
    util::couple<util::array<box<mln_site(I)> >,
	      util::array<box<mln_site(I)> > >
    extract_table_lines_with_rank(const Image<I>& in,
				  const Window<HW>& vwin,
				  const Window<VW>& hwin,
				  unsigned rank_k)
    {
      typedef accu::bbox<mln_psite(I)> A;
      typedef util::array<mln_result(A)> boxes_t;

      // Vertical lines
      std::cout << "Removing vertical lines" << std::endl;
      mln_ch_value(I,bool) vfilter = morpho::rank_filter(in, vwin, rank_k);

#ifndef NOUT
      io::pbm::save(vfilter, output_file("vertical-erosion.pbm"));
#endif

      boxes_t vboxes = component_boxes(vfilter);

      // Horizontal lines.
      std::cout << "Removing horizontal lines" << std::endl;
      mln_ch_value(I,bool) hfilter = morpho::rank_filter(in, hwin, rank_k);

#ifndef NOUT
      io::pbm::save(hfilter, output_file("horizontal-erosion.pbm"));
#endif

      boxes_t hboxes = component_boxes(hfilter);

      return make::couple(vboxes, hboxes);
    }


    /// Erase table line bboxes from an image.
    ///
    /// \param[in]	line_bboxes   vertical and horizontal line bounding
    ///				      boxes.
    ///
    /// \param[in, out] in	      input image in which the lines are
    ///				      erased.
    template <typename I>
    void
    erase_table(util::couple<util::array<box2d>,
			     util::array<box2d> >& line_bboxes,
		Image<I>& in_)
    {
      trace::entering("scribo::internal::erase_table");
      I& in = exact(in_);
      mln_precondition(in.is_valid());

      erase_table_boxes(in, line_bboxes.first(), 0);
      erase_table_boxes(in, line_bboxes.second(), 1);

#ifndef NOUT
      mln_ch_value(I,rgb8) tmp = level::convert(rgb8(), in);
      draw_component_boxes(tmp, line_bboxes.first());
      draw_component_boxes(tmp, line_bboxes.second());
      io::ppm::save(tmp, output_file("vertical-and-horizontal-erosion.ppm"));
      io::pbm::save(in, output_file("table_removed.pbm"));
#endif

      trace::exiting("scribo::internal::erase_table");
    }


    /// \}
    //--------------------------------------------------
    /// End of functions related to the table removal.
    //--------------------------------------------------





    //-***************************************
    /// Function related to text extraction
    //-***************************************
    /// \{

    template <typename R>
    struct remove_small_comps
      : Function_l2b< remove_small_comps<R> >
    {
      remove_small_comps(const util::array<R>& nsitecomp)
	: nsitecomp_(nsitecomp)
      {
      }

      /// Return false if the components is smaller than a given size.
      bool operator()(const label_16& l) const
      {
	return nsitecomp_[l] >= settings.min_comp_size;
      }

      const util::array<R>& nsitecomp_;
    };



    template <typename R>
    struct remove_smallandlarge_comps
      : Function_l2b< remove_smallandlarge_comps<R> >
    {
      remove_smallandlarge_comps(const util::array<R>& nsitecomp)
	: nsitecomp_(nsitecomp)
      {
      }

      /// Return false if the components is smaller than a given size.
      bool operator()(const label_16& l) const
      {
	return nsitecomp_[l].first >= settings.min_comp_size
	    && nsitecomp_[l].first < settings.max_comp_size
	    && math::abs(nsitecomp_[l].second.pmax().row()
			  - nsitecomp_[l].second.pmin().row())
		<= settings.max_txt_box_height;
      }

      const util::array<R>& nsitecomp_;
    };



    void
    cleanup_components(image2d<label_16>& lbl,
		       label_16& nlabels)
    {
      std::cout << "Cleanup components..." << std::endl;

      typedef accu::count<mln_psite_(image2d<label_16>)> accu_count_t;
      typedef accu::bbox<mln_psite_(image2d<label_16>)> accu_bbox_t;
      typedef accu::pair<accu_count_t, accu_bbox_t> accu_pair_t;
      typedef mln_result_(accu_pair_t) accu_pair_res_t;
      typedef mln_result_(accu_count_t) accu_count_res_t;

      if (settings.treat_tables)
      {
	// Remove components which are too small
	typedef util::array<accu_count_res_t> nsitecomp_t;
	nsitecomp_t nsitecomp = labeling::compute(accu_count_t(), lbl, nlabels);
	remove_small_comps<accu_count_res_t> fl2b(nsitecomp);
	labeling::relabel_inplace(lbl, nlabels, fl2b);
      } else
      {
	// Remove components which have too much or not enough sites and which are
	// too heigh.
	typedef util::array<accu_pair_res_t> nsitecomp_t;
	nsitecomp_t nsitecomp = labeling::compute(accu_pair_t(), lbl, nlabels);
	remove_smallandlarge_comps<accu_pair_res_t> fl2b(nsitecomp);
	labeling::relabel_inplace(lbl, nlabels, fl2b);
      }
    }


    /// Functor to be passed to depth_first_search.
    /// Map each component vertex with its representative vertex id.
    struct make_relabel_fun_t
    {
      template <typename G>
      void init(const Graph<G>& g)
      {
	l2l.resize(exact(g).v_nmax(), mln_max(label_16));
	ncomp = 0;
      }

      void final()
      {}

      void next()
      { ++ncomp; }

      void update_treated(unsigned id)
      { l2l(id) = ncomp; }

      void update_queued(unsigned id)
      { update_treated(id); }

      bool to_be_treated(unsigned id)
      { return l2l(id) == mln_max(label_16); }

      bool to_be_queued(unsigned id)
      { return to_be_treated(id); }

      unsigned ncomp;
      fun::l2l::relabel<label_16> l2l;
    };



    /// Functor to be passed to depth_first_search.
    /// Computes the number of vertices per graph component.
    struct comp_size_t
    {
      template <typename G>
      void init(const Graph<G>& g)
      {
	treated.resize(exact(g).v_nmax(), mln_max(label_16));
      }

      void final()
      {}

      void next()
      {
	unsigned compsize = comp_vertices.nelements();
	std::cout << "compsize = " << compsize << std::endl;
	for (unsigned i = 0; i < comp_vertices.nelements(); ++i)
	  treated[comp_vertices[i]] = compsize;
	comp_vertices.clear();
      }

      void update_treated(unsigned id)
      { comp_vertices.insert(id); }

      void update_queued(unsigned id)
      {
	std::cout << "update_queued_before " << comp_vertices << std::endl;
	update_treated(id);
	std::cout << "update_queued_after " << comp_vertices << std::endl;
      }

      bool to_be_treated(unsigned id)
      { return treated[id] == mln_max(label_16); }

      bool to_be_queued(unsigned id)
      { return comp_vertices.has(id); }

      util::set<unsigned> comp_vertices;
      util::array<unsigned> treated;
    };

    unsigned
    find_root(util::array<unsigned>& parent, unsigned x)
    {
      if (parent[x] == x)
	return x;
      else
	return parent[x] = find_root(parent, parent[x]);
    }

    /// Merge bboxes according to their left neighbor.
    util::array<box2d>
    group_bboxes_with_single_link(util::array<unsigned>& link_array,
				  image2d<label_16>& lbl,
				  util::array<box2d>& cboxes, label_16& nlabels)
    {
      for (unsigned i = 0; i < link_array.nelements(); ++i)
	link_array[i] = find_root(link_array, i);

      util::array< accu::bbox<point2d> > tboxes;
      tboxes.resize(nlabels.next());
      for_all_ncomponents(i, nlabels)
	tboxes[link_array[i]].take(cboxes[i]);

      //Update labels
      labeling::relabel_inplace(lbl, nlabels,
				convert::to<fun::l2l::relabel<label_16> >(link_array));

#ifndef NOUT
      save_lbl_image(lbl, nlabels, "lbl-grouped-boxes.pgm");
#endif

      util::array<box2d> result;
      convert::from_to(tboxes, result);

      nlabels = result.nelements();

      return result;
    }


    /// Merge bboxes according to their neighbors.
    util::array<box2d>
    group_bboxes_with_graph(const util::graph& g, image2d<label_16>& lbl,
			    util::array<box2d>& cboxes, label_16& nlabels)
    {
      // Build relabel function.
      make_relabel_fun_t f;
      canvas::browsing::depth_first_search(g, f);

      util::array< accu::bbox<point2d> > tboxes;
      tboxes.resize(nlabels.next());
      for_all_ncomponents(i, nlabels)
	tboxes[f.l2l(i)].take(cboxes[i]);

      //Update labels
      labeling::relabel_inplace(lbl, nlabels, f.l2l);

#ifndef NOUT
      save_lbl_image(lbl, nlabels, "lbl-grouped-boxes.pgm");
#endif

      util::array<box2d> result(1);
      for_all_ncomponents(i, nlabels)
	if (tboxes[i].is_valid())
	  result.append(tboxes[i].to_result());

      mln_assertion(result.nelements() == f.ncomp);
      nlabels = result.nelements();


#ifndef NOUT
      image2d<label_16> lbl2 = duplicate(lbl);
      comp_size_t comp_size;
      canvas::browsing::depth_first_search(g, comp_size);

      std::cout << g << std::endl;
      for_all_ncomponents(i, nlabels)
	if (tboxes[i].is_valid())
	  if (comp_size.treated[i] < 3)
	    data::fill((lbl2 | (tboxes[i].to_result() | (pw::value(lbl2) == i))).rw(), 0u);
      save_lbl_image(lbl2, nlabels, "lbl-grouped-boxes-cleaned.ppm");
#endif

      return result;
    }



    /// Add an edge if a valid neighbor is found
    void update_link(util::graph& g, image2d<label_16>& lbl,
		     const point2d& p, const point2d& c,
		     unsigned i, int dmax)
    {
	if (lbl.domain().has(p) && lbl(p) != 0u && lbl(p) != i
	    && (math::abs(p.col() - c.col())) < dmax)
	  g.add_edge(lbl(p), i);
    }

    /// Update the lookup table \p link_array if a neighbor is found on the right of
    /// the current bbox.
    void update_link_array(util::array<unsigned>& link_array, image2d<label_16>& lbl,
			   const point2d& p, const point2d& c,
			   unsigned i, int dmax)
    {
	if (lbl.domain().has(p) && lbl(p) != 0u && lbl(p) != i
	    && (math::abs(p.col() - c.col())) < dmax && link_array[lbl(p)] == lbl(p))
	  link_array[lbl(p)] = i;
    }



    void init_link_array(util::array<unsigned>& link_array)
    {
      for (unsigned i = 0; i < link_array.nelements(); ++i)
	link_array[i] = i;
    }

    /// Map each character bbox to its left bbox neighbor if possible.
    /// Iterate to the right but link boxes to the left.
    ///
    /// \return an util::array. Map a bbox to its left neighbor.
    util::array<unsigned>
    link_character_bboxes_with_single_link(image2d<label_16>& lbl,
					   const util::array<box2d>& cboxes,
					   unsigned ncomp)
    {
      util::array<unsigned> left_link(ncomp + 1);
      init_link_array(left_link);

      for_all_ncomponents(i, ncomp)
      {
	unsigned midcol = (cboxes[i].pmax().col() - cboxes[i].pmin().col()) / 2;
	int dmax = midcol + settings.bbox_distance;
	point2d c = cboxes[i].center();

	///
	/// Find a neighbor on the right
	///

	/// First site on the right of the central site
	point2d p = c + right;

	// FIXME: Lemmings with a condition on the distance => write a special version?
	while (lbl.domain().has(p) && (lbl(p) == 0u || lbl(p) == i)
		&& math::abs(p.col() - c.col()) < dmax)
	  ++p.col();

	update_link_array(left_link, lbl, p, c, i, dmax);

      }

      return left_link;
    }


    /// Map each character bbox to its left bbox neighbor if possible.
    /// Iterate to the right but link boxes to the left.
    ///
    /// \return a pair of util::array. The first one map a bbox to its left
    ///		neighbor and the second one map a bbox to its right neighbor.
    util::couple<util::array<unsigned>, util::array<unsigned> >
    link_character_bboxes_with_double_link(image2d<label_16>& lbl,
					   const util::array<box2d>& cboxes,
					   unsigned ncomp)
    {
      util::array<unsigned> left_link(ncomp + 1), right_link(ncomp + 1);
      init_link_array(left_link);
      init_link_array(right_link);

      for_all_ncomponents(i, ncomp)
      {
	unsigned midcol = (cboxes[i].pmax().col() - cboxes[i].pmin().col()) / 2;
	int dmax = midcol + settings.bbox_distance;
	point2d c = cboxes[i].center();

	///
	/// Find a neighbor on the right
	///

	/// First site on the right of the central site
	point2d p = c + right;

	// FIXME: Lemmings with a condition on the distance => write a special version?
	while (lbl.domain().has(p) && (lbl(p) == 0u || lbl(p) == i)
		&& math::abs(p.col() - c.col()) < dmax)
	  ++p.col();

	update_link_array(left_link, lbl, p, c, i, dmax);


	///
	/// Find a neighbor on the left
	///

	/// First site on the left of the central site
	p = c + left;

	// FIXME: Lemmings with a condition on the distance => write a special version?
	while (lbl.domain().has(p) && (lbl(p) == 0u || lbl(p) == i)
		&& math::abs(p.col() - c.col()) < dmax)
	  --p.col();

	update_link_array(right_link, lbl, p, c, i, dmax);
      }

      return make::couple(left_link, right_link);
    }


    /// Map each character bbox to its left neighbors.
    util::graph
    link_character_bboxes_with_left_graph(image2d<label_16>& lbl,
					  const util::array<box2d>& cboxes,
					  unsigned ncomp)
    {
      util::graph g(ncomp + 1);

      for_all_ncomponents(i, ncomp)
      {
	unsigned midcol = (cboxes[i].pmax().col() - cboxes[i].pmin().col()) / 2;
	int dmax = midcol + settings.bbox_distance;
	point2d c = cboxes[i].center();
	/// First site on the right of the central site
	point2d p = c + right;

	// FIXME: Lemmings with a condition on the distance => write a special version?
	while (lbl.domain().has(p) && (lbl(p) == 0u || lbl(p) == i)
		&& math::abs(p.col() - c.col()) < dmax)
	  ++p.col();

	update_link(g, lbl, p, c, i, dmax);
      }

      return g;
    }


    /// Map each character bbox to its left and right neighbors.
    util::graph
    link_character_bboxes_with_left_and_right_graph(image2d<label_16>& lbl,
						    const util::array<box2d>& cboxes,
						    unsigned ncomp)
    {
      util::graph g(ncomp + 1);

      for_all_ncomponents(i, ncomp)
      {
	unsigned midcol = (cboxes[i].pmax().col() - cboxes[i].pmin().col()) / 2;
	int dmax = midcol + settings.bbox_distance;
	point2d c = cboxes[i].center();

	//
	// Find neighbors on the right
	//

	/// First site on the right of the central site
	point2d p = c + right;

	// FIXME: Lemmings with a condition on the distance => write a special version?
	while (lbl.domain().has(p) && (lbl(p) == 0u || lbl(p) == i)
		&& math::abs(p.col() - c.col()) < dmax)
	  ++p.col();

	update_link(g, lbl, p, c, i, dmax);

	//
	// Find neighbors on the left
	//

	/// First site on the right of the central site
	p = c + left;

	// FIXME: Lemmings with a condition on the distance => write a special version?
	while (lbl.domain().has(p) && (lbl(p) == 0u || lbl(p) == i)
		&& math::abs(p.col() - c.col()) < dmax)
	  --p.col();

	update_link(g, lbl, p, c, i, dmax);

      }

      return g;
    }



    util::array<box2d>
    extract_text(image2d<bool>& in,
		 image2d<label_16>& lbl,
		 label_16& nlabels)
    {
      std::cout << "extract text" << std::endl;

      typedef label_16 V;
      typedef image2d<V> I;
      typedef util::array<box2d> boxes_t;

      boxes_t cboxes = labeling::compute(accu::meta::bbox(), lbl, nlabels);

      image2d<label_16> lbl_bbox;
      initialize(lbl_bbox, lbl);
      data::fill(lbl_bbox, 0u);

      for_all_components(i, cboxes)
	draw::box(lbl_bbox, cboxes[i], i);

#ifndef NOUT
      image2d<rgb8> tmp = level::convert(rgb8(), in);
      draw_component_boxes(tmp, cboxes);
      io::ppm::save(tmp, output_file("character-bboxes.ppm"));
#endif

      //Link character bboxes to their left neighboor if possible.
      util::graph g = link_character_bboxes_with_left_graph(lbl_bbox, cboxes, nlabels);

      //Merge character bboxes through a graph.
      util::array<box2d> tboxes = group_bboxes_with_graph(g, lbl, cboxes, nlabels);

      return tboxes;
    }




    /// Function mapping value to sites of a line graph image.
    template <typename S>
    struct lg_vertex_values : public mln::Function_p2v< lg_vertex_values<S> >
    {
      /// Result is composed of a L2 distance between the two vertices of
      /// the edge, and the angle between the edge and the origin axis.
      typedef util::couple<unsigned,float> result;

      // Compute the angle between P and (0,1)
      util::couple<unsigned,float> operator()(const mln_psite(S)& p) const
      {
	unsigned distance = norm::l2_distance(p.to_site().begin().to_vec(),
					      p.to_site().end().to_vec());

	/// Compute angle between the edge and the axis.
	mln::algebra::vec<2,float> v, pv;
	v[0] = 1;
	v[1] = 0;
	pv = p.to_site().to_vec().normalize();

	float pi = 3.14;
	float pi_div2 = pi / 2;
	float angle = v * pv;
	// Be sure the angle is between 0 and pi/2

	// up left part of the circle.
	if (angle > pi_div2 && angle < pi)
	  angle = pi - angle;
	// down left part of the circle.
	else if (angle < 0 && angle < -pi_div2)
	  angle += pi;

	// down right part of the circle.
	if (angle < 0)
	  angle *= -1;

	return make::couple(distance, angle);
      }

    };



    void maptext_to_cells(const image2d<bool>& in, const image2d<bool>& table, const util::array<box2d>& tboxes)
    {
      std::cout << "map text to cells" << std::endl;
      label_16 nlabels;
      image2d<label_16> tblelbl = labeling::background(table, c8(), nlabels);
      image2d<rgb8> color = labeling::colorize(rgb8(), tblelbl, nlabels);
# ifndef NOUT
      io::ppm::save(color, output_file("cells-labels.ppm"));

      image2d<rgb8> dbg = level::convert(rgb8(), logical::not_(table));

      for_all_elements(i, tboxes)
	if (tboxes[i].is_valid())
	  data::paste(pw::cst(color(tboxes[i].center())) | (tboxes[i] | pw::value(in) == true),
		      dbg);
      io::ppm::save(dbg, output_file("text2cell.ppm"));
# endif
    }


    void merge_aligned_text_boxes(const image2d<bool>& in, util::array<box2d>& tboxes,
				  image2d<label_16>& lbl, label_16& nlabels)
    {
      std::cout << "Merging aligned text boxes" << std::endl;

      io::ppm::save(labeling::colorize(rgb8(), lbl, nlabels),
		    output_file("tboxes-lbl.ppm"));

      typedef util::couple<image2d<unsigned>, image2d<label_16> > cpl_t;
      cpl_t diz =
	transform::distance_and_influence_zone_geodesic(lbl, c8(),
							settings.bbox_distance);

#ifndef NOUT
      io::pgm::save(level::transform(diz.first(), fun::l2l::wrap<label_8>()),
		    output_file("tboxes-dmap.pgm"));
      io::ppm::save(labeling::colorize(rgb8(), diz.second(), nlabels),
		    output_file("tboxes-iz.ppm"));
#endif

      typedef util::graph G;
      G g = make::graph(diz.second() | (pw::value(diz.second()) != pw::cst(0u)),
			c8(), nlabels);

      // Compute the component centers and use them as vertex sites.
      //FIXME: Add fun::vertex2p
      typedef fun::i2v::array<point2d> fv2p_t;
      fv2p_t fv2p(nlabels.next());

      for_all_components(i, tboxes)
	fv2p(i) = tboxes[i].center();

      typedef util::line_graph<G> LG;
      LG lg(g);

      // Find lines (sites) associated to edges in g.
      typedef fun::i2v::array<p_line2d> i2e_t;
      util::array<p_line2d> lines;
      mln_edge_iter_(G) e(g);
      for_all(e)
	lines.append(p_line2d(fv2p(e.v1()), fv2p(e.v2())));

      // Map lines to vertices in lg.
      typedef p_vertices<LG, i2e_t> pvlg_t;
      pvlg_t pvlg(lg, convert::to<i2e_t>(lines));

      // Construct an image from a p_edges and a function mapping
      // lines to angles.
      typedef lg_vertex_values<pvlg_t> lgv2v_t;
      lgv2v_t lgv2v;

      mln_VAR(lg_ima, lgv2v | pvlg);

//      unsigned dmax = level::compute(accu::max<unsigned>(),
//				     thru(meta::first<mln_value_(lg_ima_t)>(),
//					  lg_ima));
//
//      mln_VAR(angle_ima, level::stretch(int_u8(),
//					thru(meta::second<mln_value_(lg_ima_t)>(),
//					     lg_ima)));
//
//      image2d<unsigned> stats(make::box2d(dmax, 255));
//      data::fill(stats, 0u);
//
//      mln_piter_(lg_ima_t) p(lg_ima.domain());
//      for_all(p)
//	++stats(point2d(lg_ima(p).first(), angle_ima(p)));

#ifndef NOUT
//      io::pgm::save(stats, "stats.pgm");
      {
	fun::i2v::array<value::rgb8> ecolor(pvlg.nsites(), literal::olive);
	mln_piter_(lg_ima_t) p(lg_ima.domain());
	for_all (p)
	{
	  mln_value_(lg_ima_t) v = lg_ima(p);
	  if ((v.second() > settings.max_cos) || (v.second() < - settings.max_cos))
	    ecolor(p) = literal::cyan;
	}

	image2d<rgb8> output = level::convert(rgb8(), in);
	internal::draw_component_boxes(output, tboxes);
	debug::draw_graph(output, pvlg, pw::cst(literal::cyan), ecolor);
	io::ppm::save(output, internal::output_file("aligned-bboxes-merged.ppm"));
      }
#endif



    }

    /// \}
    //-----------------------------------------------
    /// End of functions related to text extraction
    //-----------------------------------------------




  } // end of namespace scribo::internal





  // Facade

  void demat_table(char *argv[])
  {
    using namespace mln;
    using value::rgb8;
    using value::label_16;

    internal::settings.treat_tables = true;
    internal::input_file = basename(argv[1]);

    //Load image
    image2d<bool> in;
    io::pbm::load(in, argv[1]);
    logical::not_inplace(in);

#ifndef NOUT
    image2d<bool> in_bak = duplicate(in);
#endif

    internal::settings.max_comp_size = in.ncols() * in.nrows() * 0.05;

    // tblboxes.first() = vertical lines.
    // tblboxes.second() = horizontal lines.
    typedef util::couple<util::array<box2d>,util::array<box2d> > tblboxes_t;

    win::vline2d vline(internal::settings.ero_line_width);
    win::hline2d hline(internal::settings.ero_line_width);
    tblboxes_t tblboxes =
	internal::extract_table_lines_with_rank(in, vline, hline,
					        internal::settings.rank_filter);

    internal::erase_table(tblboxes, in);

    image2d<bool> table = internal::rebuild_table(in, tblboxes);

    /// relabel since the table has been removed.
    label_16 nlabels;
    image2d<label_16> lbl = labeling::blobs(in, c8(), nlabels);
    internal::cleanup_components(lbl, nlabels);

#ifndef NOUT
    internal::save_lbl_image(lbl, nlabels, "lbl-small-comps-removed.pgm");
#endif

    util::array<box2d> tboxes = internal::extract_text(in, lbl, nlabels);

    internal::merge_aligned_text_boxes(in, tboxes, lbl, nlabels);

    internal::maptext_to_cells(in, table, tboxes);

#ifndef NOUT
    std::cout << "Saving output" << std::endl;
    image2d<rgb8> output = level::convert(rgb8(), in_bak);
    internal::draw_component_boxes(output, tboxes);
    io::ppm::save(output, internal::output_file("out.ppm"));
#endif

//    internal::text_recognition(in, lbl, tboxes);
  }


  void demat_photo(char *argv[])
  {
    using namespace mln;
    using value::rgb8;
    using value::label_16;

    internal::settings.treat_tables = false;
    internal::input_file = basename(argv[1]);

    //Load image
    image2d<bool> in;
    io::pbm::load(in, argv[1]);
    logical::not_inplace(in);

#ifndef NOUT
    image2d<bool> in_bak = duplicate(in);
#endif

    internal::settings.max_comp_size = in.ncols() * in.nrows() * 0.05;

    //Label and remove small components.
    label_16 nlabels;
    image2d<label_16> lbl = labeling::blobs(in, c8(), nlabels);

    internal::cleanup_components(lbl, nlabels);

    util::array<box2d> tboxes = internal::extract_text(in, lbl, nlabels);
    internal::merge_aligned_text_boxes(in, tboxes, lbl, nlabels);

#ifndef NOUT
    std::cout << "Saving output" << std::endl;
    image2d<rgb8> output = level::convert(rgb8(), in_bak);
    internal::draw_component_boxes(output, tboxes);
    io::ppm::save(output, internal::output_file("out.ppm"));
#endif

    internal::text_recognition(in, lbl, tboxes);
  }

} // end of namespace scribo

# endif // ! DEMAT_HH
