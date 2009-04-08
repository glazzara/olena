#ifndef CLEAN_LINESWITH_GROUPED_BBOXES_HH
# define CLEAN_LINESWITH_GROUPED_BBOXES_HH

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
#include <mln/core/image/image_if.hh>
#include <mln/data/fill.hh>
#include <mln/literal/colors.hh>
#include <mln/logical/not.hh>
#include "label_maj.hh"

#include <scribo/text/grouping/group_with_single_left_link.hh>
#include <scribo/text/grouping/group_with_single_right_link.hh>
#include <scribo/text/grouping/group_from_double_link.hh>
#include <scribo/make/text.hh>
#include <scribo/util/text.hh>
#include <scribo/debug/save_textbboxes_image.hh>

namespace mln
{

  template <typename I, typename L>
  mln_concrete(L)
  clean_lines_with_grouped_bboxes(const Image<I>& input,
				  const Image<L>& lines_,
				  float maj_threshold);


# ifndef MLN_INCLUDE_ONLY

  template <typename I, typename L>
  mln_concrete(L)
  clean_lines_with_grouped_bboxes(const Image<I>& input,
				  const Image<L>& lines_,
				  float maj_threshold)
  {
    trace::entering("clean_lines_with_grouped_bboxes");

    const L& lines = exact(lines_);
    mln_precondition(lines.is_valid());
    typedef value::label_16 LV;

    LV nlabels;
    scribo::util::text<mln_ch_value(I,LV)>
      text = scribo::make::text(logical::not_(input), c8(), nlabels);

    //Link character bboxes to their left neighboor if possible.
    mln::util::array<unsigned>
      left_link = scribo::text::grouping::group_with_single_left_link(text, 30);
    mln::util::array<unsigned>
      right_link = scribo::text::grouping::group_with_single_right_link(text, 30);

    text = scribo::text::grouping::group_from_double_link(text,
							  left_link,
							  right_link);


//    mln_ch_value(L,LV) lbl = labeling::background(input, c8(), nlabels);
//    util::array<box<mln_site(L)> >
//      bboxes = labeling::compute(accu::meta::bbox(), lbl, nlabels);

    nlabels = text.nbboxes();
    const image2d<value::label_16>& lbl = text.label_image();
    util::array<box<mln_site(L)> >
      bboxes = labeling::compute(accu::meta::bbox(), lbl, nlabels);
    text.bboxes() = bboxes;
    scribo::debug::save_textbboxes_image(input, text.bboxes(), literal::red, "plop.ppm");

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
		      | (pw::value(lbl) == pw::cst(i))).rw(),
		   res[i].first());
//      else
//	std::cout << res[i].first() << " - " << res[i].second() << std::endl;

    trace::exiting("clean_lines_with_grouped_bboxes");
    return output;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // CLEAN_LINESWITH_GROUPED_BBOXES_HH
