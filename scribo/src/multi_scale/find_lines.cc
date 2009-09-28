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

#include <mln/core/alias/neighb2d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/data/convert.hh>
#include <mln/debug/superpose.hh>
#include <mln/io/pbm/all.hh>
#include <mln/io/ppm/save.hh>
#include <mln/pw/all.hh>
#include <mln/subsampling/subsampling.hh>
#include <mln/value/label_16.hh>
#include <mln/value/rgb8.hh>

#include <mln/world/binary_2d/enlarge.hh>

#include <scribo/debug/usage.hh>

#include <scribo/core/object_image.hh>
#include <scribo/primitive/extract/lines_h_pattern.hh>
#include <scribo/primitive/extract/lines_v_pattern.hh>

#include <scribo/filter/objects_h_thin.hh>
#include <scribo/filter/objects_v_thin.hh>

#include <sandbox/theo/Rd/sequential.hh>

#include <mln/morpho/erosion.hh>

const char *args_desc[][2] =
{
  { "input.pbm", "A binary image." },
  { "length", "   Minimum line length." },
  {0, 0}
};


namespace mln
{

  template <typename I>
  mln_concrete(I)
  process(const I& input, const std::string& filename,
	  unsigned length, unsigned delta, unsigned ratio)
  {
    I hlines = scribo::primitive::extract::lines_h_pattern(input,
							   length,
							   delta);

    value::label_16 nhlines;
    hlines = scribo::filter::objects_v_thin(hlines, c8(),
					    nhlines, delta * ratio);

//     I vlines = scribo::primitive::extract::lines_v_pattern(input,
// 							   length,
// 							   delta);


//     value::label_16 nvlines;
//     vlines = scribo::filter::objects_h_thin(vlines, c8(),
// 					    nvlines, delta * ratio);

//     image2d<value::rgb8> out = debug::superpose(input, hlines, literal::red);
//     out = debug::superpose(out, vlines, literal::green);
//     io::ppm::save(out, filename);

    return hlines;
  }


  template <typename I>
  mln_concrete(I)
  merge_results(const I& input,
		const I& out, const I& out_sub2x, const I& out_sub4x)
  {
    mln_concrete(I) output;
    initialize(output, input);



    return output;
  }


} // end of namespace mln



int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc != 4)
    return scribo::debug::usage(argv,
				"Extract discontinued horizontal and vertical lines (multi-scale version)",
				"input.pbm length output.ppm",
				args_desc,
				"A color image. Horizontal lines are in red and vertical lines in green.");

  trace::entering("main");

  typedef image2d<bool> I;
  dpoint2d none(0, 0);

  I input;
  io::pbm::load(input, argv[1]);


  // 1/1
  std::cout << "1/1" << std::endl;
  I hlines = scribo::primitive::extract::lines_h_pattern(input,
							 atoi(argv[2]),
							 3);



//   I vlines = scribo::primitive::extract::lines_v_pattern(input,
// 							 atoi(argv[2]),
// 							 3);

  image2d<value::rgb8> out = debug::superpose(input, hlines, literal::red);
//  out = debug::superpose(out, vlines, literal::green);
  io::ppm::save(out, "out_1_1.ppm");

//  I out = process(input, "out_1_1.ppm", atoi(argv[2]), 3);



  // 1/2
  std::cout << "1/2" << std::endl;
  I input_sub2x = mln::subsampling::subsampling(input, none, 2);
  I out_sub2 = process(input_sub2x, "out_1_2.ppm", atoi(argv[2]), 3, 2);



//   // 1/4
//   std::cout << "1/4" << std::endl;
//   I input_sub4x = mln::subsampling::subsampling(input, none, 4);
//   I out_sub4 = process(input_sub4x, "out_1_4.ppm", atoi(argv[2]), 3, 4);


//   out_sub4 = world::binary_2d::enlarge(out_sub4, 2);
  out_sub2 = world::binary_2d::enlarge(out_sub2, 1);

//  io::ppm::save(merge_results(input, out, out_sub2, out_sub4), argv[2]);

  I tmp      = morpho::Rd::sequential(hlines, input, c8());
  io::pbm::save(tmp, "rd.pbm");
  I tmp_sub2 = morpho::Rd::sequential(out_sub2, input, c8());
  io::pbm::save(tmp_sub2, "rd_sub2.pbm");

  out = debug::superpose(input, tmp_sub2, literal::red);
  out = debug::superpose(out, tmp, literal::red);
  io::ppm::save(out, "out.ppm");

  trace::exiting("main");
}
