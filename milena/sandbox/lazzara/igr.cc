#include <mln/essential/2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/binarization/binarization.hh>
#include <mln/labeling/background.hh>

#include <mln/debug/colorize.hh>

//#include <mln/registration/registration.hh>
//#include <mln/registration/multiscale.hh>

#include <mln/essential/3d.hh>
#include <mln/registration/icp2.hh>
#include <mln/core/image/slice_image.hh>

struct threshold : mln::Function_p2b<threshold>
{
  bool operator()(const mln::value::rgb8& val) const
  {
    unsigned inty = (val.red() + val.blue() + val.green());
    return inty > 100 && inty < 600;
  }
};


template <typename T>
inline
mln::image3d<T>
to_image3d(const mln::image2d<T>& img)
{
  mln::point3d pmin(img.domain().pmin()[0], img.domain().pmin()[1], 0);
  mln::point3d pmax(img.domain().pmax()[0], img.domain().pmax()[1], 0);
  mln::image3d<T> img3d(mln::box3d(pmin,pmax));

  mln_piter(mln::image3d<T>) p(img3d.domain());
  for_all(p)
    img3d(p) = exact(img)(mln::point2d(p[0],p[1]));

  return img3d;
}

template <typename T>
inline
mln::point<mln::grid::square, T>
to_point2d(const mln::point<mln::grid::cube, T>& p)
{
  return mln::point<mln::grid::square, T>(p[0], p[1]);
}

//point2d -> point3d
template <typename T>
inline
mln::point<mln::grid::cube, T>
to_point3d(const mln::point<mln::grid::square, T>& p)
{
  return mln::point<mln::grid::cube, T>(p[0], p[1], 0);
}

template <typename I>
mln_concrete(I)
fill_holes(const mln::Image<I>& ima)
{
  using namespace mln;

  mln_concrete(I) output;
  initialize(output, ima);
  data::fill(output, true);

  value::label_16 nlabels;
  mln_ch_value(I, value::label_16) lbl = labeling::background(ima, c4(), nlabels);

  mln_VAR(lbl_count, labeling::compute(accu::meta::count(), ima, lbl, nlabels));

  unsigned max_id = 1;
  for (unsigned i = 2; i <= nlabels; ++i)
    if (lbl_count[max_id] < lbl_count[i])
      max_id = i;

  data::fill((output | pw::value(lbl) == max_id).rw(), false);

  return output;
}


template <typename I>
mln_concrete(I)
keep_largest_component(const mln::Image<I>& ima)
{
  using namespace mln;

  image2d<bool> in_bw_cleaned = fill_holes(ima);
  io::pbm::save(in_bw_cleaned, "0x_in_bw_cleaned.pbm");

  logical::not_inplace(in_bw_cleaned);
  image2d<bool> in_bw_cleaned_full = fill_holes(in_bw_cleaned);
  io::pbm::save(in_bw_cleaned_full, "0x_in_bw_cleaned_full.pbm");

  logical::not_inplace(in_bw_cleaned_full);
  return in_bw_cleaned_full;
}

template <typename I>
mln_ch_value(I,bool)
get_main_object_shape(const mln::Image<I>& in)
{
  using namespace mln;

  typedef image2d<bool> J;

  threshold f;
  J in_bw = binarization::binarization(in, f);
  io::pbm::save(in_bw, "01_in_bw.pbm");

  J ima = keep_largest_component(in_bw);
  io::pbm::save(in_bw, "02_ima.pbm");

  std::cout << "Compute gradient" << std::endl;
  J ima_grad = morpho::gradient(ima, win_c4p());
  io::pbm::save(ima_grad, "03_ima_grad.pbm");

  return ima_grad;
}


int main(int, char* argv[])
{
  using namespace mln;
  using namespace fun::x2x;
  using value::rgb8;
  using value::int_u8;
  using value::label_16;

  //Load image
  typedef image2d<rgb8> I;
  typedef image2d<bool> J;

  I in;
  io::ppm::load(in, argv[1]);
  J in_grad = get_main_object_shape(in);

  I ref;
  io::ppm::load(ref, argv[2]);
  J ref_grad = get_main_object_shape(ref);

  std::cout << "Computing registration..." << std::endl;
  J cloud = in_grad | pw::value(in_grad) == true; //FIXME: cannot use pw::image with registration.
  //  mln_VAR(surface, (ref_grad | pw::value(ref_grad) == true));


  //mln_VAR(registration, registration::multiscale(cloud, ref_grad, 5, 3));
  typedef image3d<bool> K;
  K in_3d = make::image3d(in_grad);
  K ref_3d = make::image3d(ref_grad);
  composed< rotation<K::psite::dim, float>, translation<K::psite::dim, float> > qk;
  registration::icp(in_3d, ref_3d, qk);


  std::cout << "Build result image" << std::endl;
  K res;
  initialize(res, in_3d);
  data::fill(res, false);

  mln_VAR(ig, (in_3d | pw::value(in_3d) == true));
  mln_piter_(ig_t) p(ig.domain());
  for_all(p)
    res(qk(p.to_site().to_vec())) = true;

  io::pbm::save(slice(res,0), "04_registered.pbm");
}
