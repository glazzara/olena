#include <mln/core/image/image2d.hh>
#include <scribo/binarization/sauvola.hh>
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


  image2d<value::int_u8>
    ima_r,
    ima_g,
    ima_b;
  scribo::preprocessing::internal::split(ima, ima_r, ima_g, ima_b);

  unsigned w = atoi(argv[2]);

  image2d<bool>
    bin_ima_r = scribo::binarization::sauvola_ms(ima_r, w, 3, 67),
    bin_ima_g = scribo::binarization::sauvola_ms(ima_g, w, 3, 67),
    bin_ima_b = scribo::binarization::sauvola_ms(ima_b, w, 3, 67),
    output;
  initialize(output, bin_ima_r);

  mln_piter_(image2d<bool>) p(output.domain());
  for_all(p)
  {
    int ntrue = 0;
    if (bin_ima_r(p))
      ++ntrue;
    if (bin_ima_g(p))
      ++ntrue;
    if (bin_ima_b(p))
      ++ntrue;

    output(p) = (ntrue >= atoi(argv[3]));
  }

  io::pbm::save(output, argv[4]);
}
