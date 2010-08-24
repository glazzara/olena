#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/pw/all.hh>

#include <mln/draw/line.hh>

#include <mln/data/wrap.hh>
#include <mln/data/fill.hh>
#include <mln/data/paste.hh>
#include <mln/data/convert.hh>

#include <mln/subsampling/antialiased.hh>

#include <mln/labeling/colorize.hh>

#include <mln/morpho/closing/structural.hh>
#include <mln/morpho/closing/area.hh>
#include <mln/morpho/opening/structural.hh>
#include <mln/win/rectangle2d.hh>
#include <mln/win/vline2d.hh>

#include <mln/logical/not.hh>
#include <mln/io/pbm/all.hh>
#include <mln/io/pgm/all.hh>
#include <mln/io/ppm/save.hh>

#include <mln/literal/colors.hh>

#include <mln/value/label_16.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/rgb8.hh>

#include <mln/draw/box_plain.hh>

#include <mln/transform/influence_zone_geodesic.hh>

#include <mln/data/stretch.hh>

#include <mln/util/timer.hh>

#include <scribo/core/object_groups.hh>
#include <scribo/primitive/extract/components.hh>

#include <scribo/primitive/extract/vertical_separators.hh>
#include <scribo/primitive/remove/separators.hh>
#include <scribo/primitive/group/from_single_link.hh>
#include <scribo/preprocessing/denoise_fg.hh>

#include <scribo/primitive/link/with_single_right_link_dmax_ratio.hh>

#include <scribo/filter/object_links_top_aligned.hh>
#include <scribo/filter/object_groups_small.hh>
#include <scribo/filter/object_links_bottom_aligned.hh>
#include <scribo/debug/save_linked_bboxes_image.hh>


namespace mln
{

  void process(image2d<bool> input, unsigned scale, unsigned l)
  {
    win::hline2d vl(l);
    image2d<bool> input_op_line = morpho::opening::structural(input, vl);

    if (scale == 1)
      io::pbm::save(input_op_line, "input_op_line_1.pbm");
    if (scale == 2)
      io::pbm::save(input_op_line, "input_op_line_2.pbm");
    if (scale == 3)
      io::pbm::save(input_op_line, "input_op_line_3.pbm");

    win::rectangle2d rect(11, 11);
    image2d<bool> input_op = morpho::opening::structural(input_op_line, rect);

    if (scale == 1)
      io::pbm::save(input_op, "input_op_rect_1.pbm");
    if (scale == 2)
      io::pbm::save(input_op, "input_op_rect_2.pbm");
    if (scale == 3)
      io::pbm::save(input_op, "input_op_rect_3.pbm");


    image2d<value::rgb8> output = data::convert(value::rgb8(), logical::not_(input));
    data::fill((output | pw::value(input_op)).rw(), literal::red);

    if (scale == 1)
      io::ppm::save(output, "output_1.ppm");
    if (scale == 2)
      io::ppm::save(output, "output_2.ppm");
    if (scale == 3)
      io::ppm::save(output, "output_3.ppm");
  }

}

int main(int argc, char *argv[])
{
  using namespace scribo;
  using namespace mln;

  typedef image2d<bool> I;
  I input;
  mln::io::pbm::load(input, argv[1]);

  logical::not_inplace(input);

  unsigned l = atoi(argv[2]);

  process(input, 1, l);


  typedef image2d<value::int_u8> J;
  J input_pgm = data::convert(value::int_u8(), input);

  I input_x2 = data::convert(bool(), subsampling::antialiased(input_pgm, 2));
  process(input_x2, 2, l / 2);


  I input_x4 = data::convert(bool(), subsampling::antialiased(input_pgm, 4));
  process(input_x4, 3, l / 4);
}
