#include <mln/core/image/image2d.hh>
#include <scribo/binarization/sauvola_ms.hh>
#include <scribo/preprocessing/split_bg_fg.hh>
#include <mln/io/pbm/save.hh>


int main(int argc, char *argv[])
{
  using namespace mln;


  if (argc < 5)
  {
    std::cout << "Usage: " << argv[0] << " input.ppm w n out.pbm" << std::endl;
    return 1;
  }

  typedef image2d<value::rgb8> I;
  I ima;
  io::ppm::load(ima, argv[1]);


  unsigned w = atoi(argv[2]);

  image2d<bool> output = scribo::binarization::sauvola_ms(ima, w, 3, 67);

  io::pbm::save(output, argv[4]);
}
