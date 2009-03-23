#include <iostream>
#include <fstream>

#include <mln/algebra/vec.hh>

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb1d.hh>

#include <mln/draw/line.hh>

#include <mln/io/magick/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/pbm/save.hh>

#include <mln/level/stretch.hh>

#include <mln/make/box2d.hh>

#include <mln/morpho/closing/volume.hh>
#include <mln/morpho/watershed/flooding.hh>

#include <mln/util/couple.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/value/label_16.hh>

#include <mln/world/binary_2d/projected_histo.hh>
#include <mln/world/binary_2d/subsample.hh>


using namespace mln;
using value::int_u8;
using value::int_u12;
using value::label_16;



int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cout << "Usage: " << argv[0] << "input.ext output.pgm" << std::endl;
    return 1;
  }

  image2d<bool> ima;
  io::magick::load(ima, argv[1]);

  //image2d<int_u8> ima_sampled = world::binary_2d::subsample(ima, 3);
  //io::pgm::save(ima_sampled, argv[2]);

  box2d left_box = make::box2d(0, 0, ima.nrows() - 1, (ima.ncols() / 3));
  util::couple<image1d<float>, image1d<float> > histos_left = world::binary_2d::projected_histo(ima | left_box);

  box2d middle_box = make::box2d(0, (ima.ncols() / 3), ima.nrows() - 1, (2 * ima.ncols() / 3));
  util::couple<image1d<float>, image1d<float> > histos_middle = world::binary_2d::projected_histo(ima | middle_box);

  box2d right_box = make::box2d(0, (2 * ima.ncols() / 3), ima.nrows() - 1, ima.ncols() - 1);
  util::couple<image1d<float>, image1d<float> > histos_right = world::binary_2d::projected_histo(ima | right_box);

  image1d<float> row_histo = histos_right.first();
  image1d<float> col_histo = histos_right.second();

  int i = col_histo.ninds();
  while (i > 0 && col_histo.at_(i) < 60.0)
  {
    col_histo.at_(i) = 100.0;
    --i;
  }

  // Closing
  row_histo = morpho::closing::volume(row_histo, c2(), 31);

  // Watershed
  label_16 nbasins;
  image1d<label_16> row_labels = morpho::watershed::flooding(row_histo, c2(), nbasins);

  // Draw lines
  for (unsigned int i = 0; i < row_labels.ninds(); ++i)
  {
    if (row_labels.at_(i) == 0u)
    {
      algebra::vec<2, unsigned int> vmin;
      algebra::vec<2, unsigned int> vmax;
      vmin[0] = i;
      vmin[1] = 0;
      vmax[0] = i;
      vmax[1] = ima.ncols() - 1;
      mln_site_(image2d<bool>) pbeg(vmin);
      mln_site_(image2d<bool>) pend(vmax);
      draw::line(ima, pbeg, pend, 0);
    }
  }
  io::pbm::save(ima, argv[2]);

  // Gnuplot files creation
  std::ofstream fout_row("row.plot");
  for (unsigned int i = 0; i < row_histo.ninds(); ++i)
    fout_row << i << " " << row_histo.at_(i) << std::endl;

  std::ofstream fout_col("col.plot");
  for (unsigned int i = 0; i < col_histo.ninds(); ++i)
    fout_col << i << " " << col_histo.at_(i) << std::endl;

  return 0;
}
