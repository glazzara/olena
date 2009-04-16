#include "../exec/filetype.hh"

#include <mln/core/alias/point2d.hh>
#include <mln/core/alias/dpoint2d.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pbm regions.ppm output.ppm" << std::endl
	    << "  'input' is PBM: dark text / light background" << std::endl
	    << "  'regions': one color per region." << std::endl;
  std::abort();
}


int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::rgb8;

  if (argc != 4)
    usage(argv);

  trace::entering("main");

  image2d<bool> input;
  io::pbm::load(input, argv[1]);
  box2d b = input.domain();

  image2d<rgb8> reg, out(b);
  io::ppm::load(reg, argv[2]);

  data::fill(out, rgb8(0,0,0));

  mln_piter_(box2d) p(b);
  for_all(p) if (input(p))
    out(p) = reg.at_(p.row() / 4, p.col() / 4);

  io::ppm::save(out, argv[3]);

  trace::exiting("main");
}
