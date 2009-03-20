#include <iostream>
#include <fstream>

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>

#include <mln/io/magick/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/level/stretch.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>

#include <mln/world/binary_2d/projected_histo.hh>
#include <mln/world/binary_2d/subsample.hh>


using namespace mln;
using value::int_u8;
using value::int_u12;

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cout << "Usage: " << argv[0] << "input.ext output.pgm" << std::endl;
    return 1;
  }

  image2d<bool> ima;
  io::magick::load(ima, argv[1]);

  image2d<int_u8> ima_sampled = world::binary_2d::subsample(ima, 3);
  io::pgm::save(ima_sampled, argv[2]);

  image1d<float> row_histo(ima.nrows());
  image1d<float> col_histo(ima.ncols());
  world::binary_2d::projected_histo(ima, row_histo, col_histo);

  std::ofstream fout_row("row.plot");
  for (unsigned int i = 0; i < row_histo.ninds(); ++i)
  {
    fout_row << i << " " << row_histo.at_(i) << std::endl;
  }

  std::ofstream fout_col("col.plot");
  for (unsigned int i = 0; i < col_histo.ninds(); ++i)
  {
    fout_col << i << " " << col_histo.at_(i) << std::endl;
  }

  return 0;
}
