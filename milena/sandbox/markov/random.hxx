#ifndef RANDOM_HXX_
# define RANDOM_HXX_



#include <cassert>
#include <cstdlib>
#include "random.hh"



template <typename T>
Random<T>::Random (T inf, T sup)
  : inf_ (inf),
    sup_ (sup),
    i_    (0)
{
  assert (sup >= inf);
  srand (time (0));
  for (int i = 0; i < size_; i++)
    values_[i] = gen();
}

template <typename T>
inline
T
Random<T>::gen () const
{
  double res = (sup_ - inf_) * (((double) rand ()) / RAND_MAX) + inf_;
  return res;
}

template <>
inline
bool
Random<bool>::gen () const
{
  bool res = rand () > (RAND_MAX / 2);
  return res;
}


template <typename T>
inline
T
Random<T>::get ()
{
  T res = values_[i_ % size_];
  ++i_;
  return res;
}

#endif /* !RANDOM_HXX_ */
