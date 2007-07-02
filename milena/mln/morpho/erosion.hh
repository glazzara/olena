#ifndef MLN_MORPHO_EROSION_HH
# define MLN_MORPHO_EROSION_HH

# include <mln/core/concept/image.hh>
# include <mln/core/concept/window.hh>
// FIXME: # include <mln/border/assign.hh>
# include <mln/value/props.hh>


namespace mln
{

  namespace morpho
  {

    template <typename I, typename W>
    I erosion(const Image<I>& input, const Window<W>& win);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I, typename W>
      I erosion(const Image<I>& input_, const Window<W>& win_)
      {
	const I& input = exact(input_);
	const W& win   = exact(win_);

	typedef mln_value(I) value;

	I output(input.domain());
	mln_piter(I) p(input.domain());
	mln_qiter(W) q(win, p);
	for_all(p)
	  {
	    value v = mln_max(value);
	    for_all(q) if (input.owns_(q))
	      {
		if (input(q) < v)
		  v = input(q);
	      }
	    output(p) = v;
	  }
	return output;
      }

      // ...

    } // end of namespace mln::morpho::impl


    // facade

    template <typename I, typename W>
    I erosion(const Image<I>& input, const Window<W>& win)
    {
      return impl::erosion(exact(input), exact(win)); 
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_EROSION_HH
