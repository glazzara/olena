#ifndef MLN_DEBUG_PRINTLN_HH
# define MLN_DEBUG_PRINTLN_HH

# include <core/concept/image.hh>
# include <core/concept/window.hh>
# include <core/box2d.hh>


namespace mln
{

  namespace debug
  {

    template <typename I>
    void println(const Image<I>& input);

    template <typename I, typename W>
    void println(const Image<I>& input_,
		 const Window<W>& win_);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename S, typename I>
      void println(const S&, const Image<I>& input_)
      {
	const I& input = exact(input_);
	mln_piter(I) p(input.domain());
	for_all(p)
	  std::cout << input(p) << ' ';
	std::cout << std::endl;
      }

      template <typename I>
      void println(const box2d& b,
		   const I& input)
      {
	for (int row = b.pmin().row(); row <= b.pmax().row(); ++row)
	  {
	    for (int col = b.pmin().col(); col <= b.pmax().col(); ++col)
	      std::cout << input(mk_point2d(row, col)) << ' ';
	    std::cout << std::endl;
	  }
      }

    } // end of namespace mln::debug::impl



    // facade

    template <typename I>
    void println(const Image<I>& input)
    {
      impl::println(exact(input).domain(), exact(input));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::debug

} // end of namespace mln


#endif // ! MLN_DEBUG_PRINTLN_HH
