#ifndef VECTOR_HXX
# define VECTOR_HXX

# include "vector.hh"

template<typename T>
Vector<T>::Vector(T& a)
  : vect_ (a, a),
    nb_point_ (0),
    norm_ (0),
    orient_ (RIGHT)
{ }

template<typename T>
void
Vector<T>::restart_from(T& a, e_orient orient)
{
  vect_.first = a;
  vect_.second = a;
  nb_point_ = 0;
  norm_ = 0;
  orient_ = orient;
}

template<typename T>
double
Vector<T>::norm()
{
  return norm_;
}

template<typename T>
bool
Vector<T>::add_point(T& a, e_orient orient)
{
  if (nb_point_ == MAX_POINTS)
    return false;

  vect_.second = a;

  int dx = vect_.second[0] - vect_.first[0];
  int dy = vect_.second[1] - vect_.first[1];

  norm_ = sqrt(dx * dx + dy * dy);

  orient_ = orient;

  return true;
}

template<typename T>
int
Vector<T>::scalar(Vector<T>& vec)
{
  return vec.vect_.first[0] * this.vect_.first[0]
    + vec.vect_.second[1] * this.vect_.second[1];
}

template<typename T>
double
Vector<T>::angle(Vector<T>& vec)
{
  return acos(this.scalar(vec) / (this.norm() * vec.norm()));
}

#endif /* !VECTOR_HXX */
