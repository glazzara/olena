#ifndef RANDOM_HXX_
# define RANDOM_HXX_



#include <cassert>
#include <cstdlib>
#include "random.hh"



template <typename T>
Random<T>::Random (T inf, T sup)
  : inf_ (inf), sup_ (sup)
{
  assert (sup >= inf);
  srand (time (0));
}

template <typename T>
T
Random<T>::get () const
{
  double res = (sup_ - inf_) * (((double) rand ()) / RAND_MAX) + inf_;
  return res;
}

template <>
bool
Random<bool>::get () const
{
  bool res = rand () > (RAND_MAX / 2);

  return res;
}

#endif /* !RANDOM_HXX_ */
