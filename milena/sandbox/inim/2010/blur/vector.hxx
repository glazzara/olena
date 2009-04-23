#ifndef VECTOR_HXX
# define VECTOR_HXX

# include "vector.hh"

template<typename T>
Vector<T>::Vector(T& a)
  : vect_ (a, a),
    orient_ (RIGHT),
    delta_ (0.25)
{ }

template<typename T>
void
Vector<T>::restart_from(T& a, T& b)
{
  vect_.first = a;
  vect_.second = b;
  orient_ = RIGHT;
}

#endif /* !VECTOR_HXX */
