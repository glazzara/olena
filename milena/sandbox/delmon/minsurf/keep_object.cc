#include <mln/core/image/image2d.hh>
#include <mln/io/pgm/all.hh>
#include <mln/io/pbm/all.hh>
#include <mln/value/all.hh>

int main(int argc, char** argv)
{
  using namespace mln;
  image2d<value::int_u8> input;
  io::pgm::load(input, argv[1]);
  image2d<bool>	objects;
  initialize(objects, input);
  io::pbm::load(objects, argv[2]);
  image2d<value::int_u8> output;
  initialize(output, input);
  mln_piter_(image2d<value::int_u8>) p(input.domain());
  for_all (p)
    if (objects(p))
      output(p) = input(p);
    else
      output(p) = 0;
  io::pgm::save(output, argv[3]);
  return 0;
}
