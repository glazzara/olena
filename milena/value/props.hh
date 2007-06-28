#ifndef MLN_VALUE_PROPS_HH
# define MLN_VALUE_PROPS_HH

# include <limits>

# define mln_min(T) mln::value::props<T>::min()
# define mln_max(T) mln::value::props<T>::max()


namespace mln
{

  namespace value
  {

    template <typename T>
    struct props
    {
      static T min();
      static T max();
    };


# ifndef MLN_INCLUDE_ONLY

    template <>
    struct props<unsigned char>
    {
      static unsigned char min() { return 0; }
      static unsigned char max() { return 255; }
    };

    template <>
    struct props<int>
    {
      static int min() { return std::numeric_limits<int>::min(); }
      static int max() { return std::numeric_limits<int>::max(); }
    };

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_PROPS_HH
