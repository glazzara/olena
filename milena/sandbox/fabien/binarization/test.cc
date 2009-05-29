#include <iostream>
#include <fstream>

#include <mln/accu/median_h.hh>

#include <mln/algebra/vec.hh>

#include <mln/convert/from_to.hh>

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb1d.hh>

#include <mln/draw/line.hh>

#include <mln/io/magick/all.hh>

#include <mln/data/convert.hh>
#include <mln/data/stretch.hh>

#include <mln/literal/colors.hh>

#include <mln/make/box2d.hh>

#include <mln/morpho/closing/volume.hh>
#include <mln/morpho/watershed/flooding.hh>

#include <mln/util/array.hh>
#include <mln/util/couple.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/value/label_16.hh>
#include <mln/value/rgb8.hh>

#include <mln/world/binary_2d/projected_histo.hh>
#include <mln/world/binary_2d/subsample.hh>


using namespace mln;
using value::int_u8;
using value::int_u12;
using value::label_16;
using value::rgb8;


int draw_lines(image2d<bool>& ima, int col_min, int col_max)
{
  box2d box = make::box2d(0, col_min, ima.nrows() - 1, col_max);
  util::couple<image1d<float>, image1d<float> > histos = world::binary_2d::projected_histo(ima | box);
  image1d<float> row_histo = histos.first();

  /*int i = col_histo.ninds();
  while (i > 0 && col_histo.at_(i) < 60.0)
  {
    col_histo.at_(i) = 100.0;
    --i;
  }*/

  row_histo = morpho::closing::volume(row_histo, c2(), 41);

  // Watershed
  label_16 nbasins;
  image1d<label_16> row_labels = morpho::watershed::flooding(row_histo, c2(), nbasins);

  int median = 0;
  util::array<int> inter_lines;
  int begin = 0;
  int end = 0;

  // Draw lines
  for (unsigned int i = 0; i < row_labels.ninds(); ++i)
    if (row_labels.at_(i) == 0u)
    {
      if (end == 0)
      {
	begin = i;
	end = i;
      }
      else if (end == begin)
      {
	end = i;
	inter_lines.append(end - begin);
	begin = end;
      }
    }

  image1d<int> ima_lines;
  convert::from_to(inter_lines, ima_lines);
  accu::median_h<int_u12> accu_med;
  median = data::compute(accu_med, ima_lines);

  // Gnuplot files creation
  /*std::ofstream fout_row("row.plot");
  for (unsigned int i = 0; i < row_histo.ninds(); ++i)
    fout_row << i << " " << row_histo.at_(i) << std::endl;

  std::ofstream fout_col("col.plot");
  for (unsigned int i = 0; i < col_histo.ninds(); ++i)
    fout_col << i << " " << col_histo.at_(i) << std::endl;*/

  return median;
}


int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cout << "Usage: " << argv[0] << "input output" << std::endl;
    return 1;
  }

  image2d<bool> ima;
  io::magick::load(ima, argv[1]);
  image2d<rgb8> ima_color = data::convert(rgb8(), ima);

  //image2d<int_u8> ima_sampled = world::binary_2d::subsample(ima, 3);
  //io::pgm::save(ima_sampled, argv[2]);

  int median_left =  draw_lines(ima, 0, (ima.ncols() / 3));
  int median_middle = draw_lines(ima, (ima.ncols() / 3), (2 * ima.ncols() / 3));
  int median_right = draw_lines(ima, (2 * ima.ncols() / 3), ima.ncols() - 1);

  int median = (median_left + median_middle + median_right) / 3;

  std::cout << "median = " << median << std::endl;

  // Draw lines
  algebra::vec<2, unsigned int> vmin;
  algebra::vec<2, unsigned int> vmax;
  for (unsigned int i = 0; i < ima.nrows(); ++i)
  {
    if (i % median == 0)
    {
      vmin[0] = i;
      vmin[1] = 0;
      vmax[0] = i;
      vmax[1] = ima.ncols() - 1;
      mln_site_(image2d<bool>) pbeg(vmin);
      mln_site_(image2d<bool>) pend(vmax);
      draw::line(ima_color, pbeg, pend, literal::red);
    }
  }

  io::magick::save(ima_color, argv[2]);

  return 0;
}
