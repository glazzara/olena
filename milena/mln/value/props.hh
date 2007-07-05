#ifndef MLN_VALUE_PROPS_HH
# define MLN_VALUE_PROPS_HH

# include <climits>
# include <cfloat>

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
      static unsigned int min() { return 0; }
      static unsigned int max() { return UINT_MAX; }
    };

    template <>
    struct props<signed int>
    {
      static signed int min() { return INT_MIN; }
      static signed int max() { return INT_MAX; }
    };

    template <>
    struct props<unsigned long int>
    {
      static unsigned long int min() { return 0; }
      static unsigned long int max() { return ULONG_MAX; }
    };

    template <>
    struct props<signed long int>
    {
      static signed long int min() { return LONG_MIN; }
      static signed long int max() { return LONG_MAX; }
    };

    // floating

    template <>
    struct props<float>
    {
      static float min() { return FLT_MIN; }
      static float max() { return FLT_MAX; }
    };

    template <>
    struct props<double>
    {
      static double min() { return DBL_MIN; }
      static double max() { return DBL_MAX; }
    };

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_PROPS_HH
