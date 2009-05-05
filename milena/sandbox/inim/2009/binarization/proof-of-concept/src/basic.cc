//
// main.cc for INIM
// Made by nicuveo <crucuny@gmail.com>
//



//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
// Includes

#include <mln/core/image/image2d.hh>
#include <mln/core/image/image1d.hh>
#include <mln/histo/all.hh>
#include <mln/io/pgm/all.hh>
#include <mln/value/all.hh>
#include <mln/accu/histo.hh>
#include <mln/arith/revert.hh>
#include <mln/convert/to_image.hh>
#include <mln/morpho/meyer_wst.hh>
#include <mln/core/alias/neighb1d.hh>
#include <mln/geom/all.hh>
#include <mln/morpho/closing_area.hh>

#include "util.hxx"



//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
// Implementation

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

  io::pgm::load (in, file_in);
  histo::data<value::int_u8> histo = histo::compute (in);

  image1d<size_t> hist = convert::to_image (histo);

  // Histo
  print_histo (hist, file_hist + "_1.histo");

  image1d<size_t> hist_close;
  initialize(hist_close, hist);
  morpho::closing_area(arith::revert(hist), c2(), strength, hist_close);

  // Closed reverted histo
  print_histo (arith::revert(hist_close), file_hist + "_2.histo");

  image1d<size_t> wst = morpho::meyer_wst<size_t, image1d<size_t>, neighb1d> (hist_close, c2());
  std::vector<value::int_u8> colors = colors_init (wst);

  image2d<value::int_u8> cla;
  initialize(cla, in);

  image2d<value::int_u8>::fwd_piter p (in.domain ());
  for_all (p)
    cla (p) = colors_match (colors, in (p));

  image2d<value::int_u8> out = binarization(cla, 20);

  mln::io::pgm::save (out, file_out);
}
