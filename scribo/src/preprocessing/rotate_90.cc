#include <mln/core/image/image2d.hh>
#include <mln/value/rgb8.hh>
#include <mln/io/magick/load.hh>
#include <mln/io/ppm/save.hh>

#include <mln/util/timer.hh>

#include <scribo/debug/usage.hh>
#include <scribo/preprocessing/rotate_90.hh>


const char *args_desc[][2] =
{
  { "input.*", "An image." },
  { "output.ppm", "A rotated image." },
  { "positive", "if set to 1, performs a +90 rotation; -90 otherwise. (default: 0)" },
  {0, 0}
};



int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc != 3 && argc != 4)
    return scribo::debug::usage(argv,
				"Fast +90/-90 rotation",
				"input.* output.ppm",
				args_desc);


  typedef image2d<value::rgb8> I;
  I ima;
  io::magick::load(ima, argv[1]);

  bool positive = false;

  if (argc >= 4)
    positive = (atoi(argv[3]) != 0);


  mln::util::timer t;
  t.start();
  image2d<value::rgb8> out = scribo::preprocessing::rotate_90(ima, positive);
  t.stop();

  std::cout << t << std::endl;

  io::ppm::save(out, argv[2]);
}
