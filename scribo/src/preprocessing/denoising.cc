// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/data/convert.hh>
#include <mln/io/pbm/all.hh>
#include <mln/value/label.hh>
#include <mln/accu/math/count.hh>
#include <mln/labeling/relabel.hh>
#include <mln/labeling/compute.hh>
#include <mln/labeling/foreground.hh>

#include <scribo/debug/usage.hh>

const char *args_desc[][2] =
{
  { "input.pbm", "A binary image. 'True' for objects, 'False'\
for the background." },
  { "nbh", "Select the neighborhood used for checking neighbors. '4' will use C4, '8' will use C8." },
  { "n_nbh", "The minimum neighbor count to be set to true." },
  {0, 0}
};

int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc != 5)
    return scribo::debug::usage(argv,
				"Remove noise.",
				"input.pbm nbh min_nbh output.pbm",
				args_desc,
				"A binary image.");

  trace::entering("main");

  typedef image2d<bool> I;
  I input;
  io::pbm::load(input, argv[1]);

  typedef value::label<30> V;

  neighb2d nbh;
  if (atoi(argv[2]) == 4)
    nbh = c4();
  else if (atoi(argv[2]) == 8)
    nbh = c8();
  else
  {
    std::cout << "Error: nbh must be set to '4' or '8'!" << std::endl;
    return 1;
  }

  unsigned min_nbh = atoi(argv[3]);


  V nlabels;
  image2d<V> lbl = labeling::foreground(input, nbh, nlabels);
  std::cout << nlabels << std::endl;


  util::array<unsigned> result = labeling::compute(accu::meta::math::count(), lbl, nlabels);

  fun::i2v::array<bool> f(static_cast<unsigned>(nlabels) + 1, true);
  for (unsigned i = 1; i <= nlabels; ++i)
    if (result(i) < min_nbh)
      f(i) = false;

  labeling::relabel_inplace(lbl, nlabels, f);
  std::cout << nlabels << std::endl;


//   mln_piter_(I) p(input.domain());
//   mln_niter_(neighb2d) n(nbh, p);
//   unsigned n_nbh;
//   for_all(p)
//     if (input(p))
//     {
//       n_nbh = 0;
//       for_all(n)
// 	if (input(n))
// 	  ++n_nbh;

//       if (n_nbh < min_nbh)
// 	input(p) = false;
//     }

  io::pbm::save(data::convert(bool(), lbl), argv[4]);

  trace::exiting("main");

}
