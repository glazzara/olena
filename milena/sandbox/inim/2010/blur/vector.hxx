#ifndef VECTOR_HXX
# define VECTOR_HXX

# include "vector.hh"

template<typename T>
Vector<T>::Vector()
{}


template<typename T>
Vector<T>::Vector(T a)
  : vect_ (a, a),
    nb_point_ (1),
    norm_ (0),
    orient_ (RIGHT)
{ }



template<typename T>
Vector<T>::Vector(Vector<T>& vect)
{
  this->vect_.first = vect.vect_.first;
  this->vect_.second = vect.vect_.second;
  this->nb_point_ = vect.nb_point_;
  this->nb_point_ = vect.nb_point_;
  this->orient_ = vect.orient_;
  this->norm_ = vect.norm_;
}

template<typename T>
void
Vector<T>::operator= (Vector<T>& vect)
{
  this->vect_.first = vect.vect_.first;
  this->vect_.second = vect.vect_.second;
  this->nb_point_ = vect.nb_point_;
  this->nb_point_ = vect.nb_point_;
  this->orient_ = vect.orient_;
  this->norm_ = vect.norm_;
}

template<typename T>
void
Vector<T>::restart_from(T a, int orient)
{
  vect_.first = a;
  vect_.second = a;
  nb_point_ = 1;
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
void
Vector<T>::print()
{
  std::cout << "@@ From " << vect_.first
            << " To " << vect_.second
            << std::endl;
  std::cout << "@@ NB Points " << nb_point_ << std::endl;
  std::cout << "@@ Norme " << norm_ << std::endl;
  std::cout << "@@ Orientation " << orient_ << std::endl;
}

template<typename T>
bool
Vector<T>::add_point(T a, int orient)
{
  if (nb_point_ == MAX_POINTS)
    return false;

  ++nb_point_;

  vect_.second = a;

  int dx = vect_.second[0] - vect_.first[0];
  int dy = vect_.second[1] - vect_.first[1];

  norm_ = sqrt(dx * dx + dy * dy);

  orient_ = orient;

  return true;
}

template<typename T>
double
Vector<T>::scalar(Vector<T>& vec)
{
  int dx1 = vect_.second[0] - vect_.first[0];
  int dy1 = vect_.second[1] - vect_.first[1];

  int dx2 = vec.vect_.second[0] - vec.vect_.first[0];
  int dy2 = vec.vect_.second[1] - vec.vect_.first[1];

  return dx2 * dx1 + dy2 * dy1;
}

template<typename T>
double
Vector<T>::angle(Vector<T>& vec)
{
  return acos(this->scalar(vec) / (this->norm() * vec.norm()));
}

#endif /* !VECTOR_HXX */
