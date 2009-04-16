#include "../exec/filetype.hh"

#include <mln/core/alias/point2d.hh>
#include <mln/core/alias/dpoint2d.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm regions.ppm output.ppm" << std::endl
	    << "  'input': dark text / light background" << std::endl
	    << "  'regions': one color per region." << std::endl;
  std::abort();
}


const mln::value::int_u8
foo(const mln::value::int_u8& c,
    const mln::value::int_u8& g)
{
  float ratio = 1.f - float(g) / 256.f;
  return unsigned(ratio * c);
}


int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::rgb8;

  if (argc != 4)
    usage(argv);

  trace::entering("main");

  image2d<int_u8> input;
  io::pgm::load(input, argv[1]);
  box2d b = input.domain();

  image2d<rgb8> reg, out(b);
  io::ppm::load(reg, argv[2]);

  mln_piter_(box2d) p(b);
  for_all(p)
    out(p) = rgb8( foo(reg(p).red(), input(p)),
		   foo(reg(p).green(), input(p)),
		   foo(reg(p).blue(), input(p)) );

  io::ppm::save(out, argv[3]);

  trace::exiting("main");
}
