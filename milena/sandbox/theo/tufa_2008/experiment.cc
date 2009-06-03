// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

/// \file sandbox/geraud/tufa/experiment.cc

#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>
#include <mln/literal/black.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>
#include <mln/debug/println.hh>

#include <mln/core/site_set/p_array.hh>
#include <mln/data/sort_psites.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/morpho/tree/data.hh>

#include <mln/accu/volume.hh>
#include <mln/win/disk2d.hh>

#include <mln/morpho/tree/compute_attribute_image.hh>
#include <mln/morpho/meyer_wst.hh>
#include <mln/morpho/opening.hh>
#include <mln/morpho/closing_area.hh>

#include <mln/data/fill.hh>

#include <mln/data/transform.hh>
#include <mln/data/stretch.hh>



namespace mln
{

  struct colorize : Function_v2v< colorize >
  {
    typedef value::rgb8 result;
    colorize(unsigned max)
      : lut(max + 1)
    {
      lut[0] = literal::black;
      for (unsigned i = 1; i <= max; ++i)
	lut[i] = result(100 + std::rand() % 150,
			100 + std::rand() % 150,
			100 + std::rand() % 150);
    }
    result operator()(unsigned i) const
    {
      return lut[i];
    }
    std::vector<result> lut;
  };

} // mln




void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm" << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 2)
    usage(argv);


  typedef image2d<int_u8> I;
  I f;
  io::pgm::load(f, argv[1]);

  typedef p_array<point2d> S;
  S s = data::sort_psites_decreasing(f);

  // Children go towards lower levels so leafs are regional minima.
  // We get a min-tree so that we can perform morphological closings. 

  morpho::tree::data<I,S> t(f, s, c4());
  accu::volume<I> attr;
  
  image2d<unsigned> a = morpho::tree::compute_attribute_image(attr, t);
  io::pgm::save(data::stretch(int_u8(), a),
		"a.pgm");

  unsigned n;
  image2d<unsigned> wst_a = morpho::meyer_wst(a, c4(), n);
  io::ppm::save(data::transform(wst_a, colorize(n)),
		"wst_a.ppm");
  std::cout << "n(a) = " << n << std::endl;

  // FIXME: ça n'a pas de sens de faire ce qui est dessous... :-(

  
  image2d<unsigned> aa = morpho::closing_area(a, c4(), 100);
  io::pgm::save(data::stretch(int_u8(), aa),
		"aa.pgm");

  image2d<unsigned> wst_aa = morpho::meyer_wst(aa, c4(), n);
  io::ppm::save(data::transform(wst_aa, colorize(n)),
		"wst_aa.ppm");
  std::cout << "n(aa) = " << n << std::endl;
}
