#include <mln/core/image/dmorph/extended.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/routine/extend.hh>
#include <mln/core/var.hh>

#include <mln/data/transform.hh>

#include <mln/fun/p2b/big_chess.hh>

#include <mln/value/rgb8.hh>
#include <mln/literal/colors.hh>

#include <tests/data.hh>

#include <doc/tools/sample_utils.hh>

namespace mln
{

  struct saturate_rgb8 : public Function_v2v<saturate_rgb8>
  {

    typedef value::rgb8 result;

    value::rgb8 operator()(const value::rgb8& v) const
    {
      value::rgb8 v2 = v;
      v2.red() > 50 ? v2.red() -= 50 : v2.red() = 0;
      v2.green() > 50 ? v2.green() -= 50 : v2.green() = 0;
      v2.blue() > 50 ? v2.blue() -= 50 : v2.blue() = 0;
      return v2;
    }

  };

} // end of namespace mln

// \{
namespace mln
{

  struct my_ext : public Function_v2v<my_ext>
  {

    typedef value::rgb8 result;

    value::rgb8 operator()(const point2d& p) const
    {
      if ((p.row() + p.col()) % 20)
	return literal::black;
      return literal::white;
    }

  };

} // end of namespace mln
// \}

int main()
{
  using namespace mln;
  using value::rgb8;

  border::thickness = 5;

  // \{
  image2d<rgb8> lena;
  io::ppm::load(lena, MLN_IMG_DIR "/small.ppm");
  box2d bbox_enlarged = lena.domain();
  bbox_enlarged.enlarge(border::thickness);
  mln_VAR(ima_roi, lena | fun::p2b::big_chess<box2d>(lena.domain(), 10));
  // \}

  image2d<rgb8> tmp;
  initialize(tmp, lena);
  data::fill(tmp, literal::black);
  data::paste(ima_roi, tmp);
  doc::ppmsave(tmp, "extend");

  // \{
  mln_VAR(ext_with_val, extended_to(extend(ima_roi, literal::blue), bbox_enlarged));
  // \}
  doc::ppmsave(ext_with_val, "extend");

  // \{
  mln_VAR(ext_with_fun, extended_to(extend(ima_roi, my_ext()), bbox_enlarged));
  // \}
  doc::ppmsave(ext_with_fun, "extend");


  // \{
  mln_VAR(ext_with_ima, extend(ima_roi, lena));
  // \}
  doc::ppmsave(ext_with_ima, "extend");

  image2d<bool> mask;
  initialize(mask, lena);
  data::fill(mask, true);
  data::fill((mask | ima_roi.domain()).rw(), false);
  mln_VAR(ima_ext, data::transform(lena | (pw::value(mask) != false), saturate_rgb8()));
  data::paste(ima_ext, lena);
  data::paste(ima_roi, lena);
  doc::ppmsave(lena, "extend");

}
