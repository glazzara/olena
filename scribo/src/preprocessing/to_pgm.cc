#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>
#include <mln/io/magick/load.hh>
#include <mln/io/pgm/save.hh>

#include <scribo/debug/usage.hh>


const char *args_desc[][2] =
{
  { "input.*", "A color image." },
  { "output.pgm", "A graylevel image." },
  {0, 0}
};



int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc != 3)
    return scribo::debug::usage(argv,
				"Convert a color image using the minimum RGB component value as intensity.",
				"input.* output.pgm",
				args_desc);


  typedef image2d<value::rgb8> I;
  I ima;
  io::magick::load(ima, argv[1]);

  image2d<value::int_u8> out;
  initialize(out, ima);

  value::rgb8 v;
  mln_piter_(I) p(ima.domain());
  for_all(p)
  {
    v = ima(p);
    out(p) = std::min(v.red(), std::min(v.green(), v.blue()));
  }

  io::pgm::save(out, argv[2]);
}
