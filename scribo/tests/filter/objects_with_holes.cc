#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/io/pbm/load.hh>
#include <mln/value/label_8.hh>

#include <mln/util/timer.hh>
#include <mln/labeling/background.hh>
#include <mln/value/label_32.hh>
#include <mln/data/convert.hh>
#include <mln/data/wrap.hh>

#include <mln/debug/println.hh>



#include <scribo/filter/objects_with_holes.hh>
#include <scribo/primitive/extract/objects.hh>

int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc != 2)
    return 1;

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  value::label_16 nlabels;
  typedef object_image(image2d<value::label_16>) O;
  O objects = scribo::primitive::extract::objects(input, c8(), nlabels);

  util::timer timer;
  timer.start();

  value::label_16 nbglabels;
  image2d<bool> bin = data::convert(bool(), objects);
  image2d<value::label_16> lbl_bg = labeling::background(bin, c8(), nbglabels);
  float t_ = timer;
  std::cout << t_ << std::endl;

  timer.restart();
  O out = scribo::filter::objects_with_holes(objects, 2, 1);
//   debug::println(out);

  t_ = timer;
  std::cout << t_ << std::endl;
  io::pgm::save(data::wrap(value::label_8(), out), "out.pgm");
}
