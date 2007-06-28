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
    struct props<bool>
    {
      static bool min() { return false; }
      static bool max() { return true; }
    };

    // integers

    template <>
    struct props<unsigned char>
    {
      static unsigned char min() { return 0; }
      static unsigned char max() { return 255; }
    };

    template <>
    struct props<signed char>
    {
      static signed char min() { return -128; }
      static signed char max() { return  127; }
    };

    template <>
    struct props<unsigned short>
    {
      static unsigned short min() { return 0; }
      static unsigned short max() { return 65535; }
    };

    template <>
    struct props<signed short>
    {
      static signed short min() { return -32768; }
      static signed short max() { return  32767; }
    };

    template <>
    struct props<unsigned int>
    {
      static unsigned int min() { return std::numeric_limits<unsigned int>::min(); }
      static unsigned int max() { return std::numeric_limits<unsigned int>::max(); }
    };

    template <>
    struct props<signed int>
    {
      static signed int min() { return std::numeric_limits<signed int>::min(); }
      static signed int max() { return std::numeric_limits<signed int>::max(); }
    };

    template <>
    struct props<unsigned long int>
    {
      static unsigned long int min() { return std::numeric_limits<unsigned long int>::min(); }
      static unsigned long int max() { return std::numeric_limits<unsigned long int>::max(); }
    };

    template <>
    struct props<signed long int>
    {
      static signed long int min() { return std::numeric_limits<signed long int>::min(); }
      static signed long int max() { return std::numeric_limits<signed long int>::max(); }
    };

    // floating

    template <>
    struct props<float>
    {
      static float min() { return std::numeric_limits<float>::min(); }
      static float max() { return std::numeric_limits<float>::max(); }
    };

    template <>
    struct props<double>
    {
      static double min() { return std::numeric_limits<double>::min(); }
      static double max() { return std::numeric_limits<double>::max(); }
    };

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_PROPS_HH
