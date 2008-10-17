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

# include <mln/core/image/image_if.hh>
# include <mln/core/image/sub_image.hh>
# include <mln/core/image/cast_image.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/core/var.hh>
# include <mln/core/routine/clone.hh>
# include <mln/core/routine/ops.hh>
# include <mln/core/site_set/p_vaccess.hh>

# include <mln/accu/bbox.hh>

# include <mln/binarization/threshold.hh>

# include <mln/border/fill.hh>

# include <mln/convert/to.hh>
# include <mln/convert/to_fun.hh>

# include <mln/debug/println.hh>

# include <mln/draw/box.hh>

# include <mln/estim/nsites.hh>

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

# include <mln/util/array.hh>

# include <mln/value/int_u16.hh>
# include <mln/value/rgb8.hh>

namespace scribo
{

  namespace internal
  {

    using namespace mln;
    using value::int_u16;

    util::array<box2d>
    component_boxes(const image2d<bool>& filter)
    {

      int_u16 nlabels;
      image2d<int_u16> lbl = labeling::blobs(filter, c4(), nlabels);

      return labeling::compute(accu::meta::bbox(), lbl, nlabels);
    }

    void erase_boxes(image2d<bool>& ima,
		     const util::array<box2d>& boxes,
		     unsigned bbox_larger)
    {
      for (unsigned i = 1; i <= boxes.nelements(); ++i)
	level::paste(pw::cst(false)
		    | boxes[i].to_larger(bbox_larger),
		    ima);
    }


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
      io::pbm::save(vfilter, "./table-vfilter.pbm");
      boxes_t vboxes = component_boxes(vfilter);
      erase_boxes(in, vboxes, n);

      // Lignes horizontales
      win::rectangle2d hwin(w, h);
      image2d<bool> hfilter = morpho::opening(in, hwin);
      io::pbm::save(hfilter, "./table-hfilter.pbm");
      boxes_t hboxes = component_boxes(hfilter);
      erase_boxes(in, hboxes, n);

      return std::make_pair(vboxes, hboxes);
    }

    void
    extract_text(image2d<bool>& in)
    {
      typedef image2d<int_u16> I;
      typedef util::array<box2d> boxes_t;

      boxes_t tboxes = component_boxes(in);

      //FIXME: don't know how to clone and convert to image<int> properly
      // \{
      I ima(in.domain());
      level::paste(in, ima);
      level::paste(pw::cst(100)
	  | (in | (pw::value(in) == pw::cst(true))).domain(), ima);
      // \}

      for (int i = 1; i < tboxes.nelements(); ++i)
	if (estim::nsites(tboxes[i]) < 40)
	  level::paste(pw::cst(false) | tboxes[i], ima);
	else
	 draw::box(ima, tboxes[i], 150u);

      io::pgm::save(ima, "./text.pgm");
    }

  } // end of namespace scribo::internal



  // Facade
  void demat(char *argv[])
  {
    using namespace mln;
    border::thickness = 0;

    //Useful debug variables
    unsigned h = atoi(argv[2]);
    unsigned w = atoi(argv[3]);
    unsigned n = atoi(argv[4]);

    //Load image
    image2d<bool> in;
    io::pbm::load(in, argv[1]);

    internal::extract_tables(in, h, w, n);

    io::pbm::save(in, "./table-filtered.pbm");

    internal::extract_text(in);
  }

} // end of namespace scribo

# endif // ! DEMAT_HH
