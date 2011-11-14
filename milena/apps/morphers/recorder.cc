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
/// \brief Morpher recording every change in the morphed image.
///
/// To produce an AVI movie from the `lena-fill*.ppm' files, use:
///
///   for f in lena-fill*ppm; convert $f -scale 2500% $(basename $f .ppm).png
///   mencoder "mf://lena-fill*.png" -o lena-fill.avi -ovc lavc -lavcopts vcodec=mjpeg
///
/// The output `lena-fill.avi' can be embedded in a PDF file.  */

#include <sstream>
#include <iomanip>

#include <string>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/imorph/decorated_image.hh>

#include <mln/value/rgb8.hh>

#include <mln/core/routine/duplicate.hh>

#include <mln/literal/colors.hh>
#include <mln/data/fill.hh>

#include <mln/io/ppm/all.hh>

#include "apps/data.hh"


// FIXME: mln::decorated_image lacks a proper definition of
// properties! (see mln/core/image/imorph/decorated_image.hh
namespace mln
{

  namespace trait
  {

    template <typename I, typename D>
    struct image_< decorated_image<I,D> >
      : default_image_morpher< I,
			       mln_value(I),
			       decorated_image<I,D> >
    {
      typedef trait::image::category::identity_morpher category;
    };

  } // end of namespace mln::trait

} // end of namespace mln


// Recorder.
template <typename I>
struct recorder
{
  void reading(const I&, const mln_psite(I)&) const
  {
    // N/A.
  }

  void writing(I& ima, const mln_psite(I)&, const mln_value(I)&)
  {
    sequence.push_back(mln::duplicate(ima));
  }

  std::vector<I> sequence;
};

template <typename I>
mln::decorated_image< I, recorder<I> >
record(mln::Image<I>& ima)
{
  return mln::decorate(ima, recorder<I>());
}


// I/O.
namespace ppm
{
  template <typename I>
  void
  save(const mln::decorated_image< I, recorder<I> >& rec,
       const std::string& prefix)
  {
    for (size_t i = 0; i < rec.decoration().sequence.size(); ++i)
      {
	std::stringstream s;
	s << std::setfill ('0') << std::setw (6) << i;
	mln::io::ppm::save(rec.decoration().sequence[i],
			   prefix + s.str() + ".ppm");
      }
  }
}


int main()
{
  using namespace mln;
  using mln::value::rgb8;

  typedef image2d<rgb8> I;
  I lena = io::ppm::load<rgb8>(MLN_IMG_DIR "/tiny.ppm");
  decorated_image< I, recorder<I> > lena_rec = record(lena);
  /* FIXME: Cheat: use generic fill as mln::decorated_image does not
     define properly its properties.  */
  data::impl::generic::fill_with_value(lena_rec,
				       literal::green);
  ppm::save(lena_rec, "lena-fill");
}
