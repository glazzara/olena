# include <cmath>
# include <algorithm>

# include <mln/core/alias/vec3d.hh>
# include <mln/math/jacobi.hh>
# include <mln/fun/x2x/all.hh>
# include <mln/fun/x2v/all.hh>
# include <mln/convert/to.hh>
# include <mln/accu/compute.hh>
# include <mln/accu/center.hh>
# include <mln/accu/rms.hh>
# include <mln/trait/image_from_grid.hh>
# include <mln/set/compute.hh>

//Should be removed when closest_point functors are moved.
# include <mln/core/alias/neighb3d.hh>
# include <mln/transform/internal/closest_point_functor.hh>
# include <mln/canvas/distance_geodesic.hh>
# include <mln/pw/all.hh>

# include <mln/io/ppm/save.hh>
# include <mln/io/pbm/save.hh>
# include <mln/labeling/colorize.hh>

# include <mln/literal/black.hh>
# include <mln/literal/white.hh>
# include <mln/literal/colors.hh>

# include <mln/core/image/dmorph/slice_image.hh>
# include <mln/util/timer.hh>

#include <mln/core/image/imorph/tr_image.hh>
#include <mln/core/image/dmorph/extension_fun.hh>

#include <mln/accu/histo.hh>
#include <mln/accu/sum.hh>
#include <mln/debug/histo.hh>
#include <mln/io/cloud/load.hh>

#include <mln/registration/icp2.hh>

int main(int, char *argv[])
{
  using namespace mln;
  using namespace fun::x2x;
  using value::rgb8;
  using value::int_u8;
  using value::label_16;

  //Load image
  typedef image3d<bool> K;

  p_array<point3d> in_3d_, ref_3d_;
  io::cloud::load(in_3d_, argv[1]);
  io::cloud::load(ref_3d_, argv[2]);

  typedef rotation<3u,float> rot_t;
  typedef translation<3u,float> trans_t;
  composed<trans_t,rot_t> qk = registration::registration(in_3d_, ref_3d_);

}
