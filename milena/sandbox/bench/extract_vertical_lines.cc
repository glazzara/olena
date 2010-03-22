#include <mln/util/timer.hh>
#include <mln/core/image/image2d.hh>
#include <mln/io/pbm/all.hh>

#include <scribo/preprocessing/rotate_90.hh>
#include <scribo/primitive/extract/lines_v_pattern.hh>
#include <scribo/primitive/extract/lines_h_pattern.hh>

int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc < 4)
  {
    std::cout << "Usage: " << argv[0] << " input.pbm line_length delta"
	      << std::endl;
    return 1;
  }

  image2d<bool> ima;
  io::pbm::load(ima, argv[1]);

  util::timer t;

  unsigned l = atoi(argv[2]);
  unsigned d = atoi(argv[3]);

  {
    t.start();
    image2d<bool> imal = scribo::primitive::extract::lines_v_pattern(ima, l, d);
    t.stop();
    std::cout << "Extract vertical lines - " << t << std::endl;
    io::pbm::save(imal, "vlines.pbm");
  }


  {
    t.restart();
    image2d<bool> rot_ima = scribo::preprocessing::rotate_90(ima);
    t.stop();
    io::pbm::save(rot_ima, "rot_ima.pbm");
    t.resume();
    image2d<bool> imal = scribo::primitive::extract::lines_h_pattern(rot_ima, l, d);
    t.stop();
    std::cout << "Rotate + Extract horizontal lines - " << t << std::endl;
    io::pbm::save(imal, "hlines.pbm");
  }

}
