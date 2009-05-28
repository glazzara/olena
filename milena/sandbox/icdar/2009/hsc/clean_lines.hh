#ifndef CLEAN_LINES_HH
# define CLEAN_LINES_HH

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/labeling/background.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>
#include <mln/value/label_16.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/all.hh>
#include <mln/labeling/compute.hh>
#include <mln/pw/all.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/data/fill.hh>
#include "label_maj.hh"


namespace mln
{

  template <typename I, typename L>
  mln_concrete(L)
  clean_lines(const Image<I>& input, const Image<L>& lines_,
	      float maj_threshold);


# ifndef MLN_INCLUDE_ONLY

  template <typename I, typename L>
  mln_concrete(L)
  clean_lines(const Image<I>& input, const Image<L>& lines_,
	      float maj_threshold)
  {
    trace::entering("clean_lines");

    const L& lines = exact(lines_);
    mln_precondition(lines.is_valid());
    typedef value::label_16 LV;

    LV nlabels;
    mln_ch_value(L,LV) lbl = labeling::background(input, c8(), nlabels);
    util::array<box<mln_site(L)> > bboxes = labeling::compute(accu::meta::bbox(), lbl, nlabels);

    /// Compute the most represented label for each component.
    accu::label_maj<LV, mln_value(L)> accu(nlabels.next());
    mln_piter(image2d<mln_value(L)>) p(lbl.domain());
    for_all(p)
      if (lines(p) != 0u)
	accu.take(lbl(p), lines(p));

    mln_concrete(L) output = duplicate(lines);

    // Rebuild components.
    util::array<util::couple<mln_value(L), float> > res = accu.to_result();
    for (unsigned i = 1; i < res.nelements(); ++i)
      if (res[i].second() >= maj_threshold)
	data::fill(((output | bboxes[i]).rw()
		      | (pw::value(lbl) != pw::cst(literal::zero))).rw(),
		   res[i].first());
//      else
//	std::cout << res[i].first() << " - " << res[i].second() << std::endl;

    trace::exiting("clean_lines");
    return output;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // CLEAN_LINES_HH
