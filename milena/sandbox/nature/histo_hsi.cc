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
void save_histo(Image<I> &i, std::string &name, unsigned width, unsigned height, unsigned npoints)
{
  I& ima = exact(i);

  histo::data<u_t> h = histo::compute(ima);

  double norm = (double) npoints / (double) height;

  image2d<bool> output(height + 1, width + 1, 0);
  level::fill(output, true);

  mln_viter(mln::value::set<u_t>) v(h.vset());
  for_all(v)
    if (h(v) > 0)
      for (u_t i = 0; i < h(v)/norm; ++i)
	{
	  // std::cout << height - i << ", " << (u_t)v << std::endl;
	  output(point2d(height - i, (u_t)v)) = false;
	}

  io::pbm::save(output, name);
}

int main (int argc, const char* argv [])
{
  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " in.ppm [other_files.ppm]" << std::endl;
    return 1;
  }

  for (unsigned i = 1; i < argc; ++i)
    {

      image2d<rgb8> input;
      io::ppm::load(input, argv[i]);

      unsigned npoints = input.ncols() * input.nrows();

      image2d<hsi_f> hsi = level::transform(input, fun::v2v::f_rgb_to_hsi_f);
      thru<mln::meta::hue<hsi_f>, image2d<hsi_f> > h(hsi);
      cast_image_<u_t, thru<mln::meta::hue<hsi_f>, image2d<hsi_f> > > hue(h);

      std::string n(argv[i]);
      n.erase(n.length() - 4);
      io::pgm::save(hue, n.append("_hue.pgm"));

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
      save_histo(hue, name.append("_hue.pbm"), 256, 360, npoints);

      thru<mln::meta::sat<hsi_f>, image2d<hsi_f> > s(hsi);
      cast_image_<u_t, thru<mln::meta::sat<hsi_f>, image2d<hsi_f> > > sat(s);

      n = argv[i];
      n.erase(n.length() - 4);
      io::pgm::save(sat, n.append("_sat.pgm"));

      m = 0;
      for_all(p)
      {
	if (s(p) > m)
	  m = s(p);
      }
      std::cout << std::endl << m << std::endl;


      name = argv[i];
      name.erase(name.length() - 4);
      save_histo(sat, name.append("_sat.pbm"), 100, 2560, npoints);

      thru<mln::meta::inty<hsi_f>, image2d<hsi_f> > l(hsi);
      cast_image_<u_t, thru<mln::meta::inty<hsi_f>, image2d<hsi_f> > > inty(l);

      n = argv[i];
      n.erase(n.length() - 4);
      io::pgm::save(inty, n.append("_inty.pgm"));


      m = 0;
      for_all(p)
      {
	if (l(p) > m)
	  m = l(p);
      }
      std::cout << std::endl << m << std::endl;


      name = argv[i];
      name.erase(name.length() - 4);
      save_histo(inty, name.append("_inty.pbm"), 256, 256, npoints);
    }
}
