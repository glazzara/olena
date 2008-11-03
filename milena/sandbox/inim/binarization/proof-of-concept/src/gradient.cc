//
// main.cc for INIM
// Made by nicuveo <crucuny@gmail.com>
//



//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
// Includes

#include <cassert>
#include <string>
#include <vector>

#include <mln/core/image/image2d.hh>
#include <mln/histo/all.hh>
#include <mln/io/pgm/all.hh>
#include <mln/accu/histo.hh>
#include <mln/arith/revert.hh>
#include <mln/convert/to_image.hh>
#include <mln/morpho/meyer_wst.hh>
#include <mln/core/alias/neighb1d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/geom/all.hh>
#include <mln/morpho/closing_area.hh>
#include <mln/morpho/gradient.hh>
#include <mln/level/fill.hh>

// DEBUG
#include <mln/debug/all.hh>

#include <cmath>

#include "util.hxx"



//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
// Implementation

template <int P, int Q>
double
strength_of (mln::value::int_u8 x)
{
  double xprime = 1 - pow (x / 255., P);

  return 255 * log10 (1 + 9 * xprime);
}

template <int P>
double
strength_of (mln::value::int_u8 x)
{
  return strength_of<P, 1> (x);
}

int
main (int argc, char** argv)
{
  using namespace mln;

  if (argc != 5)
    return 1;
  std::string file_in   = argv[1];
  std::string file_out  = argv[2];
  std::string file_hist = argv[3];
  std::istringstream is(argv[4]);
  int strength;
  is >> strength;

  image2d<value::int_u8> in;
  image2d<value::int_u8> grad;
  image1d<unsigned long> hist(256);
  image1d<unsigned long> hist_close(256);

  io::pgm::load (in, file_in);

  initialize(grad, in);
  grad = morpho::gradient(in, win_c4p());

  level::fill(hist, 0);
  image2d<value::int_u8>::fwd_piter p (in.domain ());
  for_all (p)
    hist.at(in(p)) += (unsigned) strength_of<2> (grad (p));

  // Histo
  print_histo (hist, file_hist + "_1.histo");

  hist = arith::revert(hist);
  morpho::closing_area(hist, c2(), strength, hist_close);

  // Closed reverted histo
  print_histo (arith::revert(hist_close), file_hist + "_2.histo");

  image1d<size_t> wst = morpho::meyer_wst<size_t, image1d<unsigned long>, neighb1d> (hist_close, c2());
  std::vector<value::int_u8> colors = colors_init (wst);

  image2d<value::int_u8> cla;
  initialize(cla, in);

  image2d<value::int_u8>::fwd_piter q (in.domain ());
  for_all (q)
    cla (q) = colors_match (colors, in (q));

  image2d<value::int_u8> out = binarization(cla, 20);

  mln::io::pgm::save (out, file_out);
}
