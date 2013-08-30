// Copyright (C) 2011, 2012, 2013 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

/// \file
/// \brief Exercise a morpher recording every change in the morphed
/// image with the breadth-first thinning.
///
/// To produce an AVI movie from the `picasso-bft*.pbm' files, use:
///
///   for f in picasso-bft*pbm; convert $f -define png:bit-depth=8 -scale 2500% $(basename $f .pbm).png
///   mencoder "mf://picasso-bft*.png" -o picasso-bft.avi -ovc lavc -lavcopts gray:vcodec=mjpeg
///
/// Note: the `-define png:bit-depth=8' option prevents convert from
/// creating an image with 1-bit channels (since the image is binary)
/// and forces it to use 8-bit channels.  Indeed mencoder is unable to
/// process PNG images with 1-bit channel(s).
///
/// The output `picasso-bft.avi' can be embedded in a PDF file.  */

/* FIXME: Modernize this program as soon as patches from the
   mesh-segm-skel branch have been merged into the current branch.  */

#include <string>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/win/multiple_size.hh>
#include <mln/make/dual_neighb.hh>

#include <mln/data/compare.hh>

#include <mln/topo/skeleton/breadth_first_thinning.hh>

#include <mln/topo/detach_point.hh>

#include <mln/test/predicate.hh>
#include <mln/pw/value.hh>

#include <mln/labeling/colorize.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>

#include "apps/morphers/recorder.hh"
/* FIXME: Temporarily reuse an old file from apps/generic-skel instead
   of newer files integrated into the library (but not merged in).  */
#include "apps/morphers/image2d-skel.hh"

#include "apps/data.hh"


int main()
{
  using namespace mln;

  typedef image2d<bool> I;
  I picasso = io::pbm::load(MLN_IMG_DIR "/tiny.pbm");
  // Attach recorder to input image.
  typedef decorated_image< I, recorder<I> > J;
  J picasso_rec = record(picasso);

  // Dual neighborhood.
  typedef neighb< win::multiple_size<2, window2d, pw::value_<I> > > N;
  N nbh = make::dual_neighb(picasso, c4(), c8());
  // Simplicity criterion functor.
  ::is_simple_2d<J, N> is_simple(nbh);
  // Simple point detach procedure.
  topo::detach_point<J> detach;
  // Constraint.
  ::is_not_end_point<J, neighb2d> constraint(c4(), picasso_rec);

  J bft_rec = topo::skeleton::breadth_first_thinning(picasso_rec, nbh,
						     is_simple, detach,
						     constraint);

  // Dump recorded frames.
  /* FIXME: Hand-made (it partly duplicates ppm::save() (or more
     precisely pbm::save()) for decorated_image's).  We should be able
     to process sequences of images.  */
  bool output_initialized_p = false;
  for (size_t i = 0; i < bft_rec.decoration().sequence.size(); ++i)
    {
      I frame = bft_rec.decoration().sequence[i];

      // Skip frames until the inital image has been seen.
      if (frame == picasso)
	output_initialized_p = true;
      if (!output_initialized_p)
	continue;

      std::stringstream s;
      s << std::setfill ('0') << std::setw (6) << i;
      io::pbm::save(frame, std::string("picasso-bft") + s.str() + ".pbm");
    }
}
