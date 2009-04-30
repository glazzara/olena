#include <algorithm>

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/slice_image.hh>
#include <mln/core/routine/duplicate.hh>

#include <mln/io/dump/all.hh>
#include <mln/io/pgm/save.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>

#include <mln/accu/sum.hh>
#include <mln/data/fill.hh>
#include <mln/debug/quiet.hh>
#include <mln/convert/from_to.hh>
#include <mln/level/compute.hh>
#include <mln/level/stretch.hh>
#include <mln/util/array.hh>

#include <mln/world/inter_pixel/display_edge.hh>


using namespace mln;
using value::int_u8;
using value::int_u12;



inline
point2d
get_edge_location(point2d p, point2d n)
{
  point2d res;

  res[0] = p[0] * 2;
  res[1] = p[1] * 2;

  if (n[0] > p[0])
    ++(res[0]);
  if (n[0] < p[0])
    --(res[0]);

  if (n[1] > p[1])
    ++(res[1]);
  if (n[1] < p[1])
    --(res[1]);

  return res;
}


template <typename I>
inline
float
compute_dist(image2d<util::array<I> >& ima_arr,
	     image2d<float>& ima_sum,
	     point2d p, point2d n)
{
  float res = 0.f;

  for (unsigned i = 0; i < ima_arr(p).nelements(); ++i)
    res += std::min(ima_arr(p)[i], ima_arr(n)[i]);

  float max = std::max(ima_sum(p), ima_sum(n));
  if (max)
    res /= std::max(ima_sum(p), ima_sum(n));
  else
    res = 0.f;

  //std::cout << "dist: " << res << std::endl;

  return res;
}

template <typename I>
inline
void
compute_sum_arrays(image2d<float>& ima_sum, image2d<util::array<I> >& ima_arr)
{
  mln_piter(image2d<float>) p(ima_sum.domain());
  for_all(p)
  {
    image1d<I> tmp_ima;
    convert::from_to(ima_arr(p), tmp_ima);
    accu::sum<I> accu_sum;
    ima_sum(p) = level::compute(accu_sum, tmp_ima);
  }
}

template <typename I>
inline
image2d<float>
dist_on_edges(image2d<util::array<I> >& ima_arr)
{
  box2d b = ima_arr.bbox();
  image2d<float> edges(make::box2d(2 * b.pmin()[0], 2 * b.pmin()[1],
				   2 * b.pmax()[0], 2 * b.pmax()[1]));
  box2d edge_b = edges.bbox();
  data::fill(edges, -1.f);
  image2d<float> ima_sum;
  initialize(ima_sum, ima_arr);
  compute_sum_arrays(ima_sum, ima_arr);

  mln_piter(image2d<util::array<I> >) p(ima_arr.domain());
  mln_niter(neighb2d) n(c4(), p);
  for_all(p)
  {
    //std::cout << "can i has a loop?" << std::endl;
    for_all(n)
    {
      point2d location = get_edge_location(p, n);
      //std::cout << "p: " << p << " | n: " << n << " | location: " << location << std::endl;
      if (b.has(n) && edge_b.has(location) && edges(location) == -1.f)
      {
	//std::cout << "\\o/" << std::endl;
	edges(location) = compute_dist(ima_arr, ima_sum, p, n);
      }
    }
  }

  return edges;
}





int usage(const char* bin)
{
  std::cout << "Usage: " << bin << " input.dump" << std::endl;
  return 1;
}

int main(int argc, char* argv[])
{
  if (argc != 2)
    return usage(argv[0]);

  // Initialization.
  image3d<int_u12> input;
  io::dump::load(input, argv[1]);
  image2d<util::array<int_u12> > ima_arr;
  initialize(ima_arr, slice(input, 0));
  for (unsigned int i = 0; i < input.nslices(); ++i)
  {
    image2d<int_u12> tmp_slice = duplicate(slice(input, i));
    mln_piter_(image2d<int_u12>) p(tmp_slice.domain());
    for_all(p)
      ima_arr(p).append(tmp_slice(p));
  }

  // Edges computation.
  image2d<float> edges = dist_on_edges(ima_arr);

  /*mln_piter_(image2d<float>) p_iter(edges.domain());
  for_all(p_iter)
  {
    std::cout << "<main> " << edges(p_iter);
    if (p_iter.row() % 2 + p_iter.col() % 2 == 1)
      std::cout << " OK";
    std::cout << std::endl;
  }*/

  // Display.
  image2d<float> display_ima = world::inter_pixel::display_edge(edges, 0.0, 3);
  //std::cout << "display_ima box: " << display_ima.bbox() << std::endl;
  io::pgm::save(level::stretch(int_u8(), display_ima), "edges.pgm");

  return 0;
}
