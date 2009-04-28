#include <iostream>
#include <fstream>

#include <mln/core/image/image3d.hh>
#include <mln/io/dump/save.hh>

#include <mln/core/image/image2d.hh>
#include <mln/debug/slices_2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/save.hh>
#include <mln/level/stretch.hh>

using namespace mln;
using value::int_u8;

#define MAX_SLI 750
#define MAX_ROW 375
#define MAX_COL 80



int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cout << "Usage: " << argv[0] << " cube.in dump.out" << std::endl;
    return 1;
  }

  image3d<unsigned int> ima(MAX_SLI, MAX_ROW, MAX_COL);

  std::ifstream file_in(argv[1]);
  unsigned int val;
  unsigned int col = 0;
  unsigned int row = 0;
  unsigned int sli = 0;

  while (file_in >> val)
  {
    ima.at_(sli, row, col) = val;
    ++row;
    if (row % MAX_ROW == 0)
    {
      row = 0;
      ++sli;
      if (sli % MAX_SLI == 0)
      {
	sli = 0;
	++col;
      }
    }
  }

  io::dump::save(ima, argv[2]);

  image2d<int_u8> ima_flat = debug::slices_2d(level::stretch(int_u8(), ima), 1.f, 0);
  io::pgm::save(ima_flat, "debug.pgm");

  return 0;
}
