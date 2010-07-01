#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/pw/all.hh>

#include <mln/draw/line.hh>

#include <mln/data/wrap.hh>
#include <mln/data/fill.hh>
#include <mln/data/convert.hh>


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

#include <mln/norm/l1.hh>

#include <scribo/core/object_groups.hh>
#include <scribo/core/component_set.hh>
#include <scribo/primitive/extract/components.hh>

#include <scribo/primitive/extract/lines_h_pattern.hh>
#include <scribo/primitive/remove/separators.hh>
#include <scribo/primitive/group/from_single_link.hh>
#include <scribo/preprocessing/denoise_fg.hh>
#include <scribo/preprocessing/rotate_90.hh>

#include <scribo/primitive/link/with_single_right_link_dmax_ratio.hh>
#include <scribo/primitive/link/with_single_right_link_dmax_ratio_aligned.hh>

#include <scribo/filter/object_links_top_aligned.hh>
#include <scribo/filter/object_groups_small.hh>
#include <scribo/filter/object_links_bottom_aligned.hh>
#include <scribo/debug/save_linked_bboxes_image.hh>
#include <scribo/debug/decision_image.hh>

int main(int argc, char *argv[])
{
  using namespace scribo;
  using namespace mln;

  image2d<bool> separators;
  mln::io::pbm::load(separators, argv[1]);

  float val = atof(argv[2]);

  util::timer t;

  t.start();

  // Hit or miss
  {
    unsigned length = 100;

    unsigned space = atoi(argv[3]);

    window2d win;
    dpoint2d
      dp1(-space, 0),
      dp2( space, 0);
    win.insert(dp1);
    win.insert(dp2);


    // Adjusting extension.
    extension::adjust_fill(separators, length / 2, 0);

    accu::count_value<bool> accu(true);
    typedef image2d<unsigned> I;

    I tmp = accu::transform_line(accu, separators, length, 1);

    image2d<bool> output;
    initialize(output, separators);

    mln_piter_(I) p(separators.domain());
//    mln_qiter(window2d) q(win, p);
    bool is_foreground;
    for_all(p)
    {

      if (tmp(p) > unsigned(0.95f * length) + 1)
      {
	output(p) = false;
	continue;
      }

      is_foreground = true;

      unsigned
	top_count = tmp(p + dp1),
	bot_count = tmp(p + dp2);

      if ((top_count >= unsigned(length * val) + 1
	  && bot_count <= unsigned(length * 0.1) + 1)
	  ||
	  (top_count <= unsigned(length * 0.1) + 1
	   && bot_count >= unsigned(length * val) + 1))
	is_foreground = false;

      output(p) = is_foreground;

    }

    io::pbm::save(output, "separators_hom.pbm");
  }

}
