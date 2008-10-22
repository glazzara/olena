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

# include <mln/binarization/threshold.hh>

# include <mln/border/fill.hh>

# include <mln/convert/to.hh>
# include <mln/convert/to_fun.hh>

# include <mln/debug/println.hh>

# include <mln/draw/box.hh>

# include <mln/estim/nsites.hh>

# include <mln/fun/i2v/array.hh>

# include <mln/io/pbm/load.hh>
# include <mln/io/pgm/load.hh>
# include <mln/io/pbm/save.hh>
# include <mln/io/pgm/save.hh>

# include <mln/labeling/blobs.hh>
# include <mln/labeling/compute.hh>

# include <mln/level/fill.hh>
# include <mln/level/paste.hh>

# include <mln/morpho/hit_or_miss.hh>
# include <mln/morpho/opening.hh>

# include <mln/pw/all.hh>

# include <mln/set/inter.hh>

# include <mln/util/array.hh>

# include <mln/value/int_u16.hh>
# include <mln/value/rgb8.hh>

namespace scribo
{

  namespace internal
  {

    using namespace mln;
    using value::int_u16;

    /// Functions related to the table removal
    /// \{


    /// Extract the components bboxes.
    util::array<box2d>
    component_boxes(const image2d<bool>& filter)
    {
      int_u16 nlabels;
      image2d<int_u16> lbl = labeling::blobs(filter, c4(), nlabels);

      return labeling::compute(accu::meta::bbox(), lbl, nlabels);
    }


    /// Remove bboxes from an image.
    void erase_boxes(image2d<bool>& ima,
		     const util::array<box2d>& boxes,
		     unsigned bbox_larger)
    {
      for (unsigned i = 1; i <= boxes.nelements(); ++i)
	level::paste((pw::cst(false)
		    | boxes[i].to_larger(bbox_larger)), ima);
    }


    /// Find table bboxes and remove them from the image.
    std::pair<util::array<box2d>,
		    util::array<box2d> >
    extract_tables(image2d<bool>& in, unsigned h, unsigned w, unsigned n)
    {
      typedef image2d<int_u16> I;
      typedef accu::bbox<mln_psite_(I)> A;
      typedef util::array<mln_result_(A)> boxes_t;

      // Lignes verticales
      win::rectangle2d vwin(h, w);
      image2d<bool> vfilter = morpho::opening(in, vwin);
      //io::pbm::save(vfilter, "./table-vfilter.pbm");
      boxes_t vboxes = component_boxes(vfilter);
      erase_boxes(in, vboxes, n);

      // Lignes horizontales
      win::rectangle2d hwin(w, h);
      image2d<bool> hfilter = morpho::opening(in, hwin);
      //io::pbm::save(hfilter, "./table-hfilter.pbm");
      boxes_t hboxes = component_boxes(hfilter);
      erase_boxes(in, hboxes, n);

      //io::pbm::save(in, "./table-filtered.pbm");

      return std::make_pair(vboxes, hboxes);
    }

    /// \}
    /// End of functions related to the table removal.


    /// Function related to text extraction
    /// \{

    int_u16
    anc(const fun::i2v::array<int_u16>& f, unsigned i)
    {
      while (f(i) != i)
	i = f(i);
      return i;
    }

    void do_curri(fun::i2v::array<int_u16>& f, int_u16 i)
    {
      if (f(i) != i)
	f(i) = anc(f, f(i));
    }


    void
    remove_small_comps_i2v(image2d<int_u16>& ima, image2d<int_u16>& lbl,
			   util::array<box2d>& cboxes, int_u16& ncomps,
			   unsigned min_comp_size)
    {
      ncomps = cboxes.nelements() - 1;
      unsigned current = 1;
      fun::i2v::array<int_u16> comps;
      comps.resize(cboxes.nelements());
      comps(0) = 0;

      // Construct the transform function.
      for (int i = 1; i < ncomps;)
      {
	// On aimerait avoir une routine qui nous le fait toute seule et qui
	// soit optimisee.
	if (estim::nsites(cboxes[i]) < min_comp_size)
	{
	  comps(current) = 0;

	  /// DEBUG
	  level::paste(pw::cst(false) | cboxes[i], ima);
	  /// DEBUG

	  cboxes[i] = cboxes[ncomps];
	  comps(ncomps) = i;
	  current = ncomps--;
	}
	else
	{
	  comps(current) = i;
	  current = ++i;
	  //draw::box(ima, cboxes[i], 150u);
	}
      }

      //Relabel
      lbl = level::transform(lbl, comps);
      cboxes.resize(ncomps + 1);
    }


    /// Merge bboxes according to their left box neighbor.
    util::array< accu::bbox<point2d> >
    group_bboxes(fun::i2v::array<int_u16>& left, image2d<int_u16>& lbl,
		 util::array<box2d>& cboxes, unsigned ncomp)
    {
      // Currify left lookup table and compute text area bboxes.
      util::array< accu::bbox<point2d> > tboxes;
      tboxes.resize(ncomp + 1);
      for (unsigned i = 1; i <= ncomp; ++i)
      {
	do_curri(left, i);
	tboxes[left(i)].take(cboxes[i]);
      }

      //Update labels
      level::transform(lbl, left);

      return tboxes;
    }

    bool
    has_valid_left_link(const fun::i2v::array<int_u16>& left, unsigned j)
    {
      return left(j) == j;
    }



    bool
    is_valid_comp_neigh(const point2d& p, const point2d& c, unsigned dmax)
    {
      return (p.col() - c.col()) < dmax;
    }



    /// Update the lookup table \p left if a neighbor is found on the right of
    /// the current bbox.
    void update_link(fun::i2v::array<int_u16>& left_link, image2d<int_u16>& lbl,
		     const point2d& p, const point2d& c,
		     unsigned i, unsigned dmax)
    {
	if (lbl.domain().has(p) && lbl(p) != 0
	    && is_valid_comp_neigh(p, c, dmax)
	    && has_valid_left_link(left_link, lbl(p)))
	  left_link(lbl(p)) = i;
	else if (!has_valid_left_link(left_link, lbl(p)) && lbl(p) != 0)
	  left_link(lbl(p)) = 0;
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
	/// Box center => Routine?
	point2d c (cboxes[i].pmin().row() + ((cboxes[i].pmax().row() - cboxes[i].pmin().row()) / 2),
		   cboxes[i].pmin().col() + midcol);
	/// First point on the right of c
	point2d p(c.row(), c.col() + 1);

	// Lemmings avec condition sur la distance en plus => Faire une version speciale?
	while (lbl.domain().has(p) && (lbl(p) == 0 || lbl(p) == i)
		&& is_valid_comp_neigh(p, c, dmax))
	  ++p.col();

	update_link(left_link, lbl, p, c, i, dmax);
      }

      return left_link;
    }



    void
    extract_text(image2d<bool>& in, unsigned bbox_distance, unsigned min_comp_size)
    {
      typedef int_u16 V;
      typedef image2d<V> I;
      typedef util::array<box2d> boxes_t;

      //  Find character bboxes.
      V nlabels;
      image2d<V> lbl = labeling::blobs(in, c4(), nlabels);
      boxes_t cboxes = labeling::compute(accu::meta::bbox(), lbl, nlabels);

      std::cout << "nlabels = " << nlabels << std::endl;

      //DEBUG PURPOSE
      //FIXME: don't know how to clone and convert to image<int> properly
      // \{
      I ima(in.domain());
      level::paste(in, ima);
      level::paste(pw::cst(100)
	  | (in | (pw::value(in) == pw::cst(true))).domain(), ima);
      // \}

      //Remove small components.
      int_u16 ncomp;
      remove_small_comps_i2v(ima, lbl, cboxes, ncomp, min_comp_size);

      std::cout << "ncomp = " << ncomp << std::endl;

      //Link character bboxes to their left neighboor if possible.
      fun::i2v::array<int_u16> left =
	link_character_bboxes(lbl, cboxes, ncomp, bbox_distance);

      //Merge character bboxes according to their left neighbor.
      util::array< accu::bbox<point2d> > tboxes = group_bboxes(left, lbl, cboxes, ncomp);

      /// DEBUG PURPOSE
      /// \{
      io::pgm::save(lbl, "./textlbl.pgm");
      io::pgm::save(ima, "./text.pgm");

      for (unsigned i = 1; i <= ncomp; ++i)
	if (tboxes[i].is_valid())
	  draw::box(ima, tboxes[i].to_result(), 254u);

      for (unsigned i = 1; i <= ncomp; ++i)
	if (tboxes[i].is_valid())
	  draw::box(in, tboxes[i].to_result(), true);

      io::pgm::save(ima, "./bbtext.pgm");
      io::pbm::save(in, "./intext.pgm");
      /// \}

    }

    /// \}
    /// End of functions related to text extraction

  } // end of namespace scribo::internal



  // Facade
  void demat(char *argv[])
  {
    using namespace mln;
    border::thickness = 0;

    //Useful debug variables
    unsigned h = atoi(argv[2]);
    unsigned w = atoi(argv[3]);
    unsigned bbox_larger = atoi(argv[4]);
    unsigned bbox_distance = atoi(argv[5]);
    unsigned min_comp_size = atoi(argv[6]);

    //Load image
    image2d<bool> in;
    io::pbm::load(in, argv[1]);

    internal::extract_tables(in, h, w, bbox_larger);

    internal::extract_text(in, bbox_distance, min_comp_size);
  }

} // end of namespace scribo

# endif // ! DEMAT_HH
