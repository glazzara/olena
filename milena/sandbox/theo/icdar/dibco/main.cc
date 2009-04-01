// Copyright (C) 2009 EPITA Research and Development Laboratory
// (LRDE)
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

/// \file theo/icdar/dibco/main.cc
///
/// Simple thresholding of a gray-level document.

# include <mln/io/pgm/load.hh>
# include <mln/io/pgm/save.hh>
# include <mln/io/pbm/save.hh>

# include <mln/debug/println.hh>
# include <mln/fun/l2l/wrap.hh>
# include <mln/level/transform.hh>

# include <mln/core/image/image1d.hh>
# include <mln/core/alias/neighb1d.hh>

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>

# include <mln/core/routine/duplicate.hh>
# include <mln/core/image/image_if.hh>
# include <mln/pw/all.hh>

# include <mln/histo/compute.hh>
# include <mln/debug/histo.hh>
# include <mln/convert/from_to.hh>

# include <mln/morpho/elementary/gradient_internal.hh>
# include <mln/morpho/elementary/gradient_external.hh>
# include <mln/morpho/closing/height.hh>
# include <mln/morpho/closing/volume.hh>
# include <mln/morpho/watershed/flooding.hh>

# include <mln/linear/gaussian_1d.hh>
# include <mln/labeling/regional_minima.hh>
# include <mln/labeling/compute.hh>
# include <mln/value/label_8.hh>
# include <mln/accu/center.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm output.pbm" << std::endl
	    << "  ICDAR'2009: DIBCO." << std::endl;
  std::abort();
}


namespace mln
{

  int
  find_threshold(const histo::array<value::int_u8>& h)
  {

    const float sigma = 5; // FIXME: hard-coded!
  
    util::array<point1d> c;
    value::label_8 n;

    {
      image1d<unsigned> h_, hs_;
      image1d<value::label_8> l;

      convert::from_to(h, h_);
      hs_ = linear::gaussian_1d(h_, sigma, 0);
      l = labeling::regional_minima(hs_, c2(), n);
     
      {
// 	debug::println("l", l);
// 	debug::histo(h, "tmp_h.txt");
	
// 	std::ofstream file("tmp_hs.txt");
// 	mln_piter_(box1d) p(h_.domain());
// 	for_all(p)
// 	  file << p.ind() << ' ' << hs_(p) << std::endl;
// 	file.close();
      }

      accu::center<point1d, point1d> a;
      c = labeling::compute(a, l, n);

      c[0] = point1d(0); // Force a neutral value for the non-label value (0).

      // std::cout << "c = " << c << std::endl;
    }


    int threshold;

    {
      std::vector<int> v;
      for (unsigned i = 0; i <= n; ++i)
	v.push_back(c[i].ind());
      std::sort(v.begin(), v.end());

      for (unsigned i = 0; i <= n; ++i)
	if (v[i] != 0)
	  {
	    threshold = v[i];
	    break;
	  }

      std::cout << "threshold = " << threshold << std::endl;
    }

    return threshold;
  }


} // ! mln



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 3)
    usage(argv);

  image2d<int_u8> input;
  io::pgm::load(input, argv[1]);

  typedef int_u8 V;
  histo::array<V> h;

  image2d<int_u8> g;
  g = morpho::elementary::gradient_internal(input, c4());
  io::pgm::save(g, "tmp_g.pgm");

  g = morpho::closing::height(g, c4(), 3);  // FIXME: hard-coded!
  
  unsigned nbasins;
  image2d<unsigned> w = morpho::watershed::flooding(g, c4(), nbasins);

  io::pgm::save(level::transform(w,
				 fun::l2l::wrap<int_u8>()),
		"tmp_w.pgm");

  h = histo::compute(g | (pw::value(w) == pw::cst(0)));

  unsigned threshold = find_threshold(h);

//   output = duplicate((pw::value(g) > pw::cst(threshold)) | input.domain());

  io::pbm::save((pw::value(g) > pw::cst(threshold)) | input.domain(),
		"tmp_g.pbm");

  image2d<bool> output;
  output = duplicate((pw::value(g) > pw::cst(threshold)
		      && pw::value(w) == pw::cst(0)) | input.domain());

  io::pbm::save(output, argv[2]);
}
