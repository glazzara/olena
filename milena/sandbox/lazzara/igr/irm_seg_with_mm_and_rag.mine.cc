
#include <iterator>
#include <iostream>
#include <algorithm>

#include <mln/core/image/image2d.hh>

#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/image/dmorph/extension_fun.hh>
#include <mln/core/image/imorph/tr_image.hh>
#include <mln/core/var.hh>
#include <mln/win/rectangle2d.hh>

#include <mln/fun/p2b/big_chess.hh>
#include <mln/fun/x2x/all.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/pbm/save.hh>
#include <mln/io/ppm/save.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_16.hh>

#include <mln/data/transform.hh>

#include <mln/convert/to_window.hh>
#include <mln/convert/to_image.hh>
#include <mln/convert/to_fun.hh>

#include <mln/labeling/compute.hh>
#include <mln/labeling/background.hh>

#include <mln/data/convert.hh>

#include <mln/make/image3d.hh>

#include <mln/morpho/gradient.hh>
#include <mln/morpho/closing_area.hh>
#include <mln/morpho/meyer_wst.hh>

#include <mln/accu/mean.hh>
//#include <mln/data/take.hh>

#include <mln/util/graph.hh>

#include <mln/literal/colors.hh>

#include <mln/debug/println.hh>

#include <mln/registration/icp2.hh>

mln::value::int_u8 foo(unsigned u)
{
  return u == 0 ?
    0 : // wshed line
    1 + (u - 1) % 255; // basin
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

      io::ppm::save(slice(result,0), "tmp_final_registered.ppm");
    }



    template <typename I, typename T>
    void
    compare_registration2(Image<I>& P_, Image<I>& X_,
			  const T& transf)
    {
      I& P = exact(P_);
      const I& X = exact(X_);

      //FIXME: tr_image is bugged!
//      typedef extension_fun<I,pw::cst_<mln_value(I)> > ext_P_t;
//      ext_P_t ext_P(P, pw::cst(value::int_u8(0u)));
//      tr_image<box3d, ext_P_t, T> trima(P.domain(), ext_P, transf);
//      io::pgm::save(slice(trima, 0), "tmp_trima.ppm");

      I reg(X.domain());
      data::fill(reg, literal::zero);
      mln_piter(I) p(P.domain());
      for_all(p)
	if (reg.domain().has(transf(p.to_vec())))
	  reg(transf(p.to_vec())) = P(p);
      io::pgm::save(slice(reg,0), "tmp_registered-2.ppm");

      I tmp2 = duplicate(X);
      fun::p2b::big_chess<box3d> fun2(tmp2.domain(), 20);
      data::paste((reg | fun2), tmp2);
      io::pgm::save(slice(tmp2,0), "tmp_registration_filter-a.ppm");

//      I tmp = duplicate(X);
//      fun::p2b::big_chess<box3d> fun(tmp.domain(), 20);
//      data::paste((trima | fun), tmp);
//
//      io::pgm::save(slice(tmp,0), "tmp_registration_filter.ppm");
    }

  }


  struct threshold : Function_v2b<threshold>
  {
    bool operator()(const value::rgb8& val) const
    {
      unsigned inty = (val.red() + val.blue() + val.green());
      return inty > 100 && inty < 600;
    }

    bool operator()(const unsigned& val) const
    {
      return val < 120;// && val < 230;
    }
  };

  template <typename I>
  mln_ch_value(I,bool)
  fill_holes(const mln::Image<I>& ima)
  {
    using namespace mln;

    mln_ch_value(I,bool) output;
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
  mln_ch_value(I,bool)
  keep_largest_component(const Image<I>& ima)
  {
    using namespace mln;

    image2d<bool> in_bw_cleaned = fill_holes(ima);
//    io::pbm::save(in_bw_cleaned, "in_bw_cleaned.pbm");

    logical::not_inplace(in_bw_cleaned);
    image2d<bool> in_bw_cleaned_full = fill_holes(in_bw_cleaned);
//    io::pbm::save(in_bw_cleaned_full, "in_bw_cleaned_full.pbm");

    logical::not_inplace(in_bw_cleaned_full);
    return in_bw_cleaned_full;
  }

  template <typename I>
  mln_ch_value(I,bool)
  get_main_object_shape(const Image<I>& in)
  {
    using namespace mln;

    typedef image2d<bool> J;

//    threshold f;
//    J in_bw = binarization::binarization(in, f);
//    io::pbm::save(in_bw, "01_in_bw.pbm");

//    J ima = keep_largest_component(in_bw);
    J ima = keep_largest_component(in);

    std::cout << "Compute gradient" << std::endl;
    J ima_grad = morpho::gradient(ima, win_c4p());

    return ima_grad;
  }

  //FIXME: add conversion from label_16 to bool.
  struct to_bool : Function_v2v<to_bool>
  {
    typedef bool result;
    bool operator()(const value::label_16& l) const
    {
      return l != 0u;
    }

  };


  template <typename I>
  mln_ch_value(I,bool)
  get_cloud_image(const Image<I>& irm)
  {
    using namespace mln;
    using value::int_u8;
    using value::label_16;

    window2d c4p = convert::to_window(c4());
    c4p.insert(0,0);
    //   debug::println(convert::to_image(c4p));
    image2d<int_u8> grad = morpho::gradient(irm, c4p);
    io::pgm::save( grad, "tmp_grad_c4p.pgm" );

    image2d<int_u8> clo;
    initialize(clo, irm);

    clo = morpho::closing_area(grad, c4(), 100);
    io::pgm::save( clo, "tmp_clo_a100.pgm" );

    label_16 nbasins;
    image2d<label_16> wshed = morpho::meyer_wst(clo, c4(), nbasins);
    std::cout << "nbasins = " << nbasins << std::endl;
    io::pgm::save( data::transform(wshed, convert::to_fun(foo)),
	"tmp_wshed.pgm" );

    mln_VAR(mean_values, labeling::compute(accu::meta::mean(), irm, wshed, nbasins));
    fun::i2v::array<label_16> to_keep(nbasins.next(), 0);
    threshold f;
    for (label_16 i = 1; i < mean_values.nelements(); ++i)
      if (f(mean_values[i]))
	to_keep(i) = i;

    wshed = data::transform(wshed, to_keep);
    io::pgm::save( data::transform(wshed, convert::to_fun(foo)),
	"tmp_wshed_2.pgm" );

    image2d<bool> wshed_bw = data::transform(wshed, to_bool());
    image2d<bool> main_obj = get_main_object_shape(wshed_bw);
    io::pbm::save(main_obj, "tmp_main_obj");

    return main_obj;
  }

} // end of namespace mln


int main(int, char *argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::label_16;

  image2d<int_u8> irm;
  io::pgm::load(irm, argv[1]);

  image2d<int_u8> irm_ref;
  io::pgm::load(irm_ref, argv[2]);

  std::cout << "compute irm" << std::endl;
  image3d<bool> P = make::image3d(get_cloud_image(irm));
  std::cout << "compute irm_ref" << std::endl;
  image3d<bool> X = make::image3d(get_cloud_image(irm_ref));

  std::cout << "Registration" << std::endl;
  typedef fun::x2x::rotation<3u,float> rot_t;
  typedef fun::x2x::translation<3u,float> trans_t;
  typedef p_array<point3d> arr_t;
  arr_t P_arr = convert::to<arr_t>(P);
  arr_t X_arr = convert::to<arr_t>(X);
  fun::x2x::composed<trans_t,rot_t> qk = registration::registration(P_arr, X_arr);


  std::cout << "* Build result image" << std::endl;
  image3d<value::rgb8> registered = debug::make_registered_image(P, X, qk);
  debug::compare_registration(registered, X);



  image3d<int_u8> irm_wborders = make::image3d(duplicate(irm));
  data::fill((irm_wborders | P_arr).rw(), 255);
  io::pgm::save(slice(irm_wborders,0), "tmp_irm_with_borders.ppm");

  image3d<int_u8> ref_wborders = make::image3d(duplicate(irm_ref));
  data::fill((ref_wborders | X_arr).rw(), 255);
  io::pgm::save(slice(ref_wborders,0), "tmp_ref_with_borders.ppm");

  debug::compare_registration2(irm_wborders, ref_wborders, qk);

}
