#ifndef MLN_MLC_EQUAL_HH
# define MLN_MLC_EQUAL_HH

# include <core/concept/genpoint.hh>


namespace mln
{

  namespace mlc
  {

    template <typename T1, typename T2>
    struct equal
    {
    };

    template <typename T>
    struct equal< T, T >
    {
      static void check() {}
    };


  } // end of namespace mln::mlc

} // end of namespace mln


#endif // ! MLN_MLC_EQUAL_HH
