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
# include <mln/core/alias/neighb2d.hh>
# include <mln/core/var.hh>
# include <mln/core/site_set/p_vaccess.hh>

# include <mln/binarization/threshold.hh>
# include <mln/morpho/hit_or_miss.hh>
# include <mln/level/fill.hh>
# include <mln/border/fill.hh>
# include <mln/io/pbm/load.hh>
# include <mln/io/pgm/load.hh>
# include <mln/io/pbm/save.hh>
# include <mln/io/pgm/save.hh>
# include <mln/debug/println.hh>
# include <mln/morpho/opening.hh>
# include <mln/trait/value_.hh>
# include <mln/value/int_u8.hh>
# include <mln/level/paste.hh>
# include <mln/labeling/blobs.hh>
# include <mln/level/fill.hh>
# include <mln/pw/all.hh>
# include <mln/convert/to_fun.hh>
# include <mln/geom/bbox.hh>

# include <mln/labeling/compute.hh>
# include <mln/accu/bbox.hh>

namespace scribo
{

  namespace internal
  {

    void filter_image(mln::image2d<bool>& ima,
		      const mln::image2d<bool>& filter,
		      unsigned bbox_larger)
    {
      using namespace mln;
      using value::int_u8;

      typedef image2d<int_u8> I;
      typedef mln_accu_with_(accu::meta::bbox, mln_psite_(I)) A;
      typedef p_array<mlc_unqualif_(A::result)> boxes_t;

      int_u8 nlabels;
      I lbl = labeling::blobs(filter, c4(), nlabels);

      boxes_t boxes = labeling::compute(accu::meta::bbox(), lbl, nlabels);
      mln_piter_(boxes_t) p(boxes);

      for_all(p)
	level::paste(pw::cst(false)
	    | p.to_site().to_larger(bbox_larger),
	    ima);
    }

    void remove_tables(mln::image2d<bool>& in, unsigned h, unsigned w, unsigned n)
    {
      using namespace mln;

      // Lignes verticales
      win::rectangle2d vwin(h, w);
      image2d<bool> vfilter = morpho::opening(in, vwin);
      io::pbm::save(vfilter, "./table-vfilter.pbm");
      filter_image(in, vfilter, n);

      // Lignes horizontales
      win::rectangle2d hwin(w, h);
      image2d<bool> hfilter = morpho::opening(in, hwin);
      io::pbm::save(hfilter, "./table-hfilter.pbm");
      filter_image(in, hfilter, n);
    }

  } // end of namespace scribo::internal



  // Facade
  void demat(char *argv[])
  {
    using namespace mln;
    using value::int_u8;

    //Useful debug variables
    unsigned h = atoi(argv[2]);
    unsigned w = atoi(argv[3]);
    unsigned n = atoi(argv[4]);

    //Load image
    image2d<bool> in;
    io::pbm::load(in, argv[1]);

    internal::remove_tables(in, h, w, n);

    io::pbm::save(in, "./table-filtered.pbm");
  }

} // end of namespace scribo

# endif // ! DEMAT_HH
