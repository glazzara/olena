#ifndef MINI_STD_ALGORITHM_HH
# define MINI_STD_ALGORITHM_HH

# include <algorithm>
# include "concepts.hh"


namespace abc
{

  /// \{
  ///     Facades.

  template <typename C>
  void sort(Container<C>& c);

  template <typename C, typename I>
  void copy(const Container<C>& c, I iter);

  /// \}



  /// \{
  ///     sort

  namespace impl
  {

    template <typename C>
    void sort(Random_Access_Container<C>& c)
    {
      std::sort(c.begin(), c.end());
    }

  } // impl

  template <typename C>
  void sort(Container<C>& c)
  {
    impl::sort(c.exact());
  }

  /// \}



  /// \{
  ///     copy

  template <typename C, typename I>
  void copy(const Container<C>& c, I iter)
  {
    std::copy(c.begin(), c.end(), iter);
  }

  /// \}

  

} // end of namespace abc


#endif // ! MINI_STD_ALGORITHM_HH
