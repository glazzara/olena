#include <string>

#include <mln/core/image2d.hh>
#include <mln/value/rgb.hh>
#include <mln/value/int_u.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/load.hh>

#include "mln/io/tikz/save_header.hh"
#include "mln/io/tikz/save.hh"

int main (int argc, char *argv[]) {
  using namespace mln;
  using value::rgb;

  if (argc <= 2)
    {
      std::cout << "usage : " << argv[0] << " input.pnm output.tex" << std::endl;
      return 0;
    }

  std::string name(argv[1]);
  std::string ext(name.substr(name.size() - 3));

  if (ext == "pgm")
    {
      image2d< value::int_u<8> > ima;
      io::pgm::load(ima, name);
      io::tikz::save(ima, argv[2], ima);
      return 0;
    }

  if (ext == "ppm")
    {
      image2d< rgb<8> > ima;
      io::ppm::load(ima, name);
      io::tikz::save(ima, argv[2]);
      return 0;
    }

  std::cerr << "Unknown extension, must be ppm or pgm" << std::endl;

  return 1;
} // int main ()
