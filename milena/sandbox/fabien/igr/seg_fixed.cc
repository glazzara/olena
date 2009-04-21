#include <algorithm>

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/slice_image.hh>
#include <mln/core/routine/duplicate.hh>

#include <mln/io/dump/all.hh>

#include <mln/value/int_u12.hh>

#include <mln/accu/sum.hh>
#include <mln/data/fill.hh>
#include <mln/convert/from_to.hh>
#include <mln/level/compute.hh>
#include <mln/util/array.hh>


using namespace mln;
using value::int_u12;



inline
point2d
get_edge_location(point2d p, point2d n)
{
  point2d res(p);

  res.to_vec()[0] *= 2;
  res.to_vec()[1] *= 2;

  if (n.to_vec()[0] > p.to_vec()[0])
    ++(res.to_vec()[0]);
  if (n.to_vec()[0] < p.to_vec()[0])
    --(res.to_vec()[0]);

  if (n.to_vec()[1] > p.to_vec()[1])
    ++(res.to_vec()[1]);
  if (n.to_vec()[1] < p.to_vec()[1])
    --(res.to_vec()[1]);

  return res;
}


template <typename I>
inline
float
compute_dist(image2d<util::array<I> > ima_arr,
	     image2d<float> ima_sum,
	     point2d p, point2d n)
{
  float res = 0.f;

  for (unsigned i = 0; i < ima_arr(p).nelements(); ++i)
    res += std::min(ima_arr(p)[i], ima_arr(n)[i]);
  res /= std::max(ima_sum(p), ima_sum(n));

  return res;
}

template <typename I>
inline
void
compute_sum_arrays(image2d<float> ima_sum, image2d<util::array<I> > ima_arr)
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
image2d<float>
dist_on_edges(image2d<util::array<I> > ima_arr)
{
  image2d<float> edges(2 * ima_arr.nrows(), 2 * ima_arr.ncols());
  data::fill(edges, -1.f);
  image2d<float> ima_sum;
  initialize(ima_sum, ima_arr);
  compute_sum_arrays(ima_sum, ima_arr);

  mln_piter(image2d<util::array<I> >) p(ima_arr.domain());
  mln_niter(neighb2d) n(c4(), p);
  for_all(p)
    for_all(n)
    {
      point2d location = get_edge_location(p, n);
      if (edges(location) != -1)
	edges(location) = compute_dist(ima_arr, ima_sum, p, n);
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
  typedef int_u12 I;

  if (argc != 2)
    return usage(argv[0]);

  image3d<I> input;
  io::dump::save(input, argv[1]);
  image2d<util::array<I> > ima_arr;
  initialize(ima_arr, slice(input, 0));
  for (unsigned int i = 0; i < input.nslices(); ++i)
  {
    image2d<I> tmp_slice = duplicate(slice(input, i));
    mln_piter_(image2d<I>) p(tmp_slice.domain());
    for_all(p)
      ima_arr(p).append(tmp_slice(p));
  }

  image2d<float> edges = dist_on_edges(ima_arr);

  return 0;
}
