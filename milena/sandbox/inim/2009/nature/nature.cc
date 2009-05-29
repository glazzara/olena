// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#include <mln/core/image/image2d.hh>
#include <mln/core/image/vmorph/cast_image.hh>
#include <mln/core/image/thru.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/pbm/save.hh>

#include <mln/histo/compute.hh>

#include <mln/data/transform.hh>
#include <mln/data/paste.hh>
#include <mln/data/to_enc.hh>

#include <mln/value/rgb8.hh>
#include <mln/value/hsi.hh>

#include <mln/fun/v2v/rgb_to_hsi.hh>
#include <mln/fun/meta/hue.hh>
#include <mln/fun/meta/sat.hh>
#include <mln/fun/meta/inty.hh>

#include <mln/win/hline2d.hh>
#include <mln/win/vline2d.hh>

#include <mln/morpho/hit_or_miss.hh>
#include <mln/morpho/gradient.hh>
#include <mln/morpho/opening/structural.hh>

#include <mln/binarization/threshold.hh>

#include <mln/estim/mean.hh>

#include <mln/transform/fft.hh>

#include <mln/opt/at.hh>

#include "co_occurence.hh"

#include <math.h>

using namespace mln;
using namespace value;

enum doctype
  {
    TEXT,
    SCREENSHOT,
    PHOTO,
    DRAWING,
    NB_DOCTYPE
  };

template <typename T>
unsigned nb_peaks (histo::data<T> &h)
{
  // First, we compute the maximum, ignoring the first column because it
  // contains nonsense points

  unsigned max = 0;
  mln_viter(mln::value::set<T>) v(h.vset());

  for_all(v)
    if (((T)v).to_enc() && (h(v) > max))
      max = h(v);

  // We take the first value into account
  if (h[0] / 6 > max)
    max = h[0] / 6;

  // Now we count the number of peaks over max / 4
  unsigned cpt = 0;
  for_all(v)
    if (h(v) >= max / 4)
      cpt ++;

  return cpt;
}


int main (int argc, const char * argv [])
{
  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " in.pgm" << std::endl;
    return 1;
  }

  unsigned score[NB_DOCTYPE];
  for (unsigned i = 0; i < NB_DOCTYPE; ++i)
    score[i] = 0;

  border::thickness = 21;

  image2d<rgb8> input;
  io::ppm::load(input, argv[1]);

  // First, we get the HSI histograms and corresponding greyscale versions of the picture

  image2d<hsi_f> hsi = data::transform(input, fun::v2v::f_rgb_to_hsi_f);

  thru<mln::meta::hue<hsi_f>, image2d<hsi_f> > hue(hsi);
  thru<mln::meta::sat<hsi_f>, image2d<hsi_f> > sat(hsi);
  thru<mln::meta::inty<hsi_f>, image2d<hsi_f> > inty(hsi);

  cast_image_<float01_8, thru<mln::meta::hue<hsi_f>, image2d<hsi_f> > > qhue(hue); // quantified hue
  cast_image_<float01_8, thru<mln::meta::sat<hsi_f>, image2d<hsi_f> > > qsat(sat); // quantified sat
  cast_image_<float01_8, thru<mln::meta::inty<hsi_f>, image2d<hsi_f> > > qinty(inty); // quantified inty

  histo::data<float01_8> hhue = histo::compute(qhue);
  histo::data<float01_8> hsat = histo::compute(qsat);
  histo::data<float01_8> hinty = histo::compute(qinty);


  // Compute the number of peaks in the histogram to give a first intuition

  unsigned n = nb_peaks(hhue) * 3;

  if (n > 100)
    n = 100;

    score[PHOTO] += n;
    score[SCREENSHOT] += 20 + 80 - n * 80 / 100;
    score[TEXT] += 100 - n;
    score[DRAWING] += 20 + 80 - n * 80 / 100;

  std::cout << "Test 1 : peaks number[" << nb_peaks(hhue) << "]" << std::endl
	    << "Photo : " << score[PHOTO] << std::endl
	    << "Screenshot : " << score[SCREENSHOT] << std::endl
	    << "Text : " << score[TEXT] << std::endl
	    << "Drawing : " << score[DRAWING] << std::endl << std::endl;


  // Now we compute the co-occurence matrice

  dpoint2d d(0, 1);
  image2d<int_u8> uinty (qinty.domain());
  {
    image2d<unsigned char> tmp = data::transform(qinty, fun::v2v::enc< float01_8 >());
    data::paste(tmp, uinty);
    /*    mln_piter_(image2d<unsigned char>) p(tmp.domain());
    for_all(p)
      uinty(p) = tmp(p);
    */
  }

  image2d<unsigned> mco (co_occurence(uinty, d));

  // If there is a plain background, there will be a massive peak in the diagonal of the matrix
  unsigned max = 0;
  for (unsigned i = 0; i < mco.nrows(); i++)
    if (opt::at(mco, i, i) > max)
      max = opt::at(mco, i, i);

  for (unsigned i = 0; i < mco.nrows() - 1; i++) {
    if (opt::at(mco, i + 1, i) > max)
      max = opt::at(mco, i + 1, i);
    if (opt::at(mco, i, i + 1) > max)
      max = opt::at(mco, i, i + 1);
  }

  max = max * 100 / input.nelements();

  score[PHOTO] += 100 - max;
  score[TEXT] += max * 2;

  score[SCREENSHOT] += 50 + max / 2;
  score[DRAWING] += 50 + max / 2;

  std::cout << "Test 2 : co-occurence matrix diag_max[" << max << "]" << std::endl
	    << "Photo : +" << 100 - max << std::endl;

  std::cout << "Screenshot : +" << 50 + max / 2 << std::endl;

  std::cout << "Text : +" << max * 2 << std::endl;

  std::cout << "Drawing : +" << 50 + max / 2 << std::endl << std::endl;




  // A good way to detect screenshots : rectangle detection

  // First, we compute the gradient

  win::rectangle2d rect(5, 5);
  image2d<int_u8> grad = morpho::gradient(uinty, rect);

  // Then, we apply a lot of hit or miss filters !

  // Compute the mean
  int_u8 mean = estim::mean(grad);

  image2d<bool> imab = binarization::threshold(grad, 10);

  window2d winout;
  window2d winin;

  static const bool blmatout [] = {0, 0, 0, 0, 0,
				   0, 1, 0, 0, 0,
				   0, 1, 0, 0, 0,
				   0, 1, 1, 1, 0,
				   0, 0, 0, 0, 0};

  convert::from_to(blmatout, winout);

  static const bool blmatin [] = {0, 0, 1, 0, 0,
				  0, 0, 1, 0, 0,
				  0, 0, 1, 1, 1,
				  0, 0, 0, 0, 0,
				  0, 0, 0, 0, 0};

  convert::from_to(blmatin, winin);

  image2d<bool> bottom_left = morpho::hit_or_miss(imab, winout, winin);


  static const bool brmatout [] = {0, 0, 0, 0, 0,
				   0, 0, 0, 1, 0,
				   0, 0, 0, 1, 0,
				   0, 1, 1, 1, 0,
				   0, 0, 0, 0, 0};

  static const bool brmatin [] = {0, 0, 1, 0, 0,
				  0, 0, 1, 0, 0,
				  1, 1, 1, 0, 0,
				  0, 0, 0, 0, 0,
				  0, 0, 0, 0, 0};


  convert::from_to(brmatout, winout);
  convert::from_to(brmatin, winin);
  image2d<bool> bottom_right = morpho::hit_or_miss(imab, winout, winin);

  static const bool urmatout [] = {0, 0, 0, 0, 0,
				   0, 1, 1, 1, 0,
				   0, 0, 0, 1, 0,
				   0, 0, 0, 1, 0,
				   0, 0, 0, 0, 0};

  static const bool urmatin [] = {0, 0, 0, 0, 0,
				  0, 0, 0, 0, 0,
				  1, 1, 1, 0, 0,
				  0, 0, 1, 0, 0,
				  0, 0, 1, 0, 0};


  convert::from_to(urmatout, winout);
  convert::from_to(urmatin, winin);
  image2d<bool> up_right = morpho::hit_or_miss(imab, winout, winin);

  static const bool ulmatout [] = {0, 0, 0, 0, 0,
				   0, 1, 1, 1, 0,
				   0, 1, 0, 0, 0,
				   0, 1, 0, 0, 0,
				   0, 0, 0, 0, 0};

  static const bool ulmatin [] = {0, 0, 0, 0, 0,
				  0, 0, 0, 0, 0,
				  0, 0, 1, 1, 1,
				  0, 0, 1, 0, 0,
				  0, 0, 1, 0, 0};


  convert::from_to(ulmatout, winout);
  convert::from_to(ulmatin, winin);
  image2d<bool> up_left = morpho::hit_or_miss(imab, winout, winin);


  static const bool umatout [] = {0, 0, 0, 0, 0,
				  1, 1, 1, 1, 1,
				  0, 0, 0, 0, 0,
				  0, 0, 0, 0, 0,
				  0, 0, 0, 0, 0};

  static const bool umatin [] = {0, 0, 0, 0, 0,
				 0, 0, 0, 0, 0,
				 0, 1, 1, 1, 0,
				 0, 0, 0, 0, 0,
				 0, 0, 0, 0, 0};


  convert::from_to(umatout, winout);
  convert::from_to(umatin, winin);
  image2d<bool> up = morpho::hit_or_miss(imab, winout, winin);


  static const bool bmatout [] = {0, 0, 0, 0, 0,
				  0, 0, 0, 0, 0,
				  0, 0, 0, 0, 0,
				  1, 1, 1, 1, 1,
				  0, 0, 0, 0, 0};

  static const bool bmatin [] = {0, 0, 0, 0, 0,
				 0, 0, 0, 0, 0,
				 0, 1, 1, 1, 0,
				 0, 0, 0, 0, 0,
				 0, 0, 0, 0, 0};


  convert::from_to(bmatout, winout);
  convert::from_to(bmatin, winin);
  image2d<bool> bottom = morpho::hit_or_miss(imab, winout, winin);


  static const bool lmatout [] = {0, 1, 0, 0, 0,
				  0, 1, 0, 0, 0,
				  0, 1, 0, 0, 0,
				  0, 1, 0, 0, 0,
				  0, 1, 0, 0, 0};

  static const bool lmatin [] = {0, 0, 0, 0, 0,
				 0, 0, 1, 0, 0,
				 0, 0, 1, 0, 0,
				 0, 0, 1, 0, 0,
				 0, 0, 0, 0, 0};


  convert::from_to(lmatout, winout);
  convert::from_to(lmatin, winin);
  image2d<bool> left = morpho::hit_or_miss(imab, winout, winin);


  static const bool rmatout [] = {0, 0, 0, 1, 0,
				  0, 0, 0, 1, 0,
				  0, 0, 0, 1, 0,
				  0, 0, 0, 1, 0,
				  0, 0, 0, 1, 0};

  static const bool rmatin [] = {0, 0, 0, 0, 0,
				 0, 0, 1, 0, 0,
				 0, 0, 1, 0, 0,
				 0, 0, 1, 0, 0,
				 0, 0, 0, 0, 0};


  convert::from_to(rmatout, winout);
  convert::from_to(rmatin, winin);
  image2d<bool> right = morpho::hit_or_miss(imab, winout, winin);

  image2d<bool> inter (up_left + up_right + bottom_right + bottom_left + up + bottom + left + right);

  image2d<bool> final = morpho::opening::structural(inter, win::hline2d(20));
  final += morpho::opening::structural(inter, win::vline2d(20));

  // Now we count blank points in our result

  unsigned cpt = 0;
  mln_piter_(image2d<bool>) p (final.domain());
  for_all(p)
    cpt += final(p);

  // compute the percentage of blank points

  cpt = 1000 * cpt / final.domain().nsites();

  if (cpt > 50)
    cpt = 50;

  score[PHOTO] += 50 - cpt;
  score[SCREENSHOT] += cpt * 5;
  score[TEXT] += 50 + cpt * max / 10;
  score[DRAWING] += 2 * (50 - cpt);

  std::cout << "Test 3 : rectangle detection[" << cpt << "]" << std::endl
	    << "Photo : +" << 50 - cpt << std::endl
	    << "Screenshot : +" << cpt * 8 << std::endl
	    << "Text : +" << 50 + cpt * max / 10 << std::endl
	    << "Drawing : +" << 2 * (50 - cpt) << std::endl << std::endl;


  // Last but not least : text detection thanks to FFT

  transform::fft<double> fourier(morpho::opening::structural(uinty, win::hline2d(20)));
  fourier.transform();

  std::string name(argv[1]);
  name.erase(name.length() - 4);
  image2d<int_u8> fft = fourier.transformed_image_log_magn<int_u8>(true);
  mln_piter_(image2d<int_u8>) q (fft.domain());
  max = 0;
  for_all(q)
    if (fft(q) > max)
      max = fft(q);

  image2d<bool> fftb = binarization::threshold(fft, max * 4 / 10);

  // Find the line that best fits the points

  unsigned
    center_r = fft.nrows() / 2,
    center_c = fft.ncols() / 2;

  double arg = 0, coeff = 0;

  for_all(q)
    if (fftb(q))
      {
	double dist =
	  (center_c - q.col()) * (center_c - q.col()) +
	  (center_r - q.row()) * (center_r - q.row());

	if (!dist)
	  continue;

	double angle = acos (abs(center_r - q.row()) / sqrt(dist));
	while (angle > M_PI)
	  angle -= M_PI;
	while (angle < 0)
	  angle += M_PI;

	arg = (arg * coeff + dist * angle) / (coeff + dist);

	coeff += dist;

      }

  //  std::cout << "arg : " << arg << " coeff : " << coeff << std::endl;
  double a  = tan(arg);

  // Now compute the mean distance of the points to the line

  double b = center_c - a * center_r;
  double dist = 0;
  coeff = 0;

  for_all(q)
    if (fftb(q))
      {
	double d = abs(a * q.row() - q.col() + b) / sqrt(a * a + 1);
	dist += d * d;
	coeff ++;
      }

  dist /= coeff;

  dist = dist / (log(fftb.nelements()) / log(10));

  score[PHOTO] += dist * 25;
  score[SCREENSHOT] += dist * 25;
  score[TEXT] += (1 + cos(arg)) * 2 * (4 - dist) * 50;
  score[DRAWING] += dist * 25;

  std::cout << "Test 4 : Fourier distance[" << dist << "]" << std::endl
	    << "Photo : +" << dist * 25 << std::endl
	    << "Screenshot : +" << dist * 25 << std::endl
	    << "Text : +" << (1 + cos(arg)) * (4 - dist) * 25 << std::endl
	    << "Drawing : +" << dist * 25 << std::endl << std::endl;


  // io::pbm::save(binarization::threshold(fft, max * 4 / 10), name.append("_fft.pbm"));

  // Print the result !

  std::cout << "Photo : " << score[PHOTO] << std::endl
	    << "Screenshot : " << score[SCREENSHOT] << std::endl
	    << "Text : " << score[TEXT] << std::endl
	    << "Drawing : " << score[DRAWING] << std::endl;

  max = 0;
  unsigned index = 0;

  for (unsigned i = 0; i < NB_DOCTYPE; ++i)
    if (score[i] > max)
      {
	max = score[i];
	index = i;
      }

  return index;
}
