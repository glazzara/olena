// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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


#include <mln/arith/revert.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/data/stretch.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/load.hh>
#include <mln/morpho/closing/volume.hh>
#include <mln/morpho/elementary/dilation.hh>
#include <mln/morpho/watershed/flooding.hh>
#include <mln/value/label_8.hh>

#include "../../mln/histo/compute_histo_3d.hh"
#include "../../mln/histo/classify_with_histo_3d.hh"

using namespace mln;
using namespace mln::value;


namespace mln
{

  struct rgb8to6 : Function_v2v< rgb8to6 >
  {
    typedef value::rgb<6> result;
    value::rgb<6> operator()(const value::rgb<8>& c) const
    {
      value::rgb<6> res(c.red() / 4, c.green() / 4, c.blue() / 4);
      return res;
    }
  };

}


int
main(int argc, char** argv)
{
  // check arguments
  if (argc != 4)
  {
    std::cerr << "Usage:" << std::endl
	      << "  ./a.out ../../../../img/lena.ppm 51 out.pgm" << std::endl
	      << std::endl
	      << "BTW, the number is the closure's lambda." << std::endl;
    exit(1);
  }

  typedef value::rgb<6> rgb6;
  typedef value::int_u<6> int_u6;

  // build test image
  std::cout << "  => loading " << argv[1] << "..." << std::endl;
  image2d<value::rgb8> ima8;
  io::ppm::load(ima8, argv[1]);
  image2d<rgb6> ima6 = data::transform(ima8, rgb8to6());

  // build histo
  std::cout << "  => computing histogram..." << std::endl;
  image3d<unsigned> histo = histo::compute_histo_3d(ima6);

  // build reverted histo
  std::cout << "  => computing reverted histogram..." << std::endl;
  image3d<unsigned> reverted = arith::revert(histo);

  // compute morpho closure
  std::cout << "  => computing closure..." << std::endl;
  image3d<unsigned> closed = morpho::closing::volume(reverted, c6(), atoi(argv[2]));

  // compute watershed transform
  std::cout << "  => computing watershed..." << std::endl;
  value::label_8 nbasin;
  image3d<value::label_8> labels = morpho::watershed::flooding(closed, c6(), nbasin);
  std::cout << "found " << nbasin << " labels" << std::endl;

  // close maximas
  labels = morpho::elementary::dilation(labels, c18());

  // compute output image
  std::cout << "  => computing output labelized image..." << std::endl;
  image2d<label_8> out = histo::classify_with_histo_3d(ima6, labels);

  // save output image
  std::cout << "  => saving " << argv[3] << "..." << std::endl;
  io::pgm::save(data::stretch(int_u8(), out), argv[3]);
}
