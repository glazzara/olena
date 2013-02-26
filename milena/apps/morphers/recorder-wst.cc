// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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
/// image with the watershed transform.
///
/// To produce an AVI movie from the `lena-wst*.ppm' files, use:
///
///   for f in lena-wst*ppm; convert $f -scale 2500% $(basename $f .ppm).png
///   mencoder "mf://lena-wst*.png" -o lena-wst.avi -ovc lavc -lavcopts vcodec=mjpeg
///
/// The output `lena-wst.avi' can be embedded in a PDF file.  */

#include <string>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>
#include <mln/value/rgb8.hh>
#include <mln/literal/colors.hh>

#include <mln/morpho/gradient.hh>
#include <mln/morpho/closing/area.hh>
#include <mln/morpho/watershed/flooding.hh>

#include <mln/test/predicate.hh>
#include <mln/pw/value.hh>

#include <mln/labeling/colorize.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/ppm/save.hh>

#include "apps/morphers/recorder.hh"

#include "apps/data.hh"


int main()
{
  using namespace mln;
  using mln::value::int_u8;
  using mln::value::label_8;

  typedef image2d<int_u8> I;
  I lena = io::pgm::load<int_u8>(MLN_IMG_DIR "/tiny.pgm");

  // Gradient.
  I grad = morpho::gradient(lena, win_c4p());
  // Simplification of the gradient.
  unsigned lambda = 5;
  I closed_grad = morpho::closing::area(grad, c4(), lambda);
  // Attach recorder to closed gradient.
  decorated_image< I, recorder<I> > closed_grad_rec = record(closed_grad);

  // Watershed transform.
  typedef label_8 L; 
  L nbasins;
  typedef mln_ch_value_(I, L) J;
  decorated_image< J, recorder<J> > wst_rec =
    morpho::watershed::flooding(closed_grad_rec, c4(), nbasins);
  std::cout << nbasins << std::endl;

  // Dump recorded frames.
  /* FIXME: Hand-made (it partly duplicates ppm::save() for
     decorated_image's).  We should be able to process sequences of
     images.  */
  for (size_t i = 0; i < wst_rec.decoration().sequence.size(); ++i)
    {
      J frame = wst_rec.decoration().sequence[i];

      // Skip ``empty'' frames.
      if (test::predicate(frame.domain(), pw::value(frame) == 0))
	  continue;

      std::stringstream s;
      s << std::setfill ('0') << std::setw (6) << i;
      io::ppm::save(labeling::colorize(value::rgb8(), frame, nbasins),
      		    std::string("lena-wst") + s.str() + ".ppm");
    }
}
