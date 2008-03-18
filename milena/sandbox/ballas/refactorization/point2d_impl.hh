#ifndef POINT2D_IMPL_HH_
# define POINT2D_IMPL_HH_

# include <internal/impl.hh>
# include <internal/force_exact.hh>


namespace mln
{

  /// fwd declaration
  template <typename C>
  struct point2d;

  template <typename E, typename C>
  struct impl< E, point2d<C> >
  {
    C  operator[](unsigned i) const;


    // Non const method
//     C& operator[](unsigned i);
//     void set_all(C c);
//     point2d<C>& operator+=(const dpoint& dp);
//     point2d<C>& operator-=(const dpoint& dp);
  };

# ifndef MLN_INCLUDE_ONLY

  template <typename E, typename C>
  C
  impl< E, point2d<C> >::operator[] (unsigned i) const
  {
    return internal::force_exact<E>(*this).to_site()[i];
  }

# endif // ! MLN_INCLUDE_ONLY
}


#endif // !POINT2D_IMPL_HH_
