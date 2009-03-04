#include <mln/core/image/image2d.hh>
#include <mln/io/ppm/all.hh>
#include <mln/value/all.hh>

int main (int argc, char ** argv)
{
  using namespace mln;

  if (argc != 4)
    return 1;
  std::string file_in   = argv[1];
  std::string file_sel  = argv[2];
  std::string file_out  = argv[3];

  image2d<value::rgb8> in;
  io::ppm::load (in, file_in);

  image2d<value::rgb8> sel;
  io::ppm::load (sel, file_sel);

  image2d<value::rgb8> out;
  initialize(out, in);

  mln_piter_(image2d<value::rgb8>) p(in.domain());
  for_all (p)
    if (sel(p) == value::rgb8(0, 0, 255))
      out(p) = value::rgb8(0, 0, 0);
    else
      out(p) = in(p);

  mln::io::ppm::save (out, file_out);

  return 0;
}
