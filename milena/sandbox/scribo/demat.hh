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

#ifndef DEMAT_HH_
# define DEMAT_HH_

# include <mln/core/image/image2d.hh>

# include <mln/core/concept/function.hh>
# include <mln/core/image/image_if.hh>
# include <mln/core/image/sub_image.hh>
# include <mln/core/image/cast_image.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/core/var.hh>
# include <mln/core/routine/clone.hh>
# include <mln/core/routine/ops.hh>
# include <mln/core/site_set/p_vaccess.hh>
# include <mln/core/site_set/p_set.hh>

# include <mln/accu/bbox.hh>
# include <mln/accu/count.hh>

# include <mln/border/fill.hh>

# include <mln/convert/to.hh>
# include <mln/convert/to_fun.hh>

# include <mln/debug/println.hh>

# include <mln/draw/box.hh>

# include <mln/fun/i2v/array.hh>

# include <mln/io/pbm/load.hh>
# include <mln/io/pbm/save.hh>
# include <mln/io/ppm/save.hh>
# include <mln/io/pgm/save.hh>

# include <mln/labeling/blobs.hh>
# include <mln/labeling/compute.hh>

# include <mln/level/convert.hh>
# include <mln/level/compute.hh>
# include <mln/level/fill.hh>
# include <mln/level/paste.hh>
# include <mln/level/apply.hh>
# include <mln/level/transform.hh>

# include <mln/literal/all.hh>

# include <mln/logical/not.hh>

# include <mln/morpho/hit_or_miss.hh>
# include <mln/morpho/erosion.hh>

# include <mln/pw/all.hh>

# include <mln/util/array.hh>

# include <mln/value/int_u16.hh>
# include <mln/value/rgb8.hh>

# include <mln/win/hline2d.hh>
# include <mln/win/vline2d.hh>

namespace scribo
{

  namespace internal
  {

    using namespace mln;
    using value::int_u16;
    using value::rgb8;


    void draw_component_boxes(image2d<rgb8>& output, const util::array<box2d>& boxes)
    {
      for (unsigned i = 1; i < boxes.nelements(); ++i)
        if (boxes[i].is_valid())
	  draw::box(output, boxes[i], literal::red);
    }

    template <typename V>
    void save_lbl_image(const image2d<V>& lbl, unsigned nlabels,
		        const char *filename)
    {
      image2d<rgb8> output(lbl.domain());

      fun::i2v::array<rgb8> f(nlabels + 1);
      f(0) = literal::black;
      for (unsigned i = 1; i <= nlabels; ++i)
	f(i) = rgb8(255 / ((i % 10) + 1), (100 + i) % 255, (255 + i)%255);
      output = level::transform(lbl, f);
      io::ppm::save(output, filename);
    }

    /// Functions related to the table removal
    /// \{


    /// Extract the components bboxes.
    util::array<box2d>
    component_boxes(const image2d<bool>& filter)
    {
      std::cout << "component boxes" << std::endl;
      int_u16 nlabels;
      image2d<int_u16> lbl = labeling::blobs(filter, c4(), nlabels);

      return labeling::compute(accu::meta::bbox(), lbl, nlabels);
    }

    /// Remove table bboxes from an image.
    void erase_table_boxes(image2d<bool>& output,
		     util::array<box2d>& boxes,
		     unsigned bbox_enlarge, unsigned dim)
    {
      for (unsigned i = 1; i < boxes.nelements(); ++i)
      {
	boxes[i].enlarge(dim, bbox_enlarge);
	level::paste((pw::cst(false)
		    | (boxes[i] | (pw::value(output) == pw::cst(true)))), output);
      }
    }


    /// Find table bboxes and remove them from the image.
    std::pair<util::array<box2d>,
		    util::array<box2d> >
    extract_tables(image2d<bool>& in,
		   image2d<rgb8>& output,
		   unsigned l)
    {
      typedef image2d<int_u16> I;
      typedef accu::bbox<mln_psite_(I)> A;
      typedef util::array<mln_result_(A)> boxes_t;


      // Lignes verticales
      std::cout << "Removing vertical lines" << std::endl;
      win::vline2d vline(l);
      image2d<bool> vfilter = morpho::erosion(in, vline);

#ifndef NOUT
      io::pbm::save(vfilter, "./table-vfilter.pbm");
#endif

      boxes_t vboxes = component_boxes(vfilter);
      erase_table_boxes(in, vboxes, (l / 2), 0);


      // Lignes horizontales
      std::cout << "Removing horizontal lines" << std::endl;
      win::hline2d hline(l);
      image2d<bool> hfilter = morpho::erosion(in, hline);

#ifndef NOUT
      io::pbm::save(hfilter, "./table-hfilter.pbm");
#endif

      boxes_t hboxes = component_boxes(hfilter);
      erase_table_boxes(in, hboxes, (l / 2), 1);


#ifndef NOUT
      image2d<rgb8> tmp = clone(output);
      draw_component_boxes(tmp, vboxes);
      draw_component_boxes(tmp, hboxes);
      io::ppm::save(tmp, "./table-filtered.ppm");
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
	left_link(i) = most_left(left_link, left_link(i));
      return left_link(i);
    }

    template <typename V>
    void
    remove_small_comps_i2v(image2d<V>& lbl,
			   V& nlabels,
			   unsigned min_comp_size)
    {
      std::cout << "Removing small components smaller than "
		<< min_comp_size << " sites among " << nlabels
		<< " components" << std::endl;

      typedef accu::count<mln_psite(image2d<V>)> accu_count_t;

      util::array<mln_result(accu_count_t)> nsitecomp
	    = labeling::compute(accu_count_t(), lbl, nlabels);

      V ncomp = 0;
      fun::i2v::array<V> f(nsitecomp.nelements());
      f(0) = 0;
      for (unsigned i = 1; i <= nlabels; ++i)
      {
	if (nsitecomp[i] < min_comp_size)
	  f(i) = 0;
	else
	  f(i) = ++ncomp;
      }

      level::apply(lbl, f);
      nlabels = ncomp;

#ifndef NOUT
      std::cout << "nlabels = " << nlabels << std::endl;
      save_lbl_image(lbl, nlabels, "./lbl-small-comps-removed.pgm");
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
      for (unsigned i = 1; i <= ncomp; ++i)
      {
	//if (left_link(i) != i)
	//  left_link(i) = most_left(left_link, left_link(i));
	tboxes[most_left(left_link, i)].take(cboxes[i]);
      }

      //Update labels
      for (unsigned i = 1; i <= ncomp; ++i)
	uncurri_left_link(left_link, i);
      level::apply(lbl, left_link);

#ifndef NOUT
      save_lbl_image(lbl, ncomp, "./lbl-grouped-boxes.pgm");
#endif

      util::array<box2d> result;
      for (unsigned i = 1; i <= ncomp; ++i)
	if (tboxes[i].is_valid())
	  result.append(tboxes[i].to_result());

      return result;
    }


    /// Update the lookup table \p left if a neighbor is found on the right of
    /// the current bbox.
    void update_link(fun::i2v::array<int_u16>& left_link, image2d<int_u16>& lbl,
		     const point2d& p, const point2d& c,
		     unsigned i, unsigned dmax)
    {
	if (lbl.domain().has(p) && lbl(p) != 0 && lbl(p) != i
	    && (p.col() - c.col()) < dmax)
	{
	  if (left_link(lbl(p)) == lbl(p) && most_left(left_link, i) != lbl(p))
	    left_link(lbl(p)) = i;
//	  else
//	    left_link(lbl(p)) = 0;//FIXME: should be uncommented?
	}
    }



    /// Map each character bbox to its left bbox neighbor if possible.
    /// Iterate to the right but link boxes to the left.
    fun::i2v::array<int_u16>
    link_character_bboxes(image2d<int_u16>& lbl,
			  const util::array<box2d>& cboxes,
			  unsigned ncomp,
			  unsigned bbox_distance)
    {
      fun::i2v::array<int_u16> left_link;
      left_link.resize(ncomp + 1);

      for (unsigned i = 0; i <= ncomp; ++i)
	left_link(i) = i;

      for (unsigned i = 1; i <= ncomp; ++i)
      {
	unsigned midcol = (cboxes[i].pmax().col() - cboxes[i].pmin().col()) / 2;
	unsigned dmax = midcol + bbox_distance;
	point2d c = cboxes[i].center();
	/// First site on the right of the central site
	point2d p(c.row(), c.col() + 1);

	// FIXME: Lemmings with a condition on the distance => write a special version?
	while (lbl.domain().has(p) && (lbl(p) == 0 || lbl(p) == i)
		&& (p.col() - c.col()) < dmax)
	  ++p.col();

	update_link(left_link, lbl, p, c, i, dmax);
      }

      return left_link;
    }

/*
    void merge_bboxes(util::array<box2d>& cboxes,
		      image2d<int_u16>& lbl, unsigned &nlabels)
    {
      fun::i2v::array<int_u16> merge;
      unsigned current = 1;

      for (unsigned i = 1; i <= nlabels;)
      {
	unsigned midcol = (cboxes[i].pmax().col() - cboxes[i].pmin().col()) / 2;
	point2d c = cboxes[i].center();
	/// First site on the right of the center site
	point2d p(c.row(), c.col() + 1);

	// FIXME: Lemmings with a condition on the distance => write a special version?
	while (lbl.domain().has(p) && (lbl(p) == 0 || lbl(p) == i)
		&& (p.col() - c.col()) <= midcol)
	  ++p.col();

	if (lbl.domain().has(p) && lbl(p) != 0 && lbl(p) != i
	    && (p.col() - c.col()) <= midcol)
	{

	}
      }

      level::apply(lbl, merge);
      cboxes.resize(nlabels);
    }
*/

    util::array<box2d>
    extract_text(image2d<bool>& in,
		 image2d<rgb8>& output,
		 unsigned bbox_distance,
		 unsigned min_comp_size)
    {
      std::cout << "extracting text..." << std::endl;

      typedef int_u16 V;
      typedef image2d<V> I;
      typedef util::array<box2d> boxes_t;

      //  Find character bboxes.
      V nlabels;
      image2d<V> lbl = labeling::blobs(in, c4(), nlabels);

      //Remove small components.
      remove_small_comps_i2v(lbl, nlabels, min_comp_size);

      boxes_t cboxes = labeling::compute(accu::meta::bbox(), lbl, nlabels);

#ifndef NOUT
      image2d<rgb8> tmp = clone(output);
      draw_component_boxes(tmp, cboxes);
      io::ppm::save(tmp, "character-bboxes.ppm");
#endif

      //merge_bboxes(cboxes, lbl, nlabels);

      //Link character bboxes to their left neighboor if possible.
      fun::i2v::array<int_u16> left =
	link_character_bboxes(lbl, cboxes, nlabels, bbox_distance);

      //Merge character bboxes according to their left neighbor.
      util::array<box2d> tboxes = group_bboxes(left, lbl, cboxes, nlabels);

      return tboxes;
    }

    /// \}
    /// End of functions related to text extraction

  } // end of namespace scribo::internal


  // Facade
  void demat(char *argv[])
  {
    using namespace mln;
    using value::rgb8;

    border::thickness = 3;
    trace::quiet = false;

    //Useful debug variables
    unsigned l = atoi(argv[3]);
    unsigned bbox_distance = atoi(argv[4]);
    unsigned min_comp_size = atoi(argv[5]);

    //Load image
    image2d<bool> in;
    io::pbm::load(in, argv[1]);
    logical::not_inplace(in);

#ifndef NOUT
    io::pbm::save(in, "in-neg.pbm");
#endif

    image2d<rgb8> output = level::convert(rgb8(), in);

    internal::extract_tables(in, output, l);

    util::array<box2d> tboxes =
      internal::extract_text(in, output, bbox_distance, min_comp_size);

    internal::draw_component_boxes(output, tboxes);
    io::ppm::save(output, argv[2]);

    /// Use txt bboxes here with Tesseract
    /// for (i = 1; i < tboxes.nelements(); ++i)
    ///	  tesseract(in | tboxes[i])
  }

} // end of namespace scribo

# endif // ! DEMAT_HH
