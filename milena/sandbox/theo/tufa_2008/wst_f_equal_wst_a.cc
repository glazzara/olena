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

/// \file sandbox/geraud/tufa/steps.cc

#include <mln/core/image/image2d.hh>
#include <mln/pw/all.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/pbm/save.hh>
#include <mln/literal/black.hh>

#include <mln/core/site_set/p_array.hh>
#include <mln/data/sort_psites.hh>
#include <mln/data/transform.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/morpho/tree/data.hh>

#include <mln/accu/shape/volume.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>
#include <mln/morpho/meyer_wst.hh>

#include <mln/labeling/regional_minima.hh>

#include <mln/core/var.hh>


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

  unsigned nref, n;

  typedef image2d<int_u8> I;
  I f;
  io::pgm::load(f, argv[1]);

  labeling::regional_minima(f, c4(), nref);
  std::cout << nref << std::endl;


  image2d<unsigned> wst_f = morpho::meyer_wst(f, c4(), n);
  mln_assertion(n == nref);

  io::ppm::save(data::transform(wst_f, colorize(n)),
		"wst_f.ppm");
  mln_VAR(WST_f, (pw::value(wst_f) == pw::cst(0u)) | f.domain()); 
  io::pbm::save(WST_f, "wst_f.pbm");


  typedef p_array<point2d> S;
  S s = data::sort_psites_decreasing(f);

  // Children go towards lower levels so leafs are regional minima.
  // We get a min-tree so that we can perform morphological closings. 

  morpho::tree::data<I,S> t(f, s, c4());
  accu::shape::volume<I> vol;
  image2d<unsigned> a = morpho::tree::compute_attribute_image(vol, t);
  
  labeling::regional_minima(a, c4(), n);
  mln_assertion(n == nref);


  image2d<unsigned> wst_a = morpho::meyer_wst(a, c4(), n);
  mln_assertion(n == nref);

  io::ppm::save(data::transform(wst_a, colorize(n)),
		"wst_a.ppm");
  mln_VAR(WST_a, (pw::value(wst_a) == pw::cst(0u)) | f.domain()); 
  io::pbm::save(WST_a, "wst_a.pbm");

  
  io::pbm::save((pw::value(WST_a) != pw::value(WST_f)) | f.domain(),
		"diff.pbm");
}
