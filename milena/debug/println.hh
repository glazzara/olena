#ifndef MLN_DEBUG_PRINTLN_HH
# define MLN_DEBUG_PRINTLN_HH

# include <core/concept/image.hh>
# include <core/concept/window.hh>


namespace mln
{

  namespace debug
  {

    template <typename I>
    void println(const Image<I>& input_);

    template <typename I, typename W>
    void println(const Image<I>& input_,
		 const Window<W>& win_);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    void println(const Image<I>& input_)
    {
      const I& input = exact(input_);
      oln_piter(I) p(input.domain());
      for_all(p)
	std::cout << input(p) << ' ';
      std::cout << std::endl;
    }


    template <typename I, typename W>
    void println(const Image<I>&  input_,
		 const Window<W>& win_)
    {
      const I& input = exact(input_);
      const W& win   = exact(win_);

      oln_piter(I) p(input.domain());
      oln_qiter(W) q(win, p);
      for_all(p)
	{
	  std::cout << input(p) << ':';
	  for_all(q)
	    if (input.owns_(q))
	      std::cout << input(q) << ' ';
	  std::cout << std::endl;
	}
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::debug

} // end of namespace mln


#endif // ! MLN_DEBUG_PRINTLN_HH
