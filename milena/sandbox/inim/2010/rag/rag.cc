#include <iostream>
# include <cstdlib> // rand
# include <ctime> // rand

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include "rag.hh"

void usage(char* name)
{
  std::cerr << "usage: " << name << " <source.pgm>" << std::endl;
  exit(1);
}

int main(int argc, char* argv[])
{
  if (argc != 2)
    usage(argv[0]);
  // mln::Image<E>

  srand(time(0));

  using namespace mln;
  using value::int_u8;

  image2d<int_u8> input;
  io::pgm::load(input, argv[1]);
  io::pgm::save(rag(input), "out.pgm");
}
