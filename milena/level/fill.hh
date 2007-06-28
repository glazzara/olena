#ifndef MLN_LEVEL_FILL_HH
# define MLN_LEVEL_FILL_HH

# include <core/concept/image.hh>


namespace mln
{

  namespace level
  {

    template <typename I>
    void fill(Image<I>& ima_, const oln_value(I)& value);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    void fill(Image<I>& ima_, const oln_value(I)& value)
    {
      I& ima = exact(ima_);
      oln_piter(I) p(ima.domain());
      for_all(p)
	ima(p) = value;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_FILL_HH
