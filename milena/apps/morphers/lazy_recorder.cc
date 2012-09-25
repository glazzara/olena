// Copyright (C) 2011, 2012 EPITA Research and Development Laboratory (LRDE)
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
/// image in a lazy fashion.
///
/// To produce an AVI movie from the `lena-fill*.ppm' files, use:
///
///   for f in lena-fill*ppm; convert $f -scale 2500% $(basename $f .ppm).png
///   mencoder "mf://lena-fill*.png" -o lena-fill.avi -ovc lavc -lavcopts vcodec=mjpeg
///
/// The output `lena-fill.avi' can be embedded in a PDF file.  */

#include <string>

#include <mln/core/image/image2d.hh>

#include <mln/value/rgb8.hh>
#include <mln/literal/colors.hh>
#include <mln/data/fill.hh>

#include <mln/io/ppm/load.hh>

#include "apps/morphers/lazy_recorder.hh"

#include "apps/data.hh"


int main()
{
  using namespace mln;
  using mln::value::rgb8;

  typedef image2d<rgb8> I;
  I lena = io::ppm::load<rgb8>(MLN_IMG_DIR "/tiny.ppm");
  decorated_image< I, lazy_recorder<I> > lena_rec = lazy_record(lena);
  data::fill(lena_rec, literal::green);
  ppm::save(lena_rec, "lena-fill-lazy");
}
