#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/label_16.hh>

#include <mln/labeling/background.hh>
#include <mln/labeling/compute.hh>
#include <mln/accu/math/count.hh>
#include <mln/accu/shape/bbox.hh>

#include <mln/fun/i2v/array.hh>
#include <mln/data/transform.hh>


namespace mln
{

  image2d<bool>
  clean_input(const image2d<bool>& input)
  {
    int max_col = input.domain().pmax().col();

    typedef value::label_16 L;
    L nlabels;
    image2d<L> lab = labeling::background(input, c8(), nlabels);
    util::array<unsigned> count = labeling::compute(accu::math::count<point2d>(), lab, nlabels);
    util::array<box2d>    box   = labeling::compute(accu::shape::bbox<point2d>(), lab, nlabels);

    fun::i2v::array<bool> ok(nlabels.next(), false); // default is text
    ok(0) = true; // bg is 'white'
    for (L l = 1; l <= nlabels; ++l)
      {
	if (count[l] <= 3)
	  {
	    ok(l) = true; // into bg
	    continue;
	  }
	int center_col = box[l].pcenter().col();
	if (center_col >= max_col - 3 ||
	    center_col <= 3)
	  ok(l) = true;
      }
    return data::transform(lab, ok);
  }

} // mln
