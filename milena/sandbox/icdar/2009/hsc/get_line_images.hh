#ifndef GET_LINE_IMAGES_HH
# define GET_LINE_IMAGES_HH

#include <mln/core/concept/image.hh>
#include <mln/core/site_set/box.hh>
#include <mln/core/image/dmorph/extended.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/core/routine/extend.hh>
#include <mln/core/var.hh>

#include <mln/data/fill.hh>

#include <mln/accu/bbox.hh>

#include <mln/geom/bbox.hh>

#include <mln/labeling/compute.hh>

#include <mln/util/array.hh>

#include <mln/pw/all.hh>
#include <mln/core/image/dmorph/image_if.hh>


namespace mln
{

  template <typename L>
  util::array<mln_ch_value(L,bool)>
  inline
  get_line_images(const Image<L>& lines_, const mln_value(L)& nlines,
		  unsigned bbox_line_enlarge)
  {
    trace::entering("get_line_images");

    const L& lines = exact(lines_);
    mln_precondition(lines.is_valid());

    util::array<box<mln_site(L)> >
      bboxes = labeling::compute(accu::meta::bbox(), lines, nlines);

    typedef mln_ch_value(L,bool) line_t;
    util::array<line_t> result;

    {
      // Skipping index 0:
      line_t dummy(1,1);
      result.append(dummy);
    }

    for (unsigned i = 1; i <= nlines; ++i)
    {
      box<mln_site(L)> b = bboxes[i];
      b.enlarge(bbox_line_enlarge);
      line_t line(b);
      data::fill(line, false);
      data::fill(((line | bboxes[i]).rw() | (pw::value(lines) == i)).rw(), true);

      result.append(line);
    }

    trace::exiting("get_line_images");
    return result;
  }


} // end of namespace mln

#endif // ! GET_LINE_IMAGES_HH
