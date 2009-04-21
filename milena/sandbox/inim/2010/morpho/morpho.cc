#include <iostream>
#include <sstream>
#include <string>
#include <mln/debug/println.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/data/fill.hh>
#include <mln/literal/all.hh>
#include <mln/arith/revert.hh>
#include <mln/level/convert.hh>
#include <mln/level/stretch.hh>
#include <mln/morpho/closing/structural.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/make/win_chamfer.hh>

int main(int argc, char** argv)
{
  using namespace mln;

  // check arguments
  if (argc != 4)
  {
    std::cerr << "Usage:" << std::endl
	      << "  ./a.out <in.pbm> <k> <out.pbm>" << std::endl;
    exit(1);
  }

  // build test image
  std::cout << "  => loading " << argv[1] << "..." << std::endl;
  image2d<bool> in;
  io::pbm::load(in, argv[1]);

  value::int_u8 k = 1;
  image2d<value::int_u8> out = level::convert(k, in);
  out = level::stretch(k, out);

  std::cout << "  => revert image..." << std::endl;
  out = arith::revert(out);

  // closure
/*  int ws[] = { 0, 1, 0,
	       1, 0, 1,
	       0, 1, 0 };
*/
  // w_window2d_int == w_window<dpoint2d, int>


  for (unsigned j = 20; j <= 40; j += 10)
  {
    window2d it;
    it.insert(0, 0);

    for (unsigned i = 0; i < j; ++i)
    {
      it.insert(0, -i);
      it.insert(0, i);
    }

    out = morpho::closing::structural(out, it);

    // export out image
    std::stringstream ss;
    ss << "../" << j << '/' << argv[3];
    std::string s;
    ss >> s;
    std::cout << "  => saving " << s << "..." << std::endl;
    io::pgm::save(out, s.c_str());
  }
}
