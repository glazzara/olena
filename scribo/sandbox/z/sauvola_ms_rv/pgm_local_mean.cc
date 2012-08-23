#include <mln/core/image/image2d.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/value/int_u8.hh>
#include <mln/data/compute_in_window.hh>
#include <mln/accu/stat/mean.hh>
#include <mln/win/rectangle2d.hh>
#include <mln/data/convert.hh>

int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc != 4)
  {
    std::cout << "Usage: " << argv[0] << " in.pgm win_size out.pgm" << std::endl;
    return 1;
  }

  unsigned win_size = atoi(argv[2]);
  win::rectangle2d win(win_size, win_size);
  typedef value::int_u8 V;
  image2d<V> input;
  io::pgm::load(input, argv[1]);

  image2d<float> mean = data::compute_in_window(accu::stat::mean<V>(), input, win);

  io::pgm::save(data::convert(value::int_u8(), mean), argv[3]);
}
