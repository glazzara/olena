#ifndef MLN_LEVEL_PASTE_HH
# define MLN_LEVEL_PASTE_HH

# include <mln/core/concept/image.hh>


namespace mln
{

  namespace level
  {

    template <typename I, typename J>
    void paste(const Image<I>& data, Image<J>& destination);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename J>
    void paste(const Image<I>& data_, Image<J>& destination_)
    {
      const I& data  = exact(data_);
      I& destination = exact(destination_);
      assert(ima.has_data() && destination.has_data());

      mln_piter(I) p(data.domain());
      for_all(p)
	destination(p) = data(p);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_PASTE_HH
