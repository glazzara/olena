
#include <mln/core/image/image3d.hh>
#include <mln/core/alias/neighb3d.hh>

#include <mln/value/int_u8.hh>
#include <mln/io/raw/load.hh>
#include <mln/io/raw/save.hh>

#include <mln/morpho/elementary/gradient.hh>
#include <mln/morpho/closing_volume.hh>
#include <mln/morpho/meyer_wst.hh>

#include <mln/accu/mean.hh>
#include <mln/labeling/compute.hh>
#include <mln/fun/i2v/array.hh>
#include <mln/level/transform.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.raw output.raw" << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 3)
    usage(argv);

  trace::quiet = false;

  image3d<int_u8> vol, grad, clo;
  io::raw::load(vol, argv[1]);

  grad = morpho::elementary::gradient(vol, c6());
  clo  = morpho::closing_volume(grad, c6(), 666);

  typedef value::label_16 L;
  L n_basins;
  image3d<L> wst = morpho::meyer_wst(clo, c6(), n_basins);

  std::cout << n_basins << std::endl;

  accu::mean<int_u8, float, int_u8> a_;
  util::array<int_u8> a = labeling::compute(a_, vol, wst, n_basins);
  std::cout << a << std::endl;
  a[0] = 0;

  fun::i2v::array<int_u8> f;
  convert::from_to(a, f);
  image3d<int_u8> out = level::transform(wst, f);

  io::raw::save(out, argv[2]);
}
