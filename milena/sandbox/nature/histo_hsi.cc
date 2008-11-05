// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#include <iterator>
#include <string>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/cast_image.hh>
#include <mln/core/image/thru.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/site_set/p_set.hh>

#include <mln/value/rgb8.hh>
#include <mln/value/hsi.hh>

#include <mln/fun/v2v/rgb_to_hsi.hh>
#include <mln/level/to_enc.hh>
#include <mln/fun/meta/hue.hh>
#include <mln/fun/meta/sat.hh>
#include <mln/fun/meta/inty.hh>

#include <mln/level/compare.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/io/pgm/save.hh>

#include <mln/accu/histo.hh>
#include <mln/histo/compute.hh>
#include <mln/level/transform.hh>
#include <mln/level/fill.hh>

using namespace mln;
using namespace value;

typedef int_u16 u_t;


template <typename I>
void save_histo(Image<I> &i, std::string &name)
{
  I& ima = exact(i);

  histo::data<float01_8> h = histo::compute(ima);

  // Compute histo max
  unsigned max = 0;
  mln_viter(mln::value::set<float01_8>) v(h.vset());

  for_all(v)
    if (h(v) > max)
      max = h(v);

  std::cout << std::endl;

  image2d<bool> output(max, mln_card(float01_8), 0);
  std::cout << max << "x" << mln_card(float01_8) << std::endl;
  level::fill(output, true);

  for_all(v)
    for (size_t i = 0; i < h(v); ++i)
      {
	//	std::cout << max - i - 1 << ", " << (unsigned) (((float01_8)v).to_enc()) << " ; " ;
	if (output.has(point2d(max - i - 1, ((float01_8)v).to_enc() )))
	  output(point2d(max - i - 1, ((float01_8)v).to_enc() )) = false;
      }

  io::pbm::save(output, name);
}

int main (int argc, const char* argv [])
{
  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " in.ppm [other_files.ppm]" << std::endl;
    return 1;
  }

  for (int i = 1; i < argc; ++i)
    {

      image2d<rgb8> input;
      io::ppm::load(input, argv[i]);

      image2d<hsi_f> hsi = level::transform(input, fun::v2v::f_rgb_to_hsi_f);
      thru<mln::meta::hue<hsi_f>, image2d<hsi_f> > h(hsi);
      cast_image_<float01_8, thru<mln::meta::hue<hsi_f>, image2d<hsi_f> > > hue(h);

      std::string n(argv[i]);
      n.erase(n.length() - 4);
      image2d<int_u8> henc;
      level::transform(hue, fun::v2v::enc< float01_8 >(), henc);
      io::pgm::save(henc, n.append("_hue.pgm"));

      image2d<hsi_f>::piter p(hsi.domain());
      float m = 0;
      for_all(p)
      {
	if (h(p) > m)
	  m = h(p);
      }
      std::cout << std::endl << m << std::endl;



      std::string name(argv[i]);
      name.erase(name.length() - 4);
      save_histo(hue, name.append("_hue.pbm"));


      thru<mln::meta::sat<hsi_f>, image2d<hsi_f> > s(hsi);
      cast_image_<float01_8, thru<mln::meta::sat<hsi_f>, image2d<hsi_f> > > sat(s);

      n = argv[i];
      n.erase(n.length() - 4);
      image2d<int_u8> senc;
      level::transform(sat, fun::v2v::enc< float01_8 >(), senc);
      io::pgm::save(senc, n.append("_sat.pgm"));

      m = 0;
      for_all(p)
      {
	if (s(p) > m)
	  m = s(p);
      }
      std::cout << std::endl << m << std::endl;


      name = argv[i];
      name.erase(name.length() - 4);
      save_histo(sat, name.append("_sat.pbm"));

      thru<mln::meta::inty<hsi_f>, image2d<hsi_f> > l(hsi);
      cast_image_<float01_8, thru<mln::meta::inty<hsi_f>, image2d<hsi_f> > > inty(l);

      n = argv[i];
      n.erase(n.length() - 4);
      image2d<int_u8> ienc;
      level::transform(inty, fun::v2v::enc< float01_8 >(), ienc);
      io::pgm::save(ienc, n.append("_inty.pgm"));


      m = 0;
      for_all(p)
      {
	if (l(p) > m)
	  m = l(p);
      }
      std::cout << std::endl << m << std::endl;


      name = argv[i];
      name.erase(name.length() - 4);
      save_histo(inty, name.append("_inty.pbm"));

    }
}
