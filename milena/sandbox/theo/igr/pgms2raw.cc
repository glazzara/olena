
#include <mln/core/image/image2d.hh>
#include <mln/make/image3d.hh>

#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/raw/save.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " output.raw input1.pgm .. inputn.pgm" << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  typedef image2d<int_u8> I;

  if (argc < 3)
    usage(argv);


  util::array<I> arr;
  for (int i = 2; i < argc; ++i)
    {
      I ima;
      io::pgm::load(ima, argv[i]);
      arr.append(ima);
    }
  image3d<int_u8> vol = make::image3d(arr);
  io::raw::save(vol, argv[1]);
}
