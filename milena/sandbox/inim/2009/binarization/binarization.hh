#include <mln/histo/all.hh>
#include <mln/convert/to_image.hh>
#include <mln/core/image/image1d.hh>
#include <mln/core/alias/neighb1d.hh>
#include <mln/arith/revert.hh>
#include <mln/morpho/closing_area.hh>
#include <mln/morpho/meyer_wst.hh>
#include <mln/border/resize.hh>
#include <mln/accu/maj_h.hh>
#include <mln/opt/at.hh>
#include "snake2d.hh"

namespace mln
{

  namespace binarization
  {
    template<typename I, typename W>
    mln_ch_value(I, bool)
    binarization(const Image<I>& input, const Window<W>& win, int strength);

# ifndef MLN_INCLUDE_ONLY
    namespace impl
    {
      template<typename I, typename W>
      mln_ch_value(I, bool)
      binarization_(const I& input, const W& win, int strength)
      {
	trace::entering("binarisation_");
	mln_ch_value(I, bool) output(input.domain());

	histo::data<mln_value(I)> hist = histo::compute (input);
	image1d<std::size_t> hist_img = convert::to_image(hist);
	image1d<std::size_t> hist_revert_closed;
	initialize(hist_revert_closed, hist_img);
	morpho::closing_area(arith::revert(hist_img), c2(), strength, hist_revert_closed);
	int nbasins;
	image1d<int> wst = morpho::meyer_wst(hist_revert_closed, c2(), nbasins);
//	if (wst(wst.nelements() - 1) == 0)
//	  wst(p) = wst(p++);
	I cla;
	initialize(cla, input);
	typename I::fwd_piter q (input.domain());
	for_all (q)
	  if (opt::at(wst, input(q)) == 0)
	    cla(q) = opt::at(wst, input(q) + 1);
	  else
	    cla(q) = opt::at(wst, input(q));
	border::resize(cla, 0);
	I maj = snake2d(accu::meta::maj_h(), cla, win);
	typename I::fwd_piter r (cla.domain());
	for_all (r)
	  output(r) = (cla(r) != maj(r));

	trace::exiting("binarisation_");
	return output;
      }
    }

    template<typename I, typename W>
    mln_ch_value(I, bool)
    binarization(const Image<I>& input, const Window<W>& win, int strength)
    {
      trace::entering("binarisation");
      mln_precondition(exact(input).is_valid());

      mln_ch_value(I, bool) output(exact(input).domain());
      output = impl::binarization_(exact(input), exact(win), strength);

      trace::exiting("binarisation");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::binarization

} // end of namespace mln
