#ifndef MLN_LEVEL_FILL_HH
# define MLN_LEVEL_FILL_HH

# include <core/concept/image.hh>


namespace mln
{

  namespace level
  {

    template <typename I>
    void fill(Image<I>& ima_,
	      const mln_value(I)& value);

    template <typename I>
    void fill(Image<I>& ima_,
	      mln_value(I) (*f)(const mln_point(I)& p));

    template <typename I>
    void fill(Image<I>& ima_,
	      const mln_value(I) array[]);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    void fill(Image<I>& ima_,
	      const mln_value(I)& value)
    {
      I& ima = exact(ima_);
      mln_piter(I) p(ima.domain());
      for_all(p)
	ima(p) = value;
    }

    template <typename I>
    void fill(Image<I>& ima_,
	      mln_value(I) (*f)(const mln_point(I)& p))
    {
      I& ima = exact(ima_);
      mln_piter(I) p(ima.domain());
      for_all(p)
	ima(p) = f(p);
    }

    template <typename I>
    void fill(Image<I>& ima_,
	      const mln_value(I) array[])
    {
      I& ima = exact(ima_);
      mln_piter(I) p(ima.domain());
      unsigned i = 0;
      for_all(p)
	ima(p) = array[i++];
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_FILL_HH
