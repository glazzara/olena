// Copyright (C) 2007,2008,2009,2010 EPITA LRDE
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#include <mln/core/alias/box3d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/core/alias/point3d.hh>
#include <mln/core/alias/w_window2d_int.hh>
#include <mln/core/alias/w_window3d_int.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/routine/initialize.hh>

#include <mln/data/fill.hh>

#include <mln/io/pgm/save.hh>
#include <mln/io/plot/save_image_sh.hh>

#include <mln/literal/zero.hh>

#include <mln/make/w_window2d_int.hh>
#include <mln/make/w_window3d_int.hh>

#include <mln/opt/at.hh>

#include <mln/transform/influence_zone_geodesic.hh>
#include <mln/transform/influence_zone_front.hh>

#include <mln/util/array.hh>
#include <mln/util/timer.hh>

#include <mln/value/label_8.hh>


/// Build inputs, build seeds, count distance.
/// \{
/// \brief Build 2d/3d input image and 2d/3d seed image.
///
/// Building seeds means making a rectangular grid over a virtual image with
/// a specific size and collects together in a array each node of this grid.
/// In the context of the kmean algorithm, seeds are equavalent to centers.
/// Building input means creating a label image and associates to each seed
/// a unique label id.
/// Distance are square euclidian distance in 2d and in 3d.
mln::image2d<mln::value::label_8> build_input2d(unsigned nb_seed, unsigned size)
{
  typedef mln::value::label_8 t_lbl;
  mln::image2d<t_lbl> input(mln::box2d(mln::point2d(0,0),
				       mln::point2d(size,size)));

  mln::data::fill(input, mln::literal::zero);

  unsigned top = size / nb_seed;
  unsigned lbl = 0;

  for (unsigned i = top/2; i < size; i += top)
    for (unsigned j = top/2; j < size; j += top)
      input(mln::point2d(i,j)) = ++lbl;

  //mln::io::pgm::save(input, "input.pgm");

  return input;
}

mln::image3d<mln::value::label_8> build_input3d(unsigned nb_seed, unsigned size)
{
  typedef mln::value::label_8 t_lbl;
  mln::image3d<t_lbl> input(mln::box3d(mln::point3d(0,0,0),
				       mln::point3d(size,size,size)));

  mln::data::fill(input, mln::literal::zero);

  unsigned top = size / nb_seed;
  unsigned lbl = 0;

  for (unsigned i = top/2; i < size; i += top)
    for (unsigned j = top/2; j < size; j += top)
      for (unsigned k = top/2; k < size; k += top)
	input(mln::point3d(k,i,j)) = ++lbl;

  //mln::io::pgm::save(input, "input.pgm");

  return input;
}

mln::util::array<mln::point3d> build_seed3d(unsigned nb_seed, unsigned size)
{
  mln::util::array<mln::point3d> result;
  unsigned top = size / nb_seed;

  for (unsigned i = top/2; i < size; i += top)
    for (unsigned j = top/2; j < size; j += top)
      for (unsigned k = top/2; k < size; k += top)
	result.append(mln::point3d(k,i,j));

  // std::cout << result << std::endl;

  return result;
}

mln::util::array<mln::point2d> build_seed2d(unsigned nb_seed, unsigned size)
{
  mln::util::array<mln::point2d> result;
  unsigned top = size / nb_seed;

  for (unsigned i = top/2; i < size; i += top)
    for (unsigned j = top/2; j < size; j += top)
	result.append(mln::point2d(j,i));

  // std::cout << result << std::endl;

  return result;
}

unsigned distance(mln::point3d p1, mln::point3d p2)
{
  unsigned row2 = (p1.row() - p2.row())*(p1.row() - p2.row());
  unsigned col2 = (p1.col() - p2.col())*(p1.col() - p2.col());
  unsigned sli2 = (p1.sli() - p2.sli())*(p1.sli() - p2.sli());
  unsigned res  = row2 + col2 + sli2;

  return res;
}

unsigned distance(mln::point2d p1, mln::point2d p2)
{
  unsigned row2 = (p1.row() - p2.row())*(p1.row() - p2.row());
  unsigned col2 = (p1.col() - p2.col())*(p1.col() - p2.col());
  unsigned res  = row2 + col2;

  return res;
}
/// \}

/// Benchmark geodesic distance.
/// \{
/// \brief The geodesic material for the benchmark process.
///
/// This part of the file contains all we need to benchmarck the
/// geodesic distance. First of all, we have the calling functions
/// that encapsulate the real call to geodesic routine. They bring us
/// a clean place where starting and stopping the timer without any
/// other code interference. Printing results could be done at this step.
/// At least, we have the test routine that call the previous functions
/// by passing arguments like different size of inputs and different
/// neighbouring. The test routines save their results in gnuplot script shell
/// file that enable to show charts.
void influence_zone_geodesic_2d(mln::util::timer&                        timer,
				const mln::image2d<mln::value::label_8>& input,
				const mln::neighb2d                    & neighb)
{
  mln::image2d<mln::value::label_8> output;

  timer.start();
  output = mln::transform::influence_zone_geodesic(input, neighb);
  timer.stop();

  //mln::io::pgm::save(output, "output.pgm");
}

/// \fixme influence_zone_geodesic_3d doesn't compile!
void influence_zone_geodesic_3d(mln::util::timer&                        timer,
				const mln::image3d<mln::value::label_8>& input,
				const mln::neighb3d                    & neighb)
{
  mln::image3d<mln::value::label_8> output;

  timer.start();
  // FIXME : Problem with the influence_zone_geodesic in 3d (bad call to at_).
  // output = mln::transform::influence_zone_geodesic(input, neighb);
  timer.stop();

  //mln::io::pgm::save(output, "output.pgm");
}


int test_influence_zone_geodesic_2d()
{
  mln::util::timer timer;

// Test the label c4
  mln::image1d<float> chart_seed_c04(16*16+2);

  mln::data::fill(chart_seed_c04, mln::literal::zero);

  for (unsigned i = 1; i < 16+1; ++i)
  {
    influence_zone_geodesic_2d(timer,build_input2d(i,256),mln::c4());
    std::cout << "c04|256x256|" << i << " = " << timer.read() << std::endl;
    mln::opt::at(chart_seed_c04, i*i) = timer.read();
  }

  mln::io::plot::save_image_sh(chart_seed_c04, "geodesic.c04.2d.seed.sh");

  // Test the label c8
  mln::image1d<float> chart_seed_c08(16*16+2);

  mln::data::fill(chart_seed_c08, mln::literal::zero);

  for (unsigned i = 1; i < 16+1; ++i)
  {
    influence_zone_geodesic_2d(timer,build_input2d(i,256),mln::c8());
    std::cout << "c08|256x256|" << i << " = " << timer.read() << std::endl;
    mln::opt::at(chart_seed_c08, i*i) = timer.read();
  }

  mln::io::plot::save_image_sh(chart_seed_c08, "geodesic.c08.2d.seed.sh");

  // Test the size of the image
  // 1, 2, 4, 8, 16, 32, 64, 128, 256
  mln::image1d<float> chart_size_c04(256+1);

  mln::data::fill(chart_size_c04, mln::literal::zero);

  for (unsigned i = 16; i < 256+1; ++i)
  {
    influence_zone_geodesic_2d(timer, build_input2d(4,i),mln::c4());
    std::cout << "c04|" << i  << "x" << i << "|16 = "
	      << timer.read() << std::endl;
    mln::opt::at(chart_size_c04, i) = timer.read();
  }

  mln::io::plot::save_image_sh(chart_size_c04, "geodesic.c04.2d.size.sh");

  // Test the size of the image
  // 1, 2, 4, 8, 16, 32, 64, 128, 256
  mln::image1d<float> chart_size_c08(256+1);

  mln::data::fill(chart_size_c08, mln::literal::zero);

  for (unsigned i = 16; i < 256+1; ++i)
  {
    influence_zone_geodesic_2d(timer, build_input2d(4,i),mln::c8());
    std::cout << "c08|" << i  << "x" << i << "|16 = "
	      << timer.read() << std::endl;
    mln::opt::at(chart_size_c08, i) = timer.read();
  }

  mln::io::plot::save_image_sh(chart_size_c08, "geodesic.c08.2d.size.sh");


  return 0;
}


void test_influence_zone_geodesic_3d()
{
  mln::util::timer timer;

  // Test the number of labels c06
  mln::image1d<float> chart_seed_c06(5*5*5+2);

  mln::data::fill(chart_seed_c06, mln::literal::zero);

  for (unsigned i = 1; i < 5+1; ++i)
  {
    influence_zone_geodesic_3d(timer, build_input3d(i,128), mln::c6());
    std::cout << "c06|128x128x128|" << i << " = " << timer.read() << std::endl;
    mln::opt::at(chart_seed_c06, i*i*i) = timer.read();
  }

  mln::io::plot::save_image_sh(chart_seed_c06, "geodesic.c06.3d.seed.sh");

  // Test the number of labels c18
  mln::image1d<float> chart_seed_c18(5*5*5+2);

  mln::data::fill(chart_seed_c18, mln::literal::zero);

  for (unsigned i = 1; i < 5+1; ++i)
  {
    influence_zone_geodesic_3d(timer, build_input3d(i,128), mln::c18());
    std::cout << "c18|128x128x128|" << i << " = " << timer.read() << std::endl;
    mln::opt::at(chart_seed_c18, i*i*i) = timer.read();
  }

  mln::io::plot::save_image_sh(chart_seed_c18, "geodesic.c18.3d.seed.sh");

  // Test the number of labels c26
  mln::image1d<float> chart_seed_c26(5*5*5+2);

  mln::data::fill(chart_seed_c26, mln::literal::zero);

  for (unsigned i = 1; i < 5+1; ++i)
  {
    influence_zone_geodesic_3d(timer, build_input3d(i,128), mln::c26());
    std::cout << "c26|128x128x128|" << i << " = " << timer.read() << std::endl;
    mln::opt::at(chart_seed_c26, i*i*i) = timer.read();
  }

  mln::io::plot::save_image_sh(chart_seed_c26, "geodesic.c26.3d.seed.sh");

  // Test the size of the image c06
  // 1, 2, 4, 8, 16, 32, 64, 128, 256
  mln::image1d<float> chart_size_c06(128+1);

  mln::data::fill(chart_size_c06, mln::literal::zero);

  for (unsigned i = 6; i < 128+1; ++i)
  {
    influence_zone_geodesic_3d(timer, build_input3d(2,i), mln::c6());
    std::cout << "c06|" << i  << "x" << i << "x" << i << "|8 = "
	      << timer.read() << std::endl;
    mln::opt::at(chart_size_c06, i) = timer.read();
  }

  mln::io::plot::save_image_sh(chart_size_c06, "geodesic.c06.3d.size.sh");

  // Test the size of the image c18
  // 1, 2, 4, 8, 16, 32, 64, 128, 256
  mln::image1d<float> chart_size_c18(128+1);

  mln::data::fill(chart_size_c18, mln::literal::zero);

  for (unsigned i = 6; i < 128+1; ++i)
  {
    influence_zone_geodesic_3d(timer, build_input3d(2,i), mln::c18());
    std::cout << "c18|" << i  << "x" << i << "x" << i << "|8 = "
	      << timer.read() << std::endl;
    mln::opt::at(chart_size_c18, i) = timer.read();
  }

  mln::io::plot::save_image_sh(chart_size_c18, "geodesic.c18.3d.size.sh");

  // Test the size of the image c26
  // 1, 2, 4, 8, 16, 32, 64, 128, 256
  mln::image1d<float> chart_size_c26(128+1);

  mln::data::fill(chart_size_c26, mln::literal::zero);

  for (unsigned i = 6; i < 128+1; ++i)
  {
    influence_zone_geodesic_3d(timer, build_input3d(2,i), mln::c26());
    std::cout << "c26|" << i  << "x" << i << "x" << i << "|8 = "
	      << timer.read() << std::endl;
    mln::opt::at(chart_size_c26, i) = timer.read();
  }

  mln::io::plot::save_image_sh(chart_size_c26, "geodesic.c26.3d.size.sh");
}
/// \}

/// Benchmark front distance.
/// \{
/// \brief The front material for the benchmark process.
///
/// This part of the file contains all we need to benchmarck the
/// front distance. First of all, we have the calling functions
/// that encapsulate the real call to geodesic routine. They bring us
/// a clean place where starting and stopping the timer without any
/// other code interference. Printing results could be done at this step.
/// At least, we have the test routine that call the previous functions
/// by passing arguments like different size of inputs and different
/// neighbouring. The test routines save their results in gnuplot script shell
/// file that enable to show charts.
void influence_zone_front_2d(mln::util::timer&                        timer,
			     const mln::image2d<mln::value::label_8>& input,
			     const mln::neighb2d&		      neighb,
			     const mln::w_window2d_int&               w2d)
{
  mln::image2d<mln::value::label_8> output;

  timer.start();
  output = mln::transform::influence_zone_front(input, neighb, w2d);
  timer.stop();

  //mln::io::pgm::save(output, "output.pgm");
}


void influence_zone_front_3d(mln::util::timer&                        timer,
			     const mln::image3d<mln::value::label_8>& input,
			     const mln::neighb3d&		      neighb,
			     const mln::w_window3d_int&               w3d)
{
  mln::image3d<mln::value::label_8> output;

  timer.start();
  output = mln::transform::influence_zone_front(input, neighb, w3d);
  timer.stop();

  //mln::io::pgm::save(output, "output.pgm");
}


void test_influence_zone_front_3d()
{
  mln::util::timer timer;

  int ws_c06[] = {
    // Internal slice
    0, 0, 0,
    0, 2, 0,
    0, 0, 0,
    // Middle slice
    0, 2, 0,
    2, 0, 2,
    0, 2, 0,
    // External slice
    0, 0, 0,
    0, 2, 0,
    0, 0, 0 };

  int ws_c18[] = {
    // Internal slice
    0, 6, 0,
    6, 4, 6,
    0, 6, 0,
    // Middle slice
    6, 4, 6,
    4, 0, 4,
    6, 4, 6,
    // External slice
    0, 6, 0,
    6, 4, 6,
    0, 6, 0 };

  int ws_c26[] = {
    // Internal slice
    7, 6, 7,
    6, 4, 6,
    7, 6, 7,
    // Middle slice
    6, 4, 6,
    4, 0, 4,
    6, 4, 6,
    // External slice
    7, 6, 7,
    6, 4, 6,
    7, 6, 7 };

  mln::w_window3d_int w3d_c06 = mln::make::w_window3d_int(ws_c06);
  mln::w_window3d_int w3d_c18 = mln::make::w_window3d_int(ws_c18);
  mln::w_window3d_int w3d_c26 = mln::make::w_window3d_int(ws_c26);

  // Test the number of labels c06
  mln::image1d<float> chart_seed_c06(5*5*5+2);

  mln::data::fill(chart_seed_c06, mln::literal::zero);

  for (unsigned i = 1; i < 5+1; ++i)
  {
    influence_zone_front_3d(timer, build_input3d(i,128), mln::c6(), w3d_c06);
    std::cout << "c06|128x128x128|" << i << " = " << timer.read() << std::endl;
    mln::opt::at(chart_seed_c06, i*i*i) = timer.read();
  }

  mln::io::plot::save_image_sh(chart_seed_c06, "front.c06.3d.seed.sh");

  // Test the number of labels c18
  mln::image1d<float> chart_seed_c18(5*5*5+2);

  mln::data::fill(chart_seed_c18, mln::literal::zero);

  for (unsigned i = 1; i < 5+1; ++i)
  {
    influence_zone_front_3d(timer, build_input3d(i,128), mln::c18(), w3d_c18);
    std::cout << "c18|128x128x128|" << i << " = " << timer.read() << std::endl;
    mln::opt::at(chart_seed_c18, i*i*i) = timer.read();
  }

  mln::io::plot::save_image_sh(chart_seed_c18, "front.c18.3d.seed.sh");

  // Test the number of labels c26
  mln::image1d<float> chart_seed_c26(5*5*5+2);

  mln::data::fill(chart_seed_c26, mln::literal::zero);

  for (unsigned i = 1; i < 5+1; ++i)
  {
    influence_zone_front_3d(timer, build_input3d(i,128), mln::c26(), w3d_c26);
    std::cout << "c26|128x128x128|" << i << " = " << timer.read() << std::endl;
    mln::opt::at(chart_seed_c26, i*i*i) = timer.read();
  }

  mln::io::plot::save_image_sh(chart_seed_c26, "front.c26.3d.seed.sh");

  // Test the size of the image c06
  // 1, 2, 4, 8, 16, 32, 64, 128, 256
  mln::image1d<float> chart_size_c06(128+1);

  mln::data::fill(chart_size_c06, mln::literal::zero);

  for (unsigned i = 6; i < 128+1; ++i)
  {
    influence_zone_front_3d(timer, build_input3d(2,i), mln::c6(), w3d_c06);
    std::cout << "c06|" << i  << "x" << i << "x" << i << "|8 = "
	      << timer.read() << std::endl;
    mln::opt::at(chart_size_c06, i) = timer.read();
  }

  mln::io::plot::save_image_sh(chart_size_c06, "front.c06.3d.size.sh");

  // Test the size of the image c18
  // 1, 2, 4, 8, 16, 32, 64, 128, 256
  mln::image1d<float> chart_size_c18(128+1);

  mln::data::fill(chart_size_c18, mln::literal::zero);

  for (unsigned i = 6; i < 128+1; ++i)
  {
    influence_zone_front_3d(timer, build_input3d(2,i), mln::c18(), w3d_c18);
    std::cout << "c18|" << i  << "x" << i << "x" << i << "|8 = "
	      << timer.read() << std::endl;
    mln::opt::at(chart_size_c18, i) = timer.read();
  }

  mln::io::plot::save_image_sh(chart_size_c18, "front.c18.3d.size.sh");

  // Test the size of the image c26
  // 1, 2, 4, 8, 16, 32, 64, 128, 256
  mln::image1d<float> chart_size_c26(128+1);

  mln::data::fill(chart_size_c26, mln::literal::zero);

  for (unsigned i = 6; i < 128+1; ++i)
  {
    influence_zone_front_3d(timer, build_input3d(2,i), mln::c26(), w3d_c26);
    std::cout << "c26|" << i  << "x" << i << "x" << i << "|8 = "
	      << timer.read() << std::endl;
    mln::opt::at(chart_size_c26, i) = timer.read();
  }

  mln::io::plot::save_image_sh(chart_size_c26, "front.c26.3d.size.sh");
}

void test_influence_zone_front_2d()
{
  mln::util::timer timer;

  int ws_c04[] = {
    0, 2, 0,
    2, 0, 2,
    0, 2, 0};

  int ws_c08[] = {
    6, 4, 6,
    4, 0, 4,
    6, 4, 6};

  mln::w_window2d_int w2d_c04 = mln::make::w_window2d_int(ws_c04);
  mln::w_window2d_int w2d_c08 = mln::make::w_window2d_int(ws_c08);

  // Test the label c4
  mln::image1d<float> chart_seed_c04(16*16+2);

  mln::data::fill(chart_seed_c04, mln::literal::zero);

  for (unsigned i = 1; i < 16+1; ++i)
  {
    influence_zone_front_2d(timer,build_input2d(i,256),mln::c4(),w2d_c04);
    std::cout << "c04|256x256|" << i << " = " << timer.read() << std::endl;
    mln::opt::at(chart_seed_c04, i*i) = timer.read();
  }

  mln::io::plot::save_image_sh(chart_seed_c04, "front.c04.2d.seed.sh");

  // Test the label c8
  mln::image1d<float> chart_seed_c08(16*16+2);

  mln::data::fill(chart_seed_c08, mln::literal::zero);

  for (unsigned i = 1; i < 16+1; ++i)
  {
    influence_zone_front_2d(timer,build_input2d(i,256),mln::c8(),w2d_c08);
    std::cout << "c08|256x256|" << i << " = " << timer.read() << std::endl;
    mln::opt::at(chart_seed_c08, i*i) = timer.read();
  }

  mln::io::plot::save_image_sh(chart_seed_c08, "front.c08.2d.seed.sh");

  // Test the size of the image
  // 1, 2, 4, 8, 16, 32, 64, 128, 256
  mln::image1d<float> chart_size_c04(256+1);

  mln::data::fill(chart_size_c04, mln::literal::zero);

  for (unsigned i = 16; i < 256+1; ++i)
  {
    influence_zone_front_2d(timer, build_input2d(4,i),mln::c4(),w2d_c04);
    std::cout << "c04|" << i  << "x" << i << "|16 = "
	      << timer.read() << std::endl;
    mln::opt::at(chart_size_c04, i) = timer.read();
  }

  mln::io::plot::save_image_sh(chart_size_c04, "front.c04.2d.size.sh");

  // Test the size of the image
  // 1, 2, 4, 8, 16, 32, 64, 128, 256
  mln::image1d<float> chart_size_c08(256+1);

  mln::data::fill(chart_size_c08, mln::literal::zero);

  for (unsigned i = 16; i < 256+1; ++i)
  {
    influence_zone_front_2d(timer, build_input2d(4,i),mln::c8(),w2d_c08);
    std::cout << "c08|" << i  << "x" << i << "|16 = "
	      << timer.read() << std::endl;
    mln::opt::at(chart_size_c08, i) = timer.read();
  }

  mln::io::plot::save_image_sh(chart_size_c08, "front.c08.2d.size.sh");
}
/// \}



/// Benchmark euclidian distance.
/// \{
/// \brief The euclidian material for the benchmark process.
///
/// This part of the file contains all we need to benchmarck the
/// euclidian distance. First of all, we have the calling functions
/// that encapsulate the real call to geodesic routine. They bring us
/// a clean place where starting and stopping the timer without any
/// other code interference. Printing results could be done at this step.
/// At least, we have the test routine that call the previous functions
/// by passing arguments like different size of inputs and different
/// neighbouring. The test routines save their results in gnuplot script shell
/// file that enable to show charts.
void influence_zone_euclidian_2d(mln::util::timer&                        timer,
				 const mln::image2d<mln::value::label_8>& input,
				 const mln::util::array<mln::point2d>&    seed)
{
  mln::image2d<mln::value::label_8> output;

  timer.start();

  mln::initialize(output, input);

  mln_piter_(mln::image2d<mln::value::label_8>) p(output.domain());
  mln_eiter_(mln::util::array<mln::point2d>)   	e(seed);

  for_all(p)
  {
    unsigned d     = 0;
    unsigned min_d = mln_max(unsigned);
    unsigned min_l = 0;

    for_all(e)
    {
      d = distance(p, seed(e.index_()));

      if (min_d > d)
      {
	min_d = d;
	min_l = input(e);
      }
    }

    output(p) = min_l;
  }

  timer.stop();

  //mln::io::pgm::save(output, "output.pgm");
}


void influence_zone_euclidian_3d(mln::util::timer&                        timer,
				 const mln::image3d<mln::value::label_8>& input,
				 const mln::util::array<mln::point3d>&    seed)
{
  mln::image3d<mln::value::label_8> output;

  timer.start();

  mln::initialize(output, input);

  mln_piter_(mln::image3d<mln::value::label_8>) p(output.domain());
  mln_eiter_(mln::util::array<mln::point3d>)   	e(seed);

  for_all(p)
  {
    unsigned d     = 0;
    unsigned min_d = mln_max(unsigned);
    unsigned min_l = 0;

    for_all(e)
    {
      d = distance(p, seed(e.index_()));

      if (min_d > d)
      {
	min_d = d;
	min_l = input(e);
      }
    }

    output(p) = min_l;
  }

  timer.stop();
}

void test_influence_zone_euclidian_2d()
{
  mln::util::timer timer;
/*
  // Global test
  mln::image2d<float> chart(mln::box2d(mln::point2d(0,0),
				       mln::point2d(16*16+2,256+1)));

  mln::data::fill(chart, mln::literal::zero);

  for (unsigned i = 1; i < 256+1; ++i) // size
    for (unsigned j = 1; j < i*i && j < 16+1; ++j) // seed
    {
      influence_zone_euclidian_2d(timer,build_input2d(j,i),build_seed2d(j,i));
      std::cout << "xxx|" << i << "x" << i << "|" << j << " = "
		<< timer.read() << std::endl;
      mln::opt::at(chart, j*j,i) = timer.read();
    }

  mln::io::plot::save_image_sh(chart, "chart.sh");
*/

  // Test the number of labels
  mln::image1d<float> chart_seed(16*16+2);

  mln::data::fill(chart_seed, mln::literal::zero);

  for (unsigned i = 1; i < 16+1; ++i)
  {
    influence_zone_euclidian_2d(timer,build_input2d(i,256),build_seed2d(i,256));
    std::cout << "xxx|256x256|" << i << " = " << timer.read() << std::endl;
    mln::opt::at(chart_seed, i*i) = timer.read();
  }

  mln::io::plot::save_image_sh(chart_seed, "euclidian.2d.seed.sh");

  // Test the size of the image
  // 1, 2, 4, 8, 16, 32, 64, 128, 256
  mln::image1d<float> chart_size(256+1);

  mln::data::fill(chart_size, mln::literal::zero);

  for (unsigned i = 16; i < 256+1; ++i)
  {
    influence_zone_euclidian_2d(timer, build_input2d(4,i), build_seed2d(4,i));
    std::cout << "xxx|" << i  << "x" << i << "|16 = "
	      << timer.read() << std::endl;
    mln::opt::at(chart_size, i) = timer.read();
  }

  mln::io::plot::save_image_sh(chart_size, "euclidian.2d.size.sh");
}

void test_influence_zone_euclidian_3d()
{
  mln::util::timer timer;

  // Test the number of labels
  mln::image1d<float> chart_seed(5*5*5+2);

  mln::data::fill(chart_seed, mln::literal::zero);

  for (unsigned i = 1; i < 5+1; ++i)
  {
    influence_zone_euclidian_3d(timer,build_input3d(i,128),build_seed3d(i,128));
    std::cout << "xxx|128x128x128|" << i << " = " << timer.read() << std::endl;
    mln::opt::at(chart_seed, i*i*i) = timer.read();
  }

  mln::io::plot::save_image_sh(chart_seed, "euclidian.3d.seed.sh");

  // Test the size of the image
  // 1, 2, 4, 8, 16, 32, 64, 128, 256
  mln::image1d<float> chart_size(128+1);

  mln::data::fill(chart_size, mln::literal::zero);

  for (unsigned i = 6; i < 128+1; ++i)
  {
    influence_zone_euclidian_3d(timer, build_input3d(2,i), build_seed3d(2,i));
    std::cout << "xxx|" << i  << "x" << i << "x" << i << "|8 = "
	      << timer.read() << std::endl;
    mln::opt::at(chart_size, i) = timer.read();
  }

  mln::io::plot::save_image_sh(chart_size, "euclidian.3d.size.sh");
}
/// \}

//------------------------------------------------------------------------------
// Main
//
// Point de fonctionnement du système [image de taille entre 1 et 256]
// Moins de 16 labels
//
//------------------------------------------------------------------------------

/// \brief The main routine that call all the benchmark tests.
///
/// This benchmark aim at comparing different kind of distance. It
/// looks after the influence geodesic, the influence front and
/// euclidian distance. Many files are generated. Some rules exist to
/// build the name of the file:
/// distance.neighbouring.dimension.(size|seed). The distance could be
/// the front distance (front), the geodesic distance (geodesic) or
/// the euclidian distance (euclidian). The neighbouring could be c04
/// or c08 in 2d, c06, c18 or c26 in 3d. The dimension could be 2d or 3d.
/// Finally, the flag size or seed precises the nature of that test, increasing
/// gradualy the size of the image or increasing gradualy the number of seeds.
/// The file contains the time benchmark of the distance studied.
///
/// \fixme test_influence_zone_geodesic_3d doesn't compile.
int main()
{
  test_influence_zone_euclidian_2d();
  test_influence_zone_front_2d();
  test_influence_zone_geodesic_2d();

  // FIXME : It doesn't compile because of a bad calling function at_ in
  // FIXME : the influence_zone_geodesic.hh line 127.
  // test_influence_zone_geodesic_3d();
  test_influence_zone_front_3d();
  test_influence_zone_euclidian_3d();

  return 0;
}
