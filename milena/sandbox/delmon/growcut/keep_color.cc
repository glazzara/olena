#include <mln/core/image/image2d.hh>
#include <mln/io/pgm/all.hh>
#include <mln/value/all.hh>

int main (int argc, char ** argv)
{
  using namespace mln;

  if (argc != 4)
    return 1;
  std::string file_in   = argv[1];
  std::string file_sel  = argv[2];
  std::string file_out  = argv[3];

  image2d<value::int_u8> in;
  io::pgm::load (in, file_in);

  image2d<value::int_u8> sel;
  io::pgm::load (sel, file_sel);

  image2d<value::int_u8> out;
  initialize(out, in);

  mln_piter_(image2d<value::int_u8>) p(in.domain());
  for_all (p)
    if (sel(p) == COLOR)
      out(p) = in(p);
    else
      out(p) = 255 - COLOR;

  mln::io::pgm::save (out, file_out);

  return 0;
}
