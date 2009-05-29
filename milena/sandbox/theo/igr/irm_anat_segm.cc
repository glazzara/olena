
#include <mln/core/image/image3d.hh>
#include <mln/core/alias/neighb3d.hh>

#include <mln/value/int_u8.hh>
#include <mln/io/dump/load.hh>
#include <mln/io/dump/save.hh>

#include <mln/morpho/elementary/gradient.hh>
#include <mln/morpho/closing_height.hh>
#include <mln/morpho/watershed/flooding.hh>

#include <mln/accu/mean.hh>
#include <mln/labeling/compute.hh>
#include <mln/fun/i2v/array.hh>
#include <mln/data/transform.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.dump lambda output.dump" << std::endl
	    << "  It relies on height closing; e.g., take lambda = 20" << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 4)
    usage(argv);

  trace::quiet = false;

  trace::entering("main");

  image3d<int_u8> vol, grad, clo;
  io::dump::load(vol, argv[1]);

  unsigned lambda = atoi(argv[2]);

  grad = morpho::elementary::gradient(vol, c6());
  clo  = morpho::closing_height(grad, c6(), lambda);

  typedef value::label_16 L;
  L n_basins;
  image3d<L> wst = morpho::watershed::flooding(clo, c6(), n_basins);

  std::cout << n_basins << std::endl;

  accu::mean<int_u8, float, int_u8> a_;
  util::array<int_u8> a = labeling::compute(a_, vol, wst, n_basins);
  for (L l = 1; l <= n_basins; ++l)
    if (a[l] == 0)
      a[l] = 1;
  a[0] = 0;

  fun::i2v::array<int_u8> f;
  convert::from_to(a, f);
  image3d<int_u8> out = data::transform(wst, f);

  io::dump::save(out, argv[3]);

  trace::exiting("main");
}
