#include <algorithm>
#include <cmath>

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/slice_image.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/core/routine/extend.hh>

#include <mln/io/dump/all.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>

#include <mln/accu/math/sum.hh>
#include <mln/arith/div.hh>
#include <mln/convert/from_to.hh>
#include <mln/data/compute.hh>
#include <mln/data/convert.hh>
#include <mln/data/stretch.hh>
#include <mln/make/image2d.hh>
#include <mln/math/diff_abs.hh>
#include <mln/morpho/dilation.hh>
#include <mln/morpho/erosion.hh>
#include <mln/pw/all.hh>
#include <mln/util/array.hh>

#include <mln/world/inter_pixel/display_edge.hh>
#include <mln/world/inter_pixel/compute.hh>
#include <mln/world/inter_pixel/immerse.hh>
#include <mln/world/inter_pixel/neighb2d.hh>
#include <mln/world/inter_pixel/all.hh>


using namespace mln;
using value::int_u8;
using value::int_u12;



namespace mln
{

  template <typename I>
  void io_save_edges_int_u12(const I& input,
		     value::int_u8 bg,
		     const std::string& filename)
  {
    mlc_equal(mln_value(I), value::int_u12)::check();
    mln_ch_value(I, value::int_u8) output;
    initialize(output, input);
    arith::div_cst(input, 16, output);
    io::pgm::save(world::inter_pixel::display_edge(output.unmorph_(),
						   bg,
						   3),
		  filename);
  }

} // end of namespace mln



// Distance function.
//-------------------

struct dist_t : Function_vv2v<dist_t>
{
  typedef int_u12 result;

  template <typename V>
  int_u12 operator()(const util::array<V>& v1,
		     const util::array<V>& v2) const
  {
    double res = 0.f;

    for (unsigned i = 0; i < v1.nelements(); ++i)
      res += std::min(v1[i], v2[i]);

    image1d<V> tmp_ima;
    image1d<V> tmp_ima2;
    accu::math::sum<V> accu_sum;

    convert::from_to(v1, tmp_ima);
    double sum_v1 = data::compute(accu_sum, tmp_ima);

    convert::from_to(v2, tmp_ima2);
    double sum_v2 = data::compute(accu_sum, tmp_ima2);

    if (sum_v1 == 0 && sum_v2 == 0)
      return 1;

    res /= std::max(sum_v1, sum_v2);
    res = 1 - res;
    res = (res * 4095) + 0.49;

    return (int) res;
  }
} dist;


struct dist2_t : Function_vv2v<dist2_t>
{
  typedef int_u12 result;

  template <typename V>
  int_u12 operator()(const util::array<V>& v1,
		     const util::array<V>& v2) const
  {
    double res = 0.f;

    for (unsigned i = 0; i < v1.nelements(); ++i)
    {
      unsigned maxi = std::max(v1[i], v2[i]);
      if (maxi != 0u)
	res += double(std::min(v1[i], v2[i])) / double(std::max(v1[i], v2[i]));
      else
	res += 1;
    }

    res = res / v1.nelements();
    res = 1 - res;
    res = (res * 4095) + 0.49;

    return (int) res;
  }
} dist2;


struct dist3_t : Function_vv2v<dist3_t>
{
  typedef int_u12 result;

  template <typename V>
  int_u12 operator()(const util::array<V>& v1,
		     const util::array<V>& v2) const
  {
    double res = 0.f;
    double min = 0;
    double max = 0;

    for (unsigned i = 0; i < v1.nelements(); ++i)
    {
      min += std::min(v1[i], v2[i]);
      max += std::max(v1[i], v2[i]);
    }

    if (max)
      res = min / max;

    res = 1 - res;
    res = (res * 4095) + 0.49;

    return (int) res;
  }
} dist3;


struct dist4_t : Function_vv2v<dist4_t>
{
  typedef int_u12 result;

  template <typename V>
  int_u12 operator()(const util::array<V>& v1,
		     const util::array<V>& v2) const
  {
    double res = 0.f;
    double maxi;

    for (unsigned i = 0; i < v1.nelements(); ++i)
    {
      maxi = std::max(v1[i], v2[i]);
      if (maxi)
	res += (double) math::diff_abs(v1[i], v2[i]) / maxi;
      else
	res += 1;
    }

    res = res / v1.nelements();
    //res = 1 - res;
    res = (res * 4095) + 0.49;

    return (int) res;
  }
} dist4;


struct dist5_t : Function_vv2v<dist5_t>
{
  typedef int_u12 result;

  template <typename V>
  int_u12 operator()(const util::array<V>& v1,
		     const util::array<V>& v2) const
  {
    double res = 0.f;

    for (unsigned i = 0; i < v1.nelements(); ++i)
      res += (double) math::diff_abs(v1[i], v2[i]);

    res = res / v1.nelements();
    //res = 1 - res;
    res = (res * 4095) + 0.49;

    return (int) res;
  }
} dist5;






int usage(const char* bin)
{
  std::cout << "Usage: " << bin << " input.dump min dist_type output.dump" << std::endl;
  return 1;
}

int main(int argc, char* argv[])
{
  if (argc != 5)
    return usage(argv[0]);

  float min = atof(argv[2]);
  unsigned dist_type = atoi(argv[3]);


  // Initialization.
  image3d<float> input;
  io::dump::load(input, argv[1]);
  typedef image2d<util::array<float> > I;
  I ima_arr;
  initialize(ima_arr, slice(input, 0));
  for (unsigned int i = 0; i < input.nslices(); ++i)
  {
    image2d<float> tmp_slice = duplicate(slice(input, i));
    mln_piter_(image2d<float>) p(tmp_slice.domain());
    for_all(p)
      ima_arr(p).append(tmp_slice(p) - min); // We set the minimum value to 0.
  }


  // Edges image creation.
  typedef image_if<I, world::inter_pixel::is_pixel> Ix;
  Ix imax = world::inter_pixel::immerse(ima_arr);

  // Edges distance computation.
  image_if<image2d<int_u12>, world::inter_pixel::is_separator> edges;
  if (dist_type == 1)
    edges = world::inter_pixel::compute(imax, dist);
  else if (dist_type == 2)
    edges = world::inter_pixel::compute(imax, dist2);
  else if (dist_type == 3)
    edges = world::inter_pixel::compute(imax, dist3);
  else if (dist_type == 4)
    edges = world::inter_pixel::compute(imax, dist4);
  else if (dist_type == 5)
    edges = world::inter_pixel::compute(imax, dist5);

  io_save_edges_int_u12(edges, 0, "dist.pgm");

  io::dump::save(edges.unmorph_(), argv[4]);

  return 0;
}
