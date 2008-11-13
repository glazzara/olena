// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

# include <mln/essential/1d.hh>
# include <mln/essential/2d.hh>
# include <mln/morpho/elementary/dilation.hh>
# include <mln/labeling/background.hh>

# include <mln/transform/influence_zone_geodesic.hh>

#include <tesseract/baseapi.h>

namespace scribo
{

  namespace internal
  {

    using namespace mln;
    using value::int_u16;
    using value::rgb8;

    struct settings_t
    {
      settings_t()
      {
	bbox_enlarge = 1;
	ero_line_width = 101;
	bbox_distance = 25;
	min_comp_size = 5;
      }

      unsigned bbox_enlarge;
      unsigned ero_line_width;
      unsigned bbox_distance;
      unsigned min_comp_size;
    };

    settings_t settings;
    char *input_file = 0;

    /// Returns a formated output file name.
    std::string output_file(const char *name)
    {
      static int file_id = 1;

      std::ostringstream os;
      os << "./"
	 << file_id++
	 << "_"
	 << input_file
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
    std::pair<point2d, point2d>
    central_sites(const box2d& b, unsigned dim)
    {
      unsigned n = b.pmax()[dim] - b.pmin()[dim];

      point2d p1 = b.center();
      p1[dim] -= n / 2;
      point2d p2 = b.center();
      p2[dim] += n / 2;

      return std::make_pair(p1, p2);
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
      image2d<rgb8> output = debug::colorize<image2d<rgb8>, image2d<V> >(lbl, nlabels);
      io::ppm::save(output, output_file(filename));
    }


    /// Functions related to Text Recognition.
    /// \{

    /// Passes the text bboxes to Tesseract and store the result an image of
    /// char.
    /// \param[in] in image from where the text bboxes are extracted.
    /// \param[in] lbl labeled image.
    /// \param[in] tboxes array of text bboxes.
    ///
    /// FIXME: For each text bbox, we create a new image. We may like to avoid that.
    void
    text_recognition(const image2d<bool>& in, const image2d<int_u16>& lbl,
		     const util::array<box2d>& tboxes)
    {
      /// Use txt bboxes here with Tesseract
      std::cout << "Text recognition..." << std::endl;

      TessBaseAPI::InitWithLanguage(NULL, NULL, "fra", NULL, false, 0, NULL);
      image2d<char> txt(in.domain());
      level::fill(txt, ' ');

      for_all_components(i, tboxes)
      {
	if (tboxes[i].is_valid())
	{
	  image2d<bool> b(tboxes[i], 0);
	  level::fill(b, false);
	  level::fill(b, in | (tboxes[i] | (pw::value(lbl) == pw::cst(i))));

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

    /// Functions related to the matrix extraction
    /// \{

    /// Align table lines bboxes according to a given dimension.
    /*
    **
    ** FIXME: DOC!
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
    */
    util::array<int>
    align_lines(unsigned nsites,
		int min_coord,
		int max_coord,
		util::array<box2d>& hboxes,
		unsigned dim)
    {
      std::cout << "extracting table lines..." << std::endl;
      util::array< util::set<unsigned> > lines;
      lines.resize(nsites);

      // Map components with actual lines.
      for_all_components(i, hboxes)
      {
	int minline = hboxes[i].pmin()[dim] - 5;
	minline = (minline < min_coord ? min_coord : minline);
	int maxline = hboxes[i].pmax()[dim] + 5;
	maxline = (maxline > max_coord ? max_coord : maxline);

	for (int line = minline;
	      line <= maxline; ++line)
	  lines[line].insert(i);
      }

      // Init box2line
      util::array<int> box2line;
      box2line.resize(hboxes.nelements());
      for_all_elements(i, box2line)
	box2line[i] = -1;

      // Find the line with the highest element count.
      unsigned max_nelts = 0;
      for_all_elements(i, lines)
	if (max_nelts < lines[i].nelements())
	  max_nelts = lines[i].nelements();

      // Aligning lines
      util::array<int> newlines;
      while (max_nelts > 0)
      {
	for_all_elements(i, lines)
	  if (lines[i].nelements() == max_nelts)
	  {
	    accu::mean<int_u16> mean;
	    for_all_elements(j, lines[i])
	      if (box2line[lines[i][j]] == -1)
		mean.take(hboxes[lines[i][j]].center()[dim]);

	    if (mean.is_valid())
	    {
	      for_all_elements(j, lines[i])
		if (box2line[lines[i][j]] == -1)
		{
		  hboxes[lines[i][j]].pmin()[dim] = mean.to_result();
		  hboxes[lines[i][j]].pmax()[dim] = mean.to_result();
		  box2line[lines[i][j]] = mean.to_result();
		}
	      newlines.append(mean.to_result());
	    }
	  }
	--max_nelts;
      }

      return newlines;
    }

    image2d<bool>
    rebuild_table(const image2d<bool>& in,
		  std::pair<util::array<box2d>,
		  util::array<box2d> >& tblboxes)
    {
      util::array<int> rows = align_lines(in.nrows(), geom::min_row(in),
					  geom::max_row(in), tblboxes.second,
					  0);
      util::array<int> cols = align_lines(in.ncols(), geom::min_col(in),
					  geom::max_col(in), tblboxes.first,
					  1);

      image1d<int> v(in.nrows());
      image1d<int> h(in.ncols());
      level::fill(v, -1);
      level::fill(h, -1);

      for_all_elements(i, rows)
	v.at(rows[i]) = i;
      for_all_elements(i, cols)
	h.at(cols[i]) = i;

      for (int i = 0; i < 10; ++i)
      {
	v = morpho::elementary::dilation(v, c2());
	h = morpho::elementary::dilation(h, c2());
      }

      util::array<box2d>& vb = tblboxes.first;
      for_all_components(i, vb)
      {
	std::pair<point2d, point2d> cp = central_sites(vb[i], 0);
	if (v.at(cp.first.row()) != -1)
	  vb[i].pmin().row() = rows[v.at(cp.first.row())];
	if (v.at(cp.second.row()) != -1)
	  vb[i].pmax().row() = rows[v.at(cp.second.row())];

	cp = central_sites(vb[i], 0);
      }

      util::array<box2d>& hb = tblboxes.second;
      for_all_components(i, hb)
      {
	std::pair<point2d, point2d> cp = central_sites(hb[i], 1);
	if (h.at(cp.first.col()) != -1)
	  hb[i].pmin().col() = cols[h.at(cp.first.col())];
	if (h.at(cp.second.col()) != -1)
	  hb[i].pmax().col() = cols[h.at(cp.second.col())];
      }

      image2d<bool> res;
      initialize(res, in);
      level::fill(res, false);
      for_all_components(i, tblboxes.first)
	draw::box(res, tblboxes.first[i], true);
      for_all_components(i, tblboxes.second)
	draw::box(res, tblboxes.second[i], true);

# ifndef NOUT
      image2d<rgb8> out(in.domain());
      level::fill(out, literal::black);
      for_all_components(i, tblboxes.first)
	draw::box(out, tblboxes.first[i], literal::red);
      for_all_components(i, tblboxes.second)
	draw::box(out, tblboxes.second[i], literal::red);
      io::ppm::save(out, output_file("table.ppm"));
# endif
      return res;
    }


    void draw_line(image2d<rgb8>& ima,
		   unsigned dim,
		   const box2d& box,
		   const rgb8& v)
    {
      std::pair<point2d, point2d> cp = central_sites(box, dim);

      draw::line(ima, cp.first, cp.second, v);
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

    void
    extract_matrix(const image2d<bool>& in,
		   std::pair<util::array<box2d>, util::array<box2d> > tboxes)
    {
      std::cout << "Extracting matrix..." << std::endl;

      image1d<unsigned> hend(in.ncols()),
			hrow(in.nrows()),
			vend(in.nrows()),
			vcol(in.ncols());

      level::fill(hend, 0);
      level::fill(hrow, 0);
      level::fill(vend, 0);
      level::fill(vcol, 0);

      for_all_components(i, tboxes.first)
      {
	++vend.at(tboxes.first[i].pmin().row());
	++vend.at(tboxes.first[i].pmax().row());
	++vcol.at(tboxes.first[i].center().col());
      }

      for_all_components(i, tboxes.second)
      {
	++hend.at(tboxes.second[i].pmin().col());
	++hend.at(tboxes.second[i].pmax().col());
	++hrow.at(tboxes.second[i].center().row());
      }

#ifndef NOUT
      image2d<rgb8> tmp(in.domain());
      level::fill(tmp, literal::black);

      for (unsigned i = 1; i < in.ncols(); ++i)
      {
	if (hend.at(i) > 0)
	  draw_col(tmp, i, literal::orange);
	if (vcol.at(i) > 0)
	  draw_col(tmp, i, literal::orange);
      }

      for (unsigned i = 1; i < in.nrows(); ++i)
      {
	if (hrow.at(i) > 0)
	  draw_row(tmp, i, literal::magenta);
	if (vend.at(i) > 0)
	  draw_row(tmp, i, literal::magenta);
      }

      for_all_components(i, tboxes.first)
	draw_line(tmp, 0, tboxes.first[i], literal::green);

      for_all_components(i, tboxes.second)
	draw_line(tmp, 1, tboxes.second[i], literal::red);

      io::ppm::save(tmp, output_file("matrix.ppm"));
#endif

    }

    /// \}



    /// Functions related to the table removal
    /// \{


    /// Extract the components bboxes.
    util::array<box2d>
    component_boxes(const image2d<bool>& filter)
    {
      std::cout << "component boxes" << std::endl;
      int_u16 nlabels = 0;
      image2d<int_u16> lbl = labeling::blobs(filter, c8(), nlabels);

      return labeling::compute(accu::meta::bbox(), lbl, nlabels);
    }

    /// Remove table bboxes from an image.
    void erase_table_boxes(image2d<bool>& output,
		     util::array<box2d>& boxes,
		     unsigned bbox_enlarge, unsigned dim)
    {
      for_all_components(i, boxes)
      {
	boxes[i].enlarge(dim, bbox_enlarge + settings.bbox_enlarge);
	boxes[i].crop_wrt(output.domain());
	level::paste((pw::cst(false) | boxes[i] |
		(pw::value(output) == pw::cst(true))), output);
      }
    }


    /// Find table bboxes and remove them from the image.
    std::pair<util::array<box2d>,
		    util::array<box2d> >
    extract_tables(image2d<bool>& in,
		   image2d<rgb8>& output)
    {
      typedef image2d<int_u16> I;
      typedef accu::bbox<mln_psite_(I)> A;
      typedef util::array<mln_result_(A)> boxes_t;


      // Lignes verticales
      std::cout << "Removing vertical lines" << std::endl;
      win::vline2d vline(settings.ero_line_width);
      image2d<bool> vfilter = morpho::erosion(in, vline);

#ifndef NOUT
      io::pbm::save(vfilter, output_file("vertical-erosion.pbm"));
#endif

      boxes_t vboxes = component_boxes(vfilter);
      erase_table_boxes(in, vboxes, (settings.ero_line_width / 2), 0);

      // Lignes horizontales
      std::cout << "Removing horizontal lines" << std::endl;
      win::hline2d hline(settings.ero_line_width);
      image2d<bool> hfilter = morpho::erosion(in, hline);

#ifndef NOUT
      io::pbm::save(hfilter, output_file("horizontal-erosion.pbm"));
#endif

      boxes_t hboxes = component_boxes(hfilter);
      erase_table_boxes(in, hboxes, (settings.ero_line_width / 2), 1);


#ifndef NOUT
      image2d<rgb8> tmp = clone(output);
      draw_component_boxes(tmp, vboxes);
      draw_component_boxes(tmp, hboxes);
      io::ppm::save(tmp, output_file("vertical-and-horizontal-erosion.ppm"));
#endif

      return std::make_pair(vboxes, hboxes);
    }

    /// \}
    /// End of functions related to the table removal.


    /// Function related to text extraction
    /// \{

    inline
    int_u16
    most_left(const fun::i2v::array<int_u16>& left_link, unsigned i)
    {
      while (left_link(i) != i)
	i = left_link(i);
      return i;
    }

    inline
    int_u16
    uncurri_left_link(fun::i2v::array<int_u16>& left_link, unsigned i)
    {
      if (left_link(i) != i)
	left_link(i) = uncurri_left_link(left_link, left_link(i));
      return left_link(i);
    }

    template <typename V>
    void
    remove_small_comps_i2v(image2d<V>& lbl,
			   V& nlabels)
    {
      std::cout << "Removing small components smaller than "
		<< settings.min_comp_size << " sites among " << nlabels
		<< " components" << std::endl;

      typedef accu::count<mln_psite(image2d<V>)> accu_count_t;

      util::array<mln_result(accu_count_t)> nsitecomp
	    = labeling::compute(accu_count_t(), lbl, nlabels);

      V ncomp = 0;

      fun::i2v::array<V> f(nsitecomp.nelements());
      f(0) = 0;

      for_all_ncomponents(i, nlabels)
      {
	if (nsitecomp[i] < settings.min_comp_size)
	  f(i) = 0;
	else
	  f(i) = ++ncomp;
      }

      lbl = level::transform(lbl, f);
      nlabels = ncomp;

#ifndef NOUT
      save_lbl_image(lbl, nlabels, "lbl-small-comps-removed.pgm");
#endif
    }


    /// Merge bboxes according to their left box neighbor.
    util::array< box2d >
    group_bboxes(fun::i2v::array<int_u16>& left_link, image2d<int_u16>& lbl,
		 util::array<box2d>& cboxes, unsigned ncomp)
    {
      // Currify left_link lookup table and compute text area bboxes.
      util::array< accu::bbox<point2d> > tboxes;
      tboxes.resize(ncomp + 1);
      for_all_ncomponents(i, ncomp)
	tboxes[uncurri_left_link(left_link, i)].take(cboxes[i]);

      //Update labels
      lbl = level::transform(lbl, left_link);

#ifndef NOUT
      save_lbl_image(lbl, ncomp, "lbl-grouped-boxes.pgm");
#endif

      util::array<box2d> result;
      for_all_ncomponents(i, ncomp)
	if (tboxes[i].is_valid())
	  result.append(tboxes[i].to_result());

      return result;
    }


    /// Update the lookup table \p left if a neighbor is found on the right of
    /// the current bbox.
    void update_link(fun::i2v::array<int_u16>& left_link, image2d<int_u16>& lbl,
		     const point2d& p, const point2d& c,
		     unsigned i, int dmax)
    {
	if (lbl.domain().has(p) && lbl(p) != 0u && lbl(p) != i
	    && (math::abs(p.col() - c.col())) < dmax)
	{
	  if (left_link(lbl(p)) == lbl(p) && most_left(left_link, i) != lbl(p))
	    left_link(lbl(p)) = i;
//	  else
//	    left_link(lbl(p)) = 0;//FIXME: should be uncommented? VERY bad results otherwise.
	}
    }



    /// Map each character bbox to its left bbox neighbor if possible.
    /// Iterate to the right but link boxes to the left.
    fun::i2v::array<int_u16>
    link_character_bboxes(image2d<int_u16>& lbl,
			  const util::array<box2d>& cboxes,
			  unsigned ncomp)
    {
      fun::i2v::array<int_u16> left_link;
      left_link.resize(ncomp + 1);

      for (unsigned i = 0; i <= ncomp; ++i)
	left_link(i) = i;

      for_all_ncomponents(i, ncomp)
      {
	unsigned midcol = (cboxes[i].pmax().col() - cboxes[i].pmin().col()) / 2;
	int dmax = midcol + settings.bbox_distance;
	point2d c = cboxes[i].center();
	/// First site on the right of the central site
	point2d p(c.row(), c.col() + 1);

	// FIXME: Lemmings with a condition on the distance => write a special version?
	while (lbl.domain().has(p) && (lbl(p) == 0u || lbl(p) == i)
		&& math::abs(p.col() - c.col()) < dmax)
	  ++p.col();

	update_link(left_link, lbl, p, c, i, dmax);
      }

      return left_link;
    }

    util::array<box2d>
    extract_text(image2d<bool>& in,
		 image2d<int_u16>& lbl,
		 image2d<rgb8>& output)
    {

      typedef int_u16 V;
      typedef image2d<V> I;
      typedef util::array<box2d> boxes_t;

      //  Find character bboxes.
      V nlabels;
      lbl = labeling::blobs(in, c8(), nlabels);

      //Remove small components.
      remove_small_comps_i2v(lbl, nlabels);

      boxes_t cboxes = labeling::compute(accu::meta::bbox(), lbl, nlabels);

#ifndef NOUT
      image2d<rgb8> tmp = clone(output);
      draw_component_boxes(tmp, cboxes);
      io::ppm::save(tmp, output_file("character-bboxes.ppm"));
#endif

      //merge_bboxes(cboxes, lbl, nlabels);

      //Link character bboxes to their left neighboor if possible.
      fun::i2v::array<int_u16> left =
	link_character_bboxes(lbl, cboxes, nlabels);

      //Merge character bboxes according to their left neighbor.
      util::array<box2d> tboxes = group_bboxes(left, lbl, cboxes, nlabels);

      return tboxes;
    }

    void maptext_to_cells(const image2d<bool>& in, const image2d<bool>& table, const util::array<box2d>& tboxes)
    {
      int_u16 nlabels;
      image2d<int_u16> tblelbl = labeling::background(table, c8(), nlabels);
      image2d<rgb8> color = debug::colorize<image2d<rgb8>, image2d<int_u16> >(tblelbl, nlabels);
# ifndef NOUT
      io::ppm::save(color, output_file("cells-labels.ppm"));
# endif

      image2d<rgb8> dbg = level::convert(rgb8(), logical::not_(table));

      for_all_elements(i, tboxes)
	if (tboxes[i].is_valid())
	{
	  level::paste(pw::cst(color(tboxes[i].center())) | (tboxes[i] | pw::value(in) == pw::cst(true)),
		dbg);
	}
# ifndef NOUT
      io::ppm::save(dbg, output_file("text2cell.ppm"));
# endif
    }

    /// \}
    /// End of functions related to text extraction

  } // end of namespace scribo::internal

  // Facade
  void demat(char *argv[], bool treat_tables)
  {
    using namespace mln;
    using value::rgb8;
    using value::int_u16;

    border::thickness = 3;
    trace::quiet = true;

    //Useful debug variables
    internal::input_file = basename(argv[1]);

    //Load image
    image2d<bool> in;
    io::pbm::load(in, argv[1]);
    logical::not_inplace(in);

    image2d<rgb8> output = level::convert(rgb8(), in);

    std::pair<util::array<box2d>,
		    util::array<box2d> > tblboxes;

    image2d<bool> table;
    if (treat_tables)
    {
      tblboxes = internal::extract_tables(in, output);
//      internal::extract_matrix(in, tblboxes);
      table = internal::rebuild_table(in, tblboxes);
    }

    image2d<value::int_u16> lbl;
    util::array<box2d> tboxes =
      internal::extract_text(in, lbl, output);

    if (treat_tables)
      internal::maptext_to_cells(in, table, tboxes);

    internal::draw_component_boxes(output, tboxes);
    io::ppm::save(output, internal::output_file("out.ppm"));

    internal::text_recognition(in, lbl, tboxes);
  }

} // end of namespace scribo

# endif // ! DEMAT_HH
