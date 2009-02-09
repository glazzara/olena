
#include <mln/make/image3d.hh>
#include <mln/data/fill.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/dump/load.hh>
#include <mln/io/dump/save.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.dump s_min r_min c_min s_max r_max c_max output.dump" << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 9)
    usage(argv);

  image3d<int_u8> vol;
  io::dump::load(vol, argv[1]);

  int
    s_min = atoi(argv[2]),
    r_min = atoi(argv[3]),
    c_min = atoi(argv[4]),

    s_max = atoi(argv[5]),
    r_max = atoi(argv[6]),
    c_max = atoi(argv[7]);

  // FIXME: Test!

  box3d b = make::box3d(s_min, r_min, c_min,  s_max, r_max, c_max);

  // IRM.dump => 0, 180, 50,  14, 340, 400

  image3d<int_u8> ima(b);
  data::fill(ima, vol);
  io::dump::save(ima, argv[8]);
}
