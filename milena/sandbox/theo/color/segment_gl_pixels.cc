
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image_if.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>
#include <mln/value/rgb8.hh>
#include <mln/literal/colors.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/pgm/save.hh>

#include <mln/level/convert.hh>

#include <mln/morpho/elementary/gradient.hh>
#include <mln/morpho/meyer_wst.hh>

#include <mln/accu/height.hh>

#include "segment.hh"





void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm n change extinction echo output.ppm" << std::endl;
  std::cerr << "gray-level version on pixels (without edges)" << std::endl;
  std::cerr << "  n >= 2" << std::endl;
  std::cerr << "  change = 0 (none), 1 (move-down), or 2 (fuse)" << std::endl;
  std::cerr << "  extinction = 0 (none) or 1 (effective)" << std::endl;
  std::cerr << "  echo = 0 (mute) or 1 (verbose)" << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;

  using value::int_u8;
  using value::rgb8;


  if (argc != 7)
    usage(argv);


  // Gray-level debase version.
  // --------------------------


  typedef image2d<int_u8> I;

  I input;
  io::pgm::load(input, argv[1]);

  unsigned n_objects = atoi(argv[2]);

  int change_kind = atoi(argv[3]);
  if (change_kind < 0 || change_kind > 2)
    usage(argv);

  int do_extinction = atoi(argv[4]);
  if (do_extinction != 0 && do_extinction != 1)
    usage(argv);

  bool echo = atoi(argv[5]);


  // Neighborhood.
  mln_VAR(nbh, c4());


  // Changing input into 'f'.

  I f = morpho::elementary::gradient(input, c4());


  // // granulometry:
  // gran_filter(f, c4(), a_);



  // accu::count< util::pix<I> > a_;
  // accu::sum_pix< util::pix<I> > a_;
  // accu::volume<I> a_;
  accu::height<I> a_;

  // It seems that both extinction and changing attributes only
  // properly works for the 'height' attribute.

  I g = filter(f, c4(),
	       a_,
	       change_kind, do_extinction,
	       n_objects,
	       echo);

  if (echo)
    debug::println("activity (g != f) = ", (pw::value(g) != pw::value(f)) | f.domain());


  // Watershed transform.

  typedef value::label_8 L;
  L nbasins;
  mln_ch_value_(I, L) w = morpho::meyer_wst(g, c4(), nbasins);

  image2d<rgb8> output = level::convert(rgb8(), input);
  data::fill((output | (pw::value(w) == 0)).rw(), literal::red);

  io::ppm::save(output, argv[6]);
}
