/*!
 * \file   dt.cc
 * \author ornthalas <ornthalas@gmail.com>
 */

#include <iostream>
#include <mln/core/image/image2d.hh>
#include <mln/debug/println.hh>
#include <mln/make/win_chamfer.hh>
#include <mln/level/fill.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/pbm/save.hh>
#include <mln/level/stretch.hh>
#include <mln/level/paste.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>

#include <mln/core/image/sub_image.hh>
#include <mln/core/image/image_if.hh>
#include <mln/pw/all.hh>

#include "../dt/path.hh"
// #include "../dt/raw_path_fast.hh"
// #include "../dt/raw_path_slow.hh"

int main()
{
  using namespace mln;

  typedef image2d<value::rgb8> I;
  typedef mln_point_(I) point;
  point start;
  point end;

  I ima = io::ppm::load<value::rgb8>("tmp.ppm"); // "../img/monkeys_april.ppm");
  mln_fwd_piter_(I) p(ima.domain());
  mln_bkd_piter_(I) pp(ima.domain());

  // Create window
  int ws[] = { 3, 2, 3,
	       2, 0, 2,
	       3, 2, 3 };

  // Search green point
  std::cout << "search green point..." << std::endl;
  for_all(p)
    if (ima(p) == value::rgb8(0, 255, 0))
    {
      start = p;
      break;
    }
  std::cout << "  => green point is " << start << std::endl << std::endl;

  // Search red point
  std::cout << "search red point..." << std::endl;
  for_all(pp)
    if (ima(pp) == value::rgb8(255, 0, 0))
    {
      end = pp;
      break;
    }
  std::cout << "  => red point is " << end << std::endl << std::endl;

  // Create mask
  std::cout << "create mask..." << std::endl;
  mln_ch_value_(I, bool) mask;
  initialize(mask, ima);

  for_all(p)
    mask(p) = (ima(p) != value::rgb8(0, 0, 0));
  std::cout << "  => saving mask.pbm..." << std::endl;
  io::pbm::save(mask, "mask.pbm");
  std::cout << "  => saved !" << std::endl << std::endl;

  // Create input
  std::cout << "create input..." << std::endl;
  mln_ch_value_(I, bool) input;
  initialize(input, ima);

  for_all(p)
    input(p) = (p == end);
  std::cout << "  => saving input.pbm..." << std::endl;
  io::pbm::save(input, "input.pbm");
  std::cout << "  => saved !" << std::endl << std::endl;

  // Create return image
  std::cout << "create output..." << std::endl;
  image2d<point> out;
  std::cout << "  => done !" << std::endl << std::endl;

  std::cout << "call path algorithm..." << std::endl;
  out = dt::path(input | (pw::value(mask) == pw::cst(true)),
		 make::w_window2d(ws), mln_max(unsigned));
  std::cout << "  => done !" << std::endl << std::endl;

  // Create output
  std::cout << "create return image" << std::endl;
  I ret;
  initialize(ret, ima);
  point c = start;

  // Copy inut into ret
  std::cout << "copy original image into return image..." << std::endl;
  for_all(p)
    ret(p) = ima(p);
  std::cout << "  => done !" << std::endl << std::endl;

  // Create path into ret
  std::cout << "create path into return image..." << std::endl;
  while (c != end)
  {
    ret(c) = value::rgb8(255, 0, 0);
    c = out(c);
  }
  std::cout << "  => done !" << std::endl << std::endl;

  // save
  std::cout << "  => saving ret.ppm..." << std::endl;
  io::ppm::save(ret, "ret.ppm");
  std::cout << "  => saved !" << std::endl << std::endl;
}
