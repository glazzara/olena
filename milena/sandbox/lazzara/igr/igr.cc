#include <mln/registration/icp2.hh>
#include <mln/essential/2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/binarization/binarization.hh>
#include <mln/labeling/background.hh>

#include <mln/debug/colorize.hh>

//#include <mln/registration/registration.hh>
//#include <mln/registration/multiscale.hh>

#include <mln/essential/3d.hh>
#include <mln/core/image/slice_image.hh>
#include <mln/core/image/tr_image.hh>
#include <mln/core/image/interpolated.hh>

#include <mln/io/cloud/load.hh>
#include <mln/util/timer.hh>
#include <mln/core/concept/function.hh>
#include <mln/trait/ch_value.hh>

#include <mln/fun/p2b/big_chess.hh>

struct threshold : mln::Function_p2b<threshold>
{
  bool operator()(const mln::value::rgb8& val) const
  {
    unsigned inty = (val.red() + val.blue() + val.green());
    return inty > 100 && inty < 600;
  }
};

namespace mln
{

  namespace registration
  {
    extern std::string method;
  }

}

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
  io::pbm::save(in_bw_cleaned, "in_bw_cleaned.pbm");

  logical::not_inplace(in_bw_cleaned);
  image2d<bool> in_bw_cleaned_full = fill_holes(in_bw_cleaned);
  io::pbm::save(in_bw_cleaned_full, "in_bw_cleaned_full.pbm");

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
//  J ima = keep_largest_component(in);
  io::pbm::save(ima, "02_ima.pbm");

  std::cout << "Compute gradient" << std::endl;
  J ima_grad = morpho::gradient(ima, win_c4p());

  return ima_grad;
}


namespace mln
{

  namespace debug
  {

    template <typename I, typename T>
    image3d<value::rgb8>
    make_registered_image(Image<I>& P_, Image<I>& X_, const T& transf)
    {
      I& P = exact(P_);
      I& X = exact(X_);

      mln_pset(I) box = geom::bbox(X);
      box.enlarge(1, 60);
      box.enlarge(2, 60);

      typedef mln_ch_value(I,value::rgb8) result_t;
      result_t result(box);
      extension_fun<result_t,pw::cst_<mln_value(result_t)> > ext_result(result, pw::cst(value::rgb8(0,0,0)));
      extension_fun<I,pw::cst_<mln_value(I)> > ext_X(X, pw::cst(false));
      data::fill(ext_result, literal::black);

      mln_VAR(ig, (P | pw::value(P) == true));
      mln_piter(ig_t) p(ig.domain());
      for_all(p)
	ext_result(transf(p.to_vec())) = literal::green;

      return ext_result;
    }

    template <typename I, typename J>
    void
    compare_registration(Image<I>& registered_, Image<J>& X_)
    {
      I& registered = exact(registered_);
      J& X = exact(X_);

      typedef mln_ch_value(I,value::rgb8) result_t;
      result_t result;
      initialize(result, registered);
      extension_fun<J,pw::cst_<mln_value(J)> > ext_X(X, pw::cst(false));

      data::fill(result, literal::black);
      data::fill((result | (pw::value(ext_X) == true)).rw(), literal::white);
      data::fill((result | (pw::value(registered) != pw::cst(literal::black))).rw(),
		 literal::green);

      io::ppm::save(slice(result,0), "final_registered.ppm");
    }



    template <typename I, typename T>
    void
    compare_registration2(Image<I>& P_, Image<I>& X_,
			  const T& transf)
    {
      I& P = exact(P_);
      const I& X = exact(X_);

      typedef extension_fun<I,pw::cst_<mln_value(I)> > ext_P_t;
      ext_P_t ext_P(P, pw::cst(value::rgb8(literal::black)));
      typedef interpolated<ext_P_t, fun::x2v::bilinear> inter_t;
      inter_t inter(ext_P);
      tr_image<box3d, inter_t, T> trima(X.domain(), inter, transf);
      io::ppm::save(slice(trima, 0), "trima.ppm");

      I tmp = duplicate(X);
      fun::p2b::big_chess<box3d> fun(tmp.domain(), 20);
      data::paste((trima | fun), tmp);

      io::ppm::save(slice(tmp,0), "registration_filter.ppm");
    }

  }
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
  typedef image3d<bool> K;
  typedef image3d<value::rgb8> L;

  I in;
  io::ppm::load(in, argv[1]);
  J in_obj = get_main_object_shape(in);
  io::pbm::save(in_obj, "in_obj.pbm");

  I ref;
  io::ppm::load(ref, argv[2]);
  J ref_obj = get_main_object_shape(ref);
  io::pbm::save(ref_obj, "ref_obj.pbm");

//  p_array<point3d> in_3d_, ref_3d_;
//  io::cloud::load(in_3d_, argv[1]);
//  io::cloud::load(ref_3d_, argv[2]);
//
//  std::cout << "* loading data" << std::endl;
//  std::cout << "    igr.cc - in_3d_.nsites = " << in_3d_.nsites() << std::endl;
//  std::cout << "    igr.cc - ref_3d_.nsites = " << ref_3d_.nsites() << std::endl;
//  K in_3d = convert::to<image3d<bool> >(in_3d_);
//  K ref_3d = convert::to<image3d<bool> >(ref_3d_);
//  io::pbm::save(slice(in_3d,0), "in_shape.ppm");
//  io::pbm::save(slice(ref_3d,0), "ref_shape.ppm");
//  K in = duplicate(in_3d);
//  K ref = duplicate(ref_3d);

  K in_3d = make::image3d(in_obj);
  K ref_3d = make::image3d(ref_obj);
  std::cout << "    igr.cc - in_3d.nsites = " << in_3d.nsites() << std::endl;
  std::cout << "    igr.cc - ref_3d.nsites = " << ref_3d.nsites() << std::endl;

  typedef p_array<point3d> p_arr_t;
  p_arr_t in_3d_ = convert::to<p_arr_t>(in_3d);
  p_arr_t ref_3d_ = convert::to<p_arr_t>(ref_3d);


/// Add objects shapes in original images.
  L in_wborders = make::image3d(duplicate(in));
  data::fill((in_wborders | in_3d_).rw(), literal::green);
  io::ppm::save(slice(in_wborders,0), "in_with_borders.ppm");

  L ref_wborders = make::image3d(duplicate(ref));
  data::fill((ref_wborders | ref_3d_).rw(), literal::green);
  io::ppm::save(slice(ref_wborders,0), "ref_with_borders.ppm");



  // Starting registration.
  util::timer t;
  t.start();
  typedef rotation<3u,float> rot_t;
  typedef translation<3u,float> trans_t;
  composed<trans_t,rot_t> qk = registration::registration(in_3d_, ref_3d_);
  std::cout << "igr.cc - Registration - " << t << "s" << std::endl;


  std::cout << "* Build result image" << std::endl;
  image3d<value::rgb8> registered = debug::make_registered_image(in_3d, ref_3d, qk);
  debug::compare_registration(registered, ref_3d);

  debug::compare_registration2(in_wborders, ref_wborders, qk);

}
