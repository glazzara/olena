#ifndef MLN_LEVEL_FILL_HH
# define MLN_LEVEL_FILL_HH

# include <mln/core/concept/image.hh>


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

    template <typename I, typename J>
    void fill(Image<I>& ima_,
	      const Image<J>& data);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    void fill(Image<I>& ima_,
	      const mln_value(I)& value)
    {
      I& ima = exact(ima_);
      assert(ima.has_data());
      mln_piter(I) p(ima.domain());
      for_all(p)
	ima(p) = value;
    }

    template <typename I>
    void fill(Image<I>& ima_,
	      mln_value(I) (*f)(const mln_point(I)& p))
    {
      I& ima = exact(ima_);
      assert(ima.has_data());
      mln_piter(I) p(ima.domain());
      for_all(p)
	ima(p) = f(p);
    }

    template <typename I>
    void fill(Image<I>& ima_,
	      const mln_value(I) array[])
    {
      I& ima = exact(ima_);
      assert(ima.has_data());
      mln_piter(I) p(ima.domain());
      unsigned i = 0;
      for_all(p)
	ima(p) = array[i++];
    }

    template <typename I, typename J>
    void fill(Image<I>& ima_,
	      const Image<J>& data_)
    {
      I&        ima = exact(ima_);
      const J& data = exact(data_);
      assert(ima.has_data() and data.has_data());

      mln_piter(I) p(ima.domain());
      for_all(p)
	ima(p) = data(p);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_FILL_HH
