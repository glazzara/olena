#include "fllt.hh"

int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  typedef fllt_tree(point2d, int_u8) tree_type;

  if (argc != 2)
  {
    std::cerr << "usage: " << argv[0] << " filename" << std::endl;
    return 1;
  }

  image2d<int_u8> lena;
  io::pgm::load(lena, argv[1]);
  tree_type tree = fllt(lena);
}
