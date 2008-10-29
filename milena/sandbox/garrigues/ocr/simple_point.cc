# include <mln/core/image/image2d.hh>
# include <mln/io/pbm/load.hh>
# include <mln/io/pbm/save.hh>
# include <mln/level/fill.hh>

# include "simple_point.hh"


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pbm output.pbm" << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 3)
    usage(argv);

  typedef image2d<bool> I;
  I input;
  io::pbm::load(input, argv[1]);

  I output;
  initialize(output, input);
  level::fill(output, false);

  mln_piter_(I) p(input.domain());
  for_all(p)
    if (input(p) && simple_point2d(input, 8, p))
      output(p) = true;

  io::pbm::save(output, argv[2]);
}
