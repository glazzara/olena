#ifndef MLC_EQUAL_HH
# define MLC_EQUAL_HH


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


#endif // ! MLC_EQUAL_HH
