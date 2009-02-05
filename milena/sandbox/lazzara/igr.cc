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

  using namespace fun::x2x;

    template <typename P>
    struct transf_quat_t
    {
      transf_quat_t()
	: q_(1,0,0,0), t_(literal::zero)
      {
      }


      inline
	float epsilon()
	{
	  static const float e = 1e-5;
	  return e;
	}

      inline
	bool about_equal(float val1, float val2)
	{
	  return fabs(val1 - val2) < epsilon();
	}

      template <unsigned n>
	algebra::vec<n,float> rotate(const algebra::quat& q, const algebra::vec<n,float>& p)
	{
	  return (q * algebra::quat(0. ,p) * q.inv()).v();
	}


      bool check_rotation(const algebra::h_mat<3,float>& mat, const algebra::quat& q)
      {
	srand(time(0));
	assert(q.is_unit());
	rotation<3,float> rot(mat);

	algebra::vec<3,float>
	  tmp = make::vec(rand(), rand(), rand()),
	      p = tmp / norm::l2(tmp),
	      p_rot_1 = rotate(q, p),
	      p_rot_2 = rot(p);
	return about_equal(norm::l2(p_rot_1 - p_rot_2), 0.f);
      }


      transf_quat_t(const algebra::quat& q, const vec3d_f& t)
	: q_(q), t_(t)
      {
	assert(q.is_unit());
	float
	  w = q.to_vec()[0],
	    x = q.to_vec()[1],  x2 = 2*x*x,  xw = 2*x*w,
	    y = q.to_vec()[2],  y2 = 2*y*y,  xy = 2*x*y,  yw = 2*y*w,
	    z = q.to_vec()[3],  z2 = 2*z*z,  xz = 2*x*z,  yz = 2*y*z,  zw = 2*z*w;
	float data[9] = {1.f - y2 - z2,  xy - zw,  xz + yw,
	  xy + zw,  1.f - x2 - z2,  yz - xw,
	  xz - yw,  yz + xw,  1.f - x2 - y2};

	algebra::h_mat<3,float> tmp = make::h_mat(data);
	std::cout << tmp << std::endl;
	// postcondition
	assert(check_rotation(tmp, q));
      }

      void
      set_quat(const algebra::quat& q)
      {
	q_ = q;
      }


      void
      set_trans(const vec3d_f& t)
      {
	t_ = t;
      }

      algebra::vec<P::dim,float>
      operator()(const algebra::vec<P::dim,float>& v) const
      {
	return (q_ * algebra::quat(0., v) * q_.inv()).v() + t_;
      }

      algebra::quat q_;
      vec3d_f t_;
    };


    template <typename P>
    struct transf_mat_t
    {
      typedef rotation<P::dim,float> rot_t;
      typedef translation<P::dim,float> trans_t;

      transf_mat_t() {}
      transf_mat_t(const algebra::quat& q, const vec3d_f& t)
	: r_(q), t_(t), c_(r_, t_)
      {
      }

      void
      set_quat(const algebra::quat& q)
      {
	r_ = rot_t(q);
      }

      void
      set_trans_(const vec3d_f& t)
      {
	t_ = trans_t(t);
      }

      algebra::vec<P::dim,float>
      operator()(const algebra::vec<P::dim,float>& v) const
      {
	return c_(v);
      }

      rot_t r_;
      trans_t t_;
      composed<rot_t, trans_t> c_;
    };


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

//  threshold f;
//  J in_bw = binarization::binarization(in, f);
//  io::pbm::save(in_bw, "01_in_bw.pbm");

//  J ima = keep_largest_component(in_bw);
  J ima = keep_largest_component(in);
//  io::pbm::save(in_bw, "02_ima.pbm");
  io::pbm::save(in, "ima.pbm");

  std::cout << "Compute gradient" << std::endl;
  J ima_grad = morpho::gradient(ima, win_c4p());
  io::pbm::save(ima_grad, "ima_grad.pbm");

  return ima_grad;
}


namespace mln
{

  namespace debug
  {
    template <typename I, typename T>
    void
    compare_registration(Image<I>& P_, Image<I>& X_, const T& transf)
    {
      I& P = exact(P_);
      I& X = exact(X_);

      mln_pset(I) box = geom::bbox(X);
      box.enlarge(40);

      typedef mln_ch_value(I,value::rgb8) result_t;
      result_t result(box);
      extension_fun<result_t,pw::cst_<mln_value(result_t)> > ext_result(result, pw::cst(value::rgb8(0,0,0)));
      extension_fun<I,pw::cst_<mln_value(I)> > ext_X(X, pw::cst(false));

      data::fill(ext_result, literal::black);
      data::fill((ext_result | (pw::value(ext_X) == true)).rw(), literal::white);

      mln_VAR(ig, (P | pw::value(P) == true));
      mln_piter(ig_t) p(ig.domain());
      for_all(p)
	ext_result(transf(p.to_vec())) = literal::green;

      io::ppm::save(slice(ext_result,0), "registered-1.ppm");
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

//  I in;
//  J in;
//  io::pbm::load(in, argv[1]);
//  J in_grad = get_main_object_shape(in);

//  I ref;
//  J ref;
//  io::pbm::load(ref, argv[2]);
//  J ref_grad = get_main_object_shape(ref);

  typedef image3d<bool> K;

  p_array<point3d> in_3d_, ref_3d_;
  io::cloud::load(in_3d_, argv[1]);
  io::cloud::load(ref_3d_, argv[2]);

  std::cout << "* loading data" << std::endl;
  std::cout << "    igr.cc - in_3d_.nsites = " << in_3d_.nsites() << std::endl;
  std::cout << "    igr.cc - ref_3d_.nsites = " << ref_3d_.nsites() << std::endl;
  K in_3d = convert::to<K>(in_3d_);
  K ref_3d = convert::to<K>(ref_3d_);

  registration::closest_point_with_map<point3d> closest_point(ref_3d_);
//  registration::closest_point_basic<point3d> closest_point(ref_3d_);

  util::timer t;
  t.start();

  typedef rotation<3u,float> rot_t;
  typedef translation<3u,float> trans_t;
  composed<trans_t,rot_t> qk = registration::icp_clean2(in_3d_, ref_3d_, closest_point);

  std::cout << "igr.cc - Registration - " << t << std::endl;

  std::cout << "* Build result image" << std::endl;
  debug::compare_registration(in_3d, ref_3d, qk);

}
