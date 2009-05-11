#include <algorithm>

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/slice_image.hh>
#include <mln/core/image/image_if.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/core/routine/extend.hh>
#include <mln/core/var.hh>

#include <mln/io/dump/all.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u12.hh>
#include <mln/value/label_16.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/float01_8.hh>

#include <mln/accu/sum.hh>
#include <mln/data/fill.hh>
#include <mln/data/paste.hh>
#include <mln/debug/quiet.hh>
#include <mln/convert/from_to.hh>
#include <mln/labeling/wrap.hh>
#include <mln/level/compute.hh>
#include <mln/level/convert.hh>
#include <mln/level/stretch.hh>
#include <mln/make/image2d.hh>
#include <mln/math/diff_abs.hh>
#include <mln/morpho/dilation.hh>
#include <mln/morpho/erosion.hh>
#include <mln/morpho/closing/volume.hh>
#include <mln/morpho/elementary/gradient.hh>
#include <mln/morpho/watershed/flooding.hh>
#include <mln/pw/all.hh>
#include <mln/util/array.hh>

#include <mln/world/inter_pixel/display_edge.hh>
#include <mln/world/inter_pixel/compute.hh>
#include <mln/world/inter_pixel/immerse.hh>
#include <mln/world/inter_pixel/neighb2d.hh>
#include <mln/world/inter_pixel/all.hh>

#include <mln/labeling/colorize.hh>
#include <mln/debug/println.hh>
#include <mln/trace/quiet.hh>

#include "plot_label.hh"

using namespace mln;
using value::int_u8;
using value::int_u12;
using value::label_16;
using value::float01_8;



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

  res = res;

  return 1 - res;
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


// Dummy.
//-------
template <typename I, typename N>
inline
image2d<int>
dummy_dist_on_edges(const I& input, const N& nbh)
{
  image2d<int> output;
  initialize(output, input);
  data::fill(output, 0);

  mln_piter(I) p(input.domain());
  mln_niter(N) n(nbh, p);
  for_all(p)
  {
    n.start();
    int c1 = input(n);
    n.next();
    int c2 = input(n);
    output(p) = math::diff_abs(c1, c2);
  }

  return output;
}



// Distance function.
//-------------------

struct dist_t : Function_vv2v<dist_t>
{
  typedef float result;

  template <typename V>
  float operator()(util::array<V> v1, util::array<V> v2) const
  {
    float res = 0.f;

    for (unsigned i = 0; i < v1.nelements(); ++i)
      res += std::min(v1[i], v2[i]);

    image1d<V> tmp_ima;
    image1d<V> tmp_ima2;
    accu::sum<V> accu_sum;

    convert::from_to(v1, tmp_ima);
    float sum_v1 = level::compute(accu_sum, tmp_ima);

    convert::from_to(v2, tmp_ima2);
    float sum_v2 = level::compute(accu_sum, tmp_ima2);

    res /= std::max(sum_v1, sum_v2);

    return 1 - res;
  }
} dist;








int usage(const char* bin)
{
  std::cout << "Usage: " << bin << " input.dump closing" << std::endl;
  return 1;
}

int main(int argc, char* argv[])
{
  if (argc != 3)
    return usage(argv[0]);


  // Initialization.
  image3d<int_u12> input;
  io::dump::load(input, argv[1]);
  typedef image2d<util::array<int_u12> > I;
  I ima_arr;
  initialize(ima_arr, slice(input, 0));
  for (unsigned int i = 0; i < input.nslices(); ++i)
  {
    image2d<int_u12> tmp_slice = duplicate(slice(input, i));
    mln_piter_(image2d<int_u12>) p(tmp_slice.domain());
    for_all(p)
      ima_arr(p).append(tmp_slice(p));
  }


  // Edges image creation.
  //image2d<float> edges = dist_on_edges(ima_arr);
  typedef image_if<I, world::inter_pixel::is_pixel> Ix;
  Ix imax = world::inter_pixel::immerse(ima_arr);

  // Edges distance computation.
  mln_VAR(edges, world::inter_pixel::compute(imax, dist));

  {
    // Display.
    mln_VAR(display_ima, world::inter_pixel::display_edge(edges.unmorph_(), 0.0, 3));
    io::pgm::save(level::stretch(int_u8(), display_ima), "edges.pgm");
  }

  // Save edges.
  //mln_VAR(e, edges.unmorph_());
  //data::fill((e | (!world::inter_pixel::is_separator())).rw(), nbasins.next());
  //io::pgm::save(level::stretch(int_u8(), edges.unmorph_()), "edges_raw.pgm");

  mln_VAR(e, level::stretch(int_u8(), edges));

  {
    // Display.
    mln_VAR(display_ima, world::inter_pixel::display_edge(e.unmorph_(), 0.0, 3));
    io::pgm::save(level::stretch(int_u8(), display_ima), "e.pgm");
  }

  // Closing.
  mln_VAR(clo, morpho::closing::volume(e, world::inter_pixel::e2e(), atoi(argv[2])));

  {
    // Display.
    mln_VAR(display_clo, world::inter_pixel::display_edge(clo.unmorph_(), 0.0, 3));
    io::pgm::save(level::stretch(int_u8(), display_clo), "edges2.pgm");
  }

  typedef label_16 L;
  L nbasins;
  mln_VAR(wst, morpho::watershed::flooding(clo, world::inter_pixel::e2e(), nbasins));

  std::cout << "nbasins: " << nbasins << std::endl;


  mln_VAR(w_all, wst.unmorph_());
  //data::fill((w | (!world::inter_pixel::is_separator())).rw(), nbasins.next());
  mln_VAR(w_pixels, w_all | world::inter_pixel::is_pixel());
  data::paste(morpho::dilation(extend(w_pixels, pw::value(w_all)), c4().win()), w_all);
  // edges -> dots
  mln_VAR(w_dots, w_all | world::inter_pixel::dim2::is_dot());
  data::paste(morpho::erosion(extend(w_dots, pw::value(w_all)), c4().win()), w_all);

  //io::ppm::save(labeling::colorize(value::rgb8(), w, nbasins.next()), "result.ppm");
  io::pgm::save(labeling::wrap(int_u8(), w_all), "result_labels.pgm");


  // Plots.
  image2d<L> w_simple = world::inter_pixel::full2image(w_all);
  plot_label(input, w_simple, 166u);
  plot_label(input, w_simple, 182u);
  plot_label(input, w_simple, 188u);
  plot_label(input, w_simple, 189u);
  plot_label(input, w_simple, 193u);
  plot_label(input, w_simple, 195u);
  plot_label(input, w_simple, 196u);
  plot_label(input, w_simple, 198u);
  plot_label(input, w_simple, 199u);
  plot_label(input, w_simple, 200u);
  plot_label(input, w_simple, 201u);











  /*mln_VAR(clo, morpho::closing::volume(edges | world::inter_pixel::dim2::is_edge(), world::inter_pixel::e2e(), atoi(argv[2])));

  // Debug.
  //debug::println("clo", clo);

  // Display.
  image2d<float> display_ima2 = world::inter_pixel::display_edge(clo.unmorph_(), 0.0, 3);
  io::pgm::save(level::stretch(int_u8(), display_ima2), "edges2.pgm");

  // Watershed.
  typedef label_16 L;
  L nbasins;
  mln_VAR(wst, morpho::watershed::flooding(clo, world::inter_pixel::e2e(), nbasins));

  // Debug.
  //debug::println("wst", wst);

  // Extension.
  image2d<L> w_all = wst.unmorph_();
  // edges -> pixel
  mln_VAR(w_pixels, w_all | world::inter_pixel::dim2::is_pixel());
  data::paste(morpho::dilation(extend(w_pixels, pw::value(w_all)), c4().win()), w_all);
  // edges -> dots
  mln_VAR(w_dots, w_all | world::inter_pixel::dim2::is_dot());
  data::paste(morpho::erosion(extend(w_dots, pw::value(w_all)), c4().win()), w_all);


  // Save labels map.
  std::cout << "nbasins: " << nbasins << std::endl;
  io::dump::save(wst.unmorph_(), "watershed_fixed.dump");*/

  return 0;
}
